SetScrByteBW:
	push bc
		push af
			and 11110000b	;in 4 color mode we use half the 'character 
			ld b,a
			rrca
			rrca
			rrca
			rrca
			or b
			ld (hl),a
			inc hl
		pop af
		push af
			and 00001111b	;Do the 2nd half - we have to put it in the following byte.
			ld b,a
			rrca
			rrca
			rrca
			rrca
			or b
			ld (hl),a
			inc hl
		pop af
		pop bc
	

GetScreenPos:
	ld a,c			;Yline
	and 11111100b
	sla a			;Logical units to lines
	ld c,a
	srl b			;Logical units to bytes

	;Looking at Y line (in C) - we need to take each set of bits, and work with them separately:
	;YYYYYYYY	Y line number (0-200)
	;-----YYY	(0-7)  - this part needs to be multiplied by &0800 and added to the total
	;YYYYY---	(0-31) - This part needs to be multiplied by 64 (&40)

	;screen is 64 bytes wide = -------- %01000000
	ld a,C	
	and 11111000b 	; -------- YYYYY--- *8
	ld l,a
	xor a
	sla l			; -------Y YYYY----	*16
	rla
	sla l			; ------YY YYY----- *32
	rla
	sla l			; -----YYY YY------	*64
	rla
	ld h,a			;We've now effectively multiplied by 64 (&40)

		;-----YYY	(0-7)  - this part needs to be multiplied by
	ld a,C			;&0800 and added to the total
	and 00000111b	
	rlca			;X8
	rlca
	rlca
	add 0xC0			;Screen Base

	ld d,a			;Load into top byte, and add as 16 bit
	ld e,b			;Xpos
	add hl,de 	
	ret 
        
GetNextLine:
	push af

		ld a,h		;Add &08 to H (each CPC line is &0800 bytes below the last
		add 0x08
		ld h,a
			;Every 8 lines we need to jump back to the top of the memory range to get the correct line
			;The code below will check if we need to do this - yes it's annoying but that's just the way the CPC screen is!
		bit 7,h		;Change this to bit 6,h if your screen is at &8000!
		jp nz,GetNextLineDone
		push bc
			ld bc,0x050	;if we got here we need to jump back to the top of the screen - the command here will do that
			add hl,bc
		pop bc
GetNextLineDone:
                pop af
                ret        