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
                
SetPalette:		;-GRB
		ld c,a
		ld b,0x7f
		push bc
			ld a,l
			and 11110000b
			call CPCcolconv
			ld b,a	 				;R
			ld a,l
			and 00001111b  			;B
			call CPCcolconvR
			ld l,a
			ld a,h					;G
			and 00001111b
			call CPCcolconvR
			ld h,a
			xor a
			add h					;Add Green x9
			add h
			add h
			add h
			add h
			add h
			add h
			add h
			add h
			add b					;Add Red x3
			add b
			add b
			add l					;Add Blue x1
			ld b,0
			ld c,a
			ld hl,CpcPalette_Map
			add hl,bc
			ld a,(hl)
		pop bc
		out (c),c               
		out (c),a
	ret
CPCcolconvR:			;We need to shift the Red color bits
	rrca
	rrca
	rrca
	rrca
CPCcolconv:				;We need to limit our returned value to 0,1 or 2
	cp 0x50
	jr c,CPCcolconv0	
	cp 0xA0
	jr c,CPCcolconv1
	ld a,2
	ret
CPCcolconv0:	
	xor a
	ret
CPCcolconv1:	
	ld a,1
	ret                

CPCPalette_Map:
	db 0x54,0x44,0x55
        db 0x5C,0x58,0x5D
        db 0x4C,0x45,0x4D
        
        db 0x56,0x46,0x57
        db 0x5E,0x40,0x5F
        db 0x4E,0x47,0x4F
        
        db 0x52,0x42,0x53
        db 0x5A,0x59,0x5B
        db 0x4A,0x43,0x4B