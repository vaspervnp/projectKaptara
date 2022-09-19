
RLE_ImageWidth equ 38	;Template Width - does not need changing
RLE_Draw:
	;ixh=width
	;HL=start of RLE data... DE=End of RLE
	;B=Start Y line
	;IXL=X-pos of Right hand side

;Byte formats
	;00000000 CCCCCCCC+ BBBBBBBB = C repetitions of Byte b (C can be more than one byte when 255+)
	;CCCC0000 BBBBBBBB ... bytes of linear byte data (there are C x bytes of B) (C can be more than byte when 16/255+)
	;RRRRCCCC ... C repetitions of RLE data R (C can be more than byte when 16/255+)

	;if a count is as high as it can be (15 for nibble - 255 for byte)... we load in the next byte and add it to count (and the next indefinately, if they are 255!)

	;RLE encoder works in nibbles... this was so it could compress 'checkerboard dithering on CPC mode 1. X X X
	;												       X X X

	ld a,ixh							;Width of image
	ld (ImageWidthA_Plus1-1),a
	ld (ImageWidthB_Plus2-2),a
	ld (ImageWidthC_Plus1-1),a
	ld (ImageWidthD_Plus2-2),a
	ld (ImageWidthE_Plus1-1),a
	cpl
	inc a								;Width
	ld (NegativeImageWidth_Plus2-2),a
	
	ld a,d								;Lastbyte of data range
	ld (RLE_LastByteH_Plus1-1),a		
	ld a,e
	ld (RLE_LastByteL_Plus1-1),a
	push hl
		ld a,IXL						;Xpos (right hand side)
		ld h,0xC0
		LD L,a
		
		ld a,b							;Move to Y line
		ld de,0xFFFF 	;<--SM***
NegativeImageWidth_Plus2:
		or a
RLE_DrawGetNextLine:
		jr z,RLE_DrawGotLine
		call RLE_NextScreenLineHL		
		add hl,de						;NextScreenHL adds width - we need to counter it!
		dec a
		jr RLE_DrawGetNextLine			;Move down to start Y-line
RLE_DrawGotLine:
		ld (RLE_ScrPos_Plus2-2),hl
		ld iyl,RLE_ImageWidth ;<--SM ***
ImageWidthA_Plus1:
		ld e,255						;We flip between 2 nibbles in CPC 4 pixel byte, 
	pop hl									;this marks which nibble we're doing

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	;Main loop
	
RLE_MoreBytesLoop:
	inc hl
	ld a,(hl)							;Read in a byte
	ld b,a
	or a
	jp z,RLE_OneByteData				;IF 0 we're repeating one byte many times
	and 00001111b						;Count nibbleb
        jp z,RLE_PlainBitmapData			;when byte is 0 we are dealing with one byte repeated, 
	ld ixh,0								;eg 129 129 129 129 
	ld ixl,a

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;we're doing Nibble data, Expand the data into two pixels of Mode 1 and duplicate
	;RRRRCCCC	R=rle data C=count	
	
	ld a,b
	and 00110000b						;Top 4 bits are the RLE data
	rrca								;We have to shift them around to get them into
	rrca									; the right position for the CPC screen
	ld c,a
	ld a,b
	and 11000000b			
	or c
	ld c,a
	rrca	
	rrca
	or c
	ld c,a
	ld a,ixl							;See what our count is
	cp 15
	jr nz,RLE_NoMoreNibbleBytes			;if count is 15, then we need to 
	push de									;load in the next byte as well! 
RLE_MoreNibbleBytes:
		inc hl
		ld a,(hl)
		ld d,0
		ld e,a
		add ix,de
		cp 255							;if the next byte is 255 we need 
		jr z,RLE_MoreNibbleBytes			;to load in the next byte as well!
	pop de
RLE_NoMoreNibbleBytes:

	ld a,e
	or a
	jr z,RLE_MoreBytesPart2Flip			;We need to flip the RLE nibbles 
											;before we can write them to the screen
	ld a,ixl
	cp 4
	call nc,RLE_ByteNibbles				;We've got at least 4 nibbles to write - 
											;so use the 'whole byte writer' (as it's faster)
	xor a
	ld d,a				 				;Clear the 'build up' byte for screen

	push hl
		ld hl,0xC050 ;<--SM ***
RLE_ScrPos_Plus2:
		ld b,iyl						;Get back our current byte number in the line
RLE_MoreBytes:
		ld a,c
		and 00110011b					;Write this nibble - note the pattern matches 
		or d								;CPC Mode 1 data
		ld d,a

		dec ix							;Decrease rle count
		ld a,ixl						;Have we done all our bytes?
		or ixh
		jr z,RLE_LastByteFlip			;need to do the last byte
RLE_MoreBytesPart2:
		ld a,c
		and 11001100b					;Write the second nibble - or in the last one
		or d
		ld d,a
	
		dec ix							;Decrease rle count
		ld (hl),d						;Write byte to screen
		dec hl							;Move to next byte

		dec b							;See if we've done all the bytes for this line
		call z,RLE_NextScreenLineHL

		xor a
		ld d,a 							;Clear the 'build up' byte for screen
	
		ld a,ixl						;Have we done all our bytes?
		or ixh
		jr nz,RLE_MoreBytes				;need to do the last byte
RLE_LastByte:
		ld iyl,b						;Store our current byte number in the line for later
		ld (RLE_ScrPos_Plus2-2),hl
	pop hl

	ld a,0x00			;<--SM ***
RLE_LastByteH_Plus1:					;Have we processed all the RLE bytes yet?
	cp h
	jr nz,RLE_MoreBytesLoop				;if not then process more...

	ld a,0x00			;<--SM ***
RLE_LastByteL_Plus1:
	cp l
	jr nz,RLE_MoreBytesLoop
	ret									;RLE Finished - so return
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
RLE_LastByteFlip:
	ld a,e								;E remembers which half of the nibble we're working on.
	cpl
	ld e,a
	jr RLE_LastByte
RLE_MoreBytesPart2Flip:					;We're already half way through a nibble
	push hl
	ld b,iyl							;Refresh B
	ld hl,(RLE_ScrPos_Plus2-2)			;Restore HL
	ld a,e				
	cpl									;Flip E
	ld e,a
	jr RLE_MoreBytesPart2 				;Carry on!

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RLE_NextScreenLineHL:					;Mode down a line in HL (line counter is B)
	push de
		ld b,RLE_ImageWidth ;<--SM ***
ImageWidthE_Plus1						;Reset B
		ld de,0x800+RLE_ImageWidth ;<--SM ***
ImageWidthD_Plus2
		add hl,de						;Add width
	pop de
	ret nc
	push de
		ld de,0xc050						;Note this only works for screen at &c000 
		add hl,de						;	- it's too slow to double buffer anyway.
	pop de
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RLE_NextScreenLine:						;Move down a line in DE (line counter is IYL)
	push hl
		ld iyl,RLE_ImageWidth ;<--SM ***
ImageWidthC_Plus1:
		ld hl,0x800+RLE_ImageWidth ;<--SM ***
ImageWidthB_Plus2:
		add hl,de
		ex de,hl
	pop hl
	ret nc
	push hl
		ld hl,0xc050
		add hl,de
		ex de,hl
	pop hl
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RLE_PlainBitmapData: 					;We're going to read in a series of bytes that 
	push de									;are not the same (eg 123 54 99 etc)
	
		ld a,(hl)						;Load in the count from the nibble
		rrca
		rrca
		rrca
		rrca
		ld b,0
		ld c,a
		cp 15
		jr nz,RLE_PlainBitmapDataNoExtras	;if count is 15 then we need to
												; load in next byte!
RLE_PlainBitmapDataHasExtras:
		inc hl
		ld a,(hl)				;if nibble was 15, but next byte is 0, then we're done
		push hl
			ld h,0
			ld l,a
			add hl,bc			;More bytes
			ld b,h
			ld c,l
		pop hl
		cp 255
		jr z,RLE_PlainBitmapDataHasExtras
		
RLE_PlainBitmapDataNoExtras:
		ld de,(RLE_ScrPos_Plus2-2)	;Get screenpos into DE
RLE_PlainBitmapData_More:
		inc hl
		ld a,(hl)					;Read in a byte from HL - write it to screen at DE
		ld (de),a
		dec de

		dec iyl						;Decrease line count - see if we need to do newline
		call z,RLE_NextScreenLine
		dec bc
		ld a,b
		or c
		jr nz,RLE_PlainBitmapData_More ;See if we have more data?
		ld (RLE_ScrPos_Plus2-2),de
	pop de
	jp RLE_MoreBytesLoop			;We're done - so go back to our normal loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
RLE_OneByteData:					;We're going to write a series of bytes
	push de							;	 - all of which are the same (eg 99 99 99 99 )
		xor a 
		ld b,a
		ld c,a						;BC will be our count
RLE_OneByteDataExtras:
		inc hl
		ld a,(hl)
		push hl
			ld h,0
			ld l,a
			add hl,bc				;Add next byte to our count
			ld b,h
			ld c,l
		pop hl

		cp 255
		jr z,RLE_OneByteDataExtras		;if it's 255 - add another!

		inc hl
		ld a,(hl)
		ld (RLE_ThisOneByte_Plus1-1),a	;read in 'the byte'

		ld de,(RLE_ScrPos_Plus2-2)		;Get screenpos

RLE_OneByteData_More:	
		ld a,00		;<--SM ***
RLE_ThisOneByte_Plus1:					;Write the byte to DE
		ld (de),a
		dec de
		dec iyl
		call z,RLE_NextScreenLine		;Do a new line if we get to the end of this line

		dec bc
		ld a,b
		or c
		jr nz,RLE_OneByteData_More		;Do we have more data?

		ld (RLE_ScrPos_Plus2-2),de		;Store the current screenpos for later
	pop de
	jp RLE_MoreBytesLoop		;Jump back to our main loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RLE_ByteNibbles:				;We're going to write whole bytes for extra speed
	di
	ld a,c						;C=Byte we're going to write
	exx							;use shadow regs

	ld b,iyl					;line byte count
	ld c,a						;get back c

	ld d,ixh					;count of bytes to write
	ld e,ixl

	ld hl,(RLE_ScrPos_Plus2-2)	;Get back screen mempos
RLE_ByteNibblesMore3:			;to write then stop and do normal processing
	ld a,3						;for compare below
RLE_ByteNibblesMore:
	ld (hl),c					;Write byte to screen
	dec hl 
	dec b;iyl
	call z,RLE_NextScreenLineHL	;See if

	dec de
	dec de
	cp e						;Compare E to 3 - if we don't have at least 2 bytes 
	jr c,RLE_ByteNibblesMore	;  to write then stop and do normal processing

	ld a,d
	or a
	jr nz,RLE_ByteNibblesMore3	;If we've got more bytes to write then do another batch

	ld (RLE_ScrPos_Plus2-2),hl	;Store the screen pos
	ld iyl,b					;put the new screenpos vars back into normal regs
	ld ixh,d
	ld ixl,e
	exx							;Page back non shadow regs.
	ret




	
	
