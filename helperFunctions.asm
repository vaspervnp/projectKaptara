SetScrByteBW:
        ;use a 2 bit bitmap in a common way on all platforms
        push bc
        push af
        and 0xF0	;in 4 color mode we use half the 'character 
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
        and 0xF	;Do the 2nd half - we have to put it in the following byte.
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
        ret