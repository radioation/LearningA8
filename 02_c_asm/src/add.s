        .setcpu "6502"
        .include "atari.inc"
        .include "zeropage.inc"   ; brings in tmp1 
        
.export _u_add8
.import popa

.segment "CODE"

; Calling convention (fastcall, 8-bit):
;   a is in A on entry
;   b is on the C stack
; Return:
;   result in A
.proc _u_add8
    sta tmp1        ; save A to tmp1
    jsr popa        ; get from C stack
   
    clc             ; clear carry
    adc tmp1        ; add with carry 

    rts             ; result is in A register
.endproc



