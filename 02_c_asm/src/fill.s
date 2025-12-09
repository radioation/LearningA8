        .setcpu "6502"
        .include "atari.inc"
        .include "zeropage.inc"
        
.export _clear10, _filler, _zfiller
.import popa


.segment "ZEROPAGE"
charptr:
    .res 2  ; reserve two bytes for 16-bit pointer.

_zval:    .res 1
_zcount:    .res 1
_zptr: .res 2
.exportzp _zval, _zcount, _zptr



.segment "CODE"


.proc _clear10
    ; string pointer is in the A/X registers (__Fastcall_)
    ; A = low-byte of 16-bit pointer
    ; X = high byte of 16-bit pointer
    sta charptr          ; low byte of char arry address
    stx charptr + 1      ; high byte of char arry address

    lda #$00
    ldy #$00
@loop:
    sta (charptr), y     ; store reg A value 
    iny                  ; next y

    cpy #$0A             ; at 10?
    bne @loop

    rts
.endproc





.proc _filler
    sta charptr          ; low byte of char arry address
    stx charptr + 1      ; high byte of char arry address

    jsr popa             ; param 2 is now in the accumulator
    sta tmp1             ; store temporarily to not interfere with other pops

    jsr popa             ; param 1 is now in the A register

    ldy tmp1             ; get count 
    dey                  ; counting down, so decerment before we store.
@dloop:
    sta (charptr), y        ; store reg A value 
    dey                  ; decrement y
    bpl @dloop

    rts


.endproc

.proc _zfiller
    sta _zval

    ldy _zcount          ; get count 
    dey                  ; counting down, so decerment before we store.
@dloop:
    sta (_zptr), y        ; store reg A value 
    dey                  ; decrement y
    bpl @dloop

    rts


.endproc

