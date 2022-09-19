  
;Firmware macros

CLS equ 0xBC14
PrintChar equ 0xBB5A
WaitChar equ 0xBB06
TestChar equ 0xBB09   
IntroPos equ 0x0003
KeysPos equ 0x0015
TXT_GET_WINDOW equ 0xBB66   
TXT_CLEAR_WINDOW equ 0xBB6C 
TXT_SET_GRAPHICS equ 0xBB63
SCR_SET_BORDER	equ	0xBC38
TXT_SET_CURSOR equ 0xBB75
TXT_SET_BACK equ 0xBB9F  ; if a is non zero then text background is transparent
SCR_SET_MODE equ 0xbc0e


;Colors
cBlack equ 0
cBlue equ 1
cBrightBlue equ 2
cRed equ 3
cMagenta equ 4
cMauve equ 5
cBrightRed equ 6
cPurple equ 7
cBrightMagenta equ 8
cGreen equ 9
cCyan equ 10
cSkyBlue equ 11
cYello equ 12
cWhite equ 13
cPastelBlue equ 14
cOrange equ 15
cPing equ 16
cPastelMagenta equ 17
cBrightGreen equ 18
cSeaGreen equ 19
cBrightCyan equ 20
cLime equ 21
cPastelGreen equ 22
cPastelCyan equ 23
cBrightYellow equ 24
cPastelYellow equ 25
cBrightWhite equ 26




    org  0x4000     ; start of code

    
Start:
    ld  a,1		; graphics mode
    call SCR_SET_MODE		; SCR_SET_MODE
    call CLS
; set border color
    ld  hl,PalData
    ld  b,(hl)
    ld  c,b
    call SCR_SET_BORDER		; SCR_SET_BORDER
    ld  b,0x10		; loop counter
; read palette from memory
    ld  hl,PalData+15
Loop1:
    push hl
    push bc
    ld  a,b
    dec a
    and a,0x0f
    ld  b,(hl)
    ld  c,b
    call 0xbc32		; SCR_SET_INK
    pop bc
    pop hl
    dec hl
    djnz Loop1
; set image bytes
    ;call testLetters
    ld	de,0xc000   ; DE = screen
    ld	hl,ImgData  ; HL = image data
    ld 	bc,0x4000   ; BC = # of bytes   
    ldir            ; copy
    call Intro
    ld hl, IntroPos
    push hl
    call Locate
    ld hl, IntroLines
    call PrintString
    call DELAY
    call DELAY
    call DELAY
    
    ld	de,0xc000   ; DE = screen
    ld	hl,ImgData  ; HL = image data
    ld 	bc,0x4000   ; BC = # of bytes   
    ldir            ; copy
    ld h, 4
    ld d, 36
    ld l, 21
    ld e, 23
    call TXT_SET_GRAPHICS
    call TXT_GET_WINDOW
    call TXT_CLEAR_WINDOW
    call Intro2
    call Loop
    
testLetters;    
    call CLS
    ld bc,0x0120;X byte 1 - Y line 20
    call GetScreenPos
    ld de,ltA
    Call BitmapAgain
    
StartGameLoop:
	
	
	jp StartGameLoop
   

Loop:
        call TestForQuitOrSkip
    	jp loop        ; infinite 
        
        
BitmapAgain:
	ld a,(de)
	Call SetScrByteBW ;Draw a letter A
	Call SetScrByteBW ;Draw a letter A
	Call SetScrByteBW ;Draw a letter A
	inc de
	call GetNextLine	;Move down a line
	or a			        
        jp nz,BitmapAgain	;Keep writing until we
        ;di
        ;ret
        

Done:
	call 0x0000
        
        
Intro2:
    	ld hl, KeyPresses2
    	call PrintIntro
        ret            
    
Intro:
	ld hl, KeysPos
    	call Locate 
    	ld hl, KeyPresses
    	call PrintIntro
        ret c        

Locate:
	push hl
        inc h
        inc l
        call TXT_SET_CURSOR
        pop hl
        ret

NewLine:
	ld a,13
        call PrintChar
        ld a,10
        call PrintChar
        ret
        
TestForQuitOrSkip:
	call TestChar
        ld (tempMem),hl
        ld l, a
        ld h, 0
        cp 0x51
        jp z, Done
        cp 0x53
        jp z, StartGame
        ld hl, (tempMem)
        ret
        

StartGame: 
	;ld  hl, 0x0
        ld  b,cBlack
    	ld  c,b
    	call scr_set_border
        ld  a,0
        call SCR_SET_MODE; SCR_SET_MODE
        call CLS
        Ld b,160   ;Y-Start
        ld ixh,20	;Width
        ld IXL,70-1	;X-Righthandside
        call SpriteBull_Setup
        call RLE_Draw
        di
        ;Ld b,160   ;Y-Start
        ;ld ixh,20	;Width
        ;ld IXL,30-1	;X-Righthandside
        ;call SpriteBull_Setup
        ;call RLE_Draw
        ;di
        call StartGameLoop  
 	
	include "cpc_rle.inc"

        
delayAndNext:
	call TestForQuitOrSkip
	ld (tempMem),hl
	call DELAY
        call TestForQuitOrSkip
    	call DELAY
    	ld hl, IntroPos
    	call Locate
        ld hl, (tempMem)
	inc hl
        call PrintChar
        call PrintString
        ret
        
PrintString:
	ld a,(hl)
        cp 254
        ret z
        cp 255
        jr z, delayAndNext
        call PrintChar
        inc hl
        jr PrintString
        
DELAY:
        NOP
        DEC BC
        LD A,B
        OR C
        RET Z
        JR DELAY  

PrintIntro:
    ld a,(hl)
    cp 255
    ret z
    call PrintChar
    inc hl
    jr PrintIntro
    
DOINIT:
	ret
        
SHUTDOWN:
	ret
        
SpriteBull_Setup:
     ld hl,SpriteBull_Start-1
     ld de,SpriteBull_End-1

     ret
     
Palette:
    defw 0x0000 ;0  -GRB
    defw 0x0080 ;1  -GRB
    defw 0x0800 ;2  -GRB
    defw 0x0880 ;3  -GRB
    defw 0x0008 ;4  -GRB
    defw 0x0088 ;5  -GRB
    defw 0x0808 ;6  -GRB
    defw 0x0CCC ;7  -GRB
    defw 0x0888 ;8  -GRB
    defw 0x00F0 ;9  -GRB
    defw 0x0F00 ;10  -GRB
    defw 0x0FF0 ;11  -GRB
    defw 0x000F ;12  -GRB
    defw 0x00FF ;13  -GRB
    defw 0x0F0F ;14  -GRB
    defw 0x0FFF ;15  -GRB     
        
include "helperFunctions.inc"

        

PalData:
    db $0D,$1A,$10,$19,$00,$00,$00,$00
    db $00,$00,$00,$00,$00,$00,$00,$00
ImgData:            ; data file
    incbin "tavrokathapsia-1-cpc.mode1.bin"
KeyPresses: 
	db '    Press Q to quit, S to skip intro    ',255
KeyPresses2: 
	db 'Press Q to quit, S to skip intro',255        
IntroLines:
	db '                                        ',255, '   Around 3800 years before our time,   ',255 
	db 'on the great island we now know as Crete',255, '   preparations for the great annual    ',255
	db ' religious festival in Knossos were in  ',255, '              full swing.               ',255 
	db 'With the and ceremonies of this festival',255, '   and especially the Taurokathapsia    ',255 
	db '   (Bull-leaping) ritual, the Keftiu    ',255, '(Minoans) would thank the Mother Goddess',255
	db ' and the other deities for the crop and ',255, '   the wealth granted to them in the    ',255 
	db ' previous year, and ask for their favor ',255, '           for the next one.            ',254 
tempMem:
     	db 0
KeepA: db 0
keepC: db 0
ltA: 
	db 00011100b		;Here is a letter A in Bits
	db 00100110b
	db 01000011b
	db 01000011b
	db 01111111b
	db 01000011b
	db 01000011b
	db 00000000b
SpriteBull_Start:
      db 0xF,0x14,0xF1,0x3,0xF1,0xF,0x14,0xF1,0x2,0xF2,0xF,0x13,0xF1,0x2,0xF3,0xF
      db 0x11,0xF3,0x1,0xF3,0xF,0x10,0xF3,0x2,0xF4,0xF,0xF,0xF9,0xF,0xF,0xF1,0x4
      db 0xF4,0xF,0x10,0xF2,0x3,0xF3,0xF,0x11,0xF3,0x2,0xF3,0xF,0xF,0x10,0x33,0xF8
      db 0xC1,0xF,0xC,0xC3,0xF7,0x10,0xCC,0xC1,0xF,0x4,0xC9,0xF1,0x10,0xFC,0x1,0xF5
      db 0x10,0xCC,0xC2,0xF,0x2,0xC7,0xF7,0x1,0xF7,0xC2,0xE,0xC3,0xFE,0x2,0xF6,0xC2
      db 0xB,0xC3,0xFF,0xB,0x8,0xC3,0xFF,0xE,0x7,0xFF,0x12,0x7,0xFF,0xF,0xC2,0xF1
      db 0x6,0xFF,0x9,0xC1,0xF4,0xC3,0x8,0xFF,0x9,0xC1,0xF4,0xC1,0x9,0xFF,0xA,0xC2
      db 0xF3,0xC1,0x9,0xFF,0xB,0xC1,0x10,0xCC,0xC2,0x8,0xC1,0xFF,0xB,0xC2,0x1,0xF1
      db 0x8,0xC2,0xFF,0xE,0x9,0xC2,0xFF,0xE,0x8,0xC2,0x1,0xFF,0xF,0x6,0xC2,0x2
      db 0xFF,0xF,0x6,0xC1,0x3,0xFC,0xC1,0x10,0xF3,0xFF,0x1,0x4,0xC2,0x3,0xFA,0xC6
      db 0xFF,0x0,0x4,0xC3,0x3,0xF7,0xC8,0xFF,0x1,0x4,0xC2,0x3,0xF7,0xCA,0xF6,0xC1
      db 0x20,0xF3,0xF3,0xF3,0xB,0xF6,0x4,0xC5,0xF1,0x10,0xFC,0xC9,0xF3,0xB,0xF8,0x2
      db 0xCD,0x4,0xF2,0xC,0xF2,0x1,0xF4,0x3,0xCC,0x10,0xC0,0x1,0xF3,0xE,0xF4,0xC3
      db 0x3,0xC9,0x1,0xC2,0xF4,0xF,0x3,0xC4,0x4,0xC6,0x2,0xC2,0xF3,0xF,0x14,0xC2
      db 0xF2,0xF,0x6D
SpriteBull_End: