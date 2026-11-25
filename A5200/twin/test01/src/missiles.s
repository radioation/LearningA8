
.setcpu "6502"
.include "atari5200.inc"

.export _x_dir_lut, _y_dir_lut, _zp_vel_lo, _zp_vel_hi, _zp_lut_index, _zp_missile_index, _missile_x_lo ,_missile_x_hi ,_missile_y_lo ,_missile_y_hi, _x_dir_lut_hi, _y_dir_lut_hi, _x_dir_lut_lo, _y_dir_lut_lo, _move_m0


   .segment "ZEROPAGE"
_zp_vel_lo:
      .res 1  ;   velocity low
_zp_vel_hi:
  .res 1    ; velocity hi byte

_zp_lut_index:
    .res 1

_zp_missile_index:    ; which missile are we looking at (0-3)
   .res 1



.segment "BSS"
    _missile_x_lo: .res 4
    _missile_x_hi: .res 4

    _missile_y_lo: .res 4
    _missile_y_hi: .res 4


.segment "RODATA"

; X LUT ---------------------------------
x_dir_lut:
    .byte $DE,$E1,$E4,$E8,$ED,$F3,$F9,$00,$07,$0D,$13,$18,$1C,$1F,$22,$00
    .byte $DC,$DE,$E1,$E5,$EB,$F1,$F8,$00,$08,$0F,$15,$1B,$1F,$22,$24,$00
    .byte $D9,$DB,$DE,$E2,$E7,$EE,$F7,$00,$09,$12,$19,$1E,$22,$25,$27,$00
    .byte $D6,$D8,$DB,$DE,$E3,$EB,$F4,$00,$0C,$15,$1D,$22,$25,$28,$2A,$00
    .byte $D4,$D5,$D7,$DA,$DE,$E5,$F1,$00,$0F,$1B,$22,$26,$29,$2B,$2C,$00
    .byte $D2,$D2,$D3,$D5,$D8,$DE,$EB,$00,$15,$22,$28,$2B,$2D,$2E,$2E,$00
    .byte $D0,$D1,$D1,$D1,$D2,$D5,$00,$00,$00,$2B,$2E,$2F,$2F,$2F,$30,$00
    .byte $D0,$D0,$D0,$D0,$D0,$00,$00,$00,$00,$00,$30,$30,$30,$30,$30,$00
    .byte $D0,$D1,$D1,$D1,$D2,$D5,$00,$00,$00,$2B,$2E,$2F,$2F,$2F,$30,$00
    .byte $D2,$D2,$D3,$D5,$D8,$DE,$EB,$00,$15,$22,$28,$2B,$2D,$2E,$2E,$00
    .byte $D4,$D5,$D7,$DA,$DE,$E5,$F1,$00,$0F,$1B,$22,$26,$29,$2B,$2C,$00
    .byte $D6,$D8,$DB,$DE,$E3,$EB,$F4,$00,$0C,$15,$1D,$22,$25,$28,$2A,$00
    .byte $D9,$DB,$DE,$E2,$E7,$EE,$F7,$00,$09,$12,$19,$1E,$22,$25,$27,$00
    .byte $DC,$DE,$E1,$E5,$EB,$F1,$F8,$00,$08,$0F,$15,$1B,$1F,$22,$24,$00
    .byte $DE,$E1,$E4,$E8,$ED,$F3,$F9,$00,$07,$0D,$13,$18,$1C,$1F,$22,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

_x_dir_lut = x_dir_lut
; Y LUT ---------------------------------
y_dir_lut:
    .byte $DE,$DC,$D9,$D6,$D4,$D2,$D0,$D0,$D0,$D2,$D4,$D6,$D9,$DC,$DE,$00
    .byte $E1,$DE,$DB,$D8,$D5,$D2,$D1,$D0,$D1,$D2,$D5,$D8,$DB,$DE,$E1,$00
    .byte $E4,$E1,$DE,$DB,$D7,$D3,$D1,$D0,$D1,$D3,$D7,$DB,$DE,$E1,$E4,$00
    .byte $E8,$E5,$E2,$DE,$DA,$D5,$D1,$D0,$D1,$D5,$DA,$DE,$E2,$E5,$E8,$00
    .byte $ED,$EB,$E7,$E3,$DE,$D8,$D2,$D0,$D2,$D8,$DE,$E3,$E7,$EB,$ED,$00
    .byte $F3,$F1,$EE,$EB,$E5,$DE,$D5,$00,$D5,$DE,$E5,$EB,$EE,$F1,$F3,$00
    .byte $F9,$F8,$F7,$F4,$F1,$EB,$00,$00,$00,$EB,$F1,$F4,$F7,$F8,$F9,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    .byte $07,$08,$09,$0C,$0F,$15,$00,$00,$00,$15,$0F,$0C,$09,$08,$07,$00
    .byte $0D,$0F,$12,$15,$1B,$22,$2B,$00,$2B,$22,$1B,$15,$12,$0F,$0D,$00
    .byte $13,$15,$19,$1D,$22,$28,$2E,$30,$2E,$28,$22,$1D,$19,$15,$13,$00
    .byte $18,$1B,$1E,$22,$26,$2B,$2F,$30,$2F,$2B,$26,$22,$1E,$1B,$18,$00
    .byte $1C,$1F,$22,$25,$29,$2D,$2F,$30,$2F,$2D,$29,$25,$22,$1F,$1C,$00
    .byte $1F,$22,$25,$28,$2B,$2E,$2F,$30,$2F,$2E,$2B,$28,$25,$22,$1F,$00
    .byte $22,$24,$27,$2A,$2C,$2E,$30,$30,$30,$2E,$2C,$2A,$27,$24,$22,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_y_dir_lut = y_dir_lut

x_dir_lut_hi:
    .byte $FD,$FE,$FE,$FE,$FE,$FF,$FF,$00,$00,$00,$01,$01,$01,$01,$02,$00
    .byte $FD,$FD,$FE,$FE,$FE,$FF,$FF,$00,$00,$00,$01,$01,$01,$02,$02,$00
    .byte $FD,$FD,$FD,$FE,$FE,$FE,$FF,$00,$00,$01,$01,$01,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FE,$FE,$FF,$00,$00,$01,$01,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FD,$FE,$FF,$00,$00,$01,$02,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FD,$FD,$FE,$00,$01,$02,$02,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FD,$FD,$00,$00,$00,$02,$02,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FD,$00,$00,$00,$00,$00,$03,$03,$03,$03,$03,$00
    .byte $FD,$FD,$FD,$FD,$FD,$FD,$00,$00,$00,$02,$02,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FD,$FD,$FE,$00,$01,$02,$02,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FD,$FE,$FF,$00,$00,$01,$02,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FD,$FE,$FE,$FF,$00,$00,$01,$01,$02,$02,$02,$02,$00
    .byte $FD,$FD,$FD,$FE,$FE,$FE,$FF,$00,$00,$01,$01,$01,$02,$02,$02,$00
    .byte $FD,$FD,$FE,$FE,$FE,$FF,$FF,$00,$00,$00,$01,$01,$01,$02,$02,$00
    .byte $FD,$FE,$FE,$FE,$FE,$FF,$FF,$00,$00,$00,$01,$01,$01,$01,$02,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_x_dir_lut_hi = x_dir_lut_hi
x_dir_lut_lo:
    .byte $E1,$0C,$42,$83,$D1,$2D,$93,$00,$6D,$D3,$2F,$7D,$BE,$F4,$1F,$00
    .byte $B9,$E1,$14,$56,$A9,$0D,$82,$00,$7E,$F3,$57,$AA,$EC,$1F,$47,$00
    .byte $8F,$B2,$E1,$20,$75,$E3,$69,$00,$97,$1D,$8B,$E0,$1F,$4E,$71,$00
    .byte $65,$81,$A8,$E1,$33,$A9,$46,$00,$BA,$57,$CD,$1F,$58,$7F,$9B,$00
    .byte $3E,$51,$6D,$9A,$E1,$56,$0D,$00,$F3,$AA,$1F,$66,$93,$AF,$C2,$00
    .byte $1E,$27,$37,$51,$81,$E1,$A9,$00,$57,$1F,$7F,$AF,$C9,$D9,$E2,$00
    .byte $08,$0A,$0F,$17,$27,$51,$00,$00,$00,$AF,$D9,$E9,$F1,$F6,$F8,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    .byte $08,$0A,$0F,$17,$27,$51,$00,$00,$00,$AF,$D9,$E9,$F1,$F6,$F8,$00
    .byte $1E,$27,$37,$51,$81,$E1,$A9,$00,$57,$1F,$7F,$AF,$C9,$D9,$E2,$00
    .byte $3E,$51,$6D,$9A,$E1,$56,$0D,$00,$F3,$AA,$1F,$66,$93,$AF,$C2,$00
    .byte $65,$81,$A8,$E1,$33,$A9,$46,$00,$BA,$57,$CD,$1F,$58,$7F,$9B,$00
    .byte $8F,$B2,$E1,$20,$75,$E3,$69,$00,$97,$1D,$8B,$E0,$1F,$4E,$71,$00
    .byte $B9,$E1,$14,$56,$A9,$0D,$82,$00,$7E,$F3,$57,$AA,$EC,$1F,$47,$00
    .byte $E1,$0C,$42,$83,$D1,$2D,$93,$00,$6D,$D3,$2F,$7D,$BE,$F4,$1F,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_x_dir_lut_lo = x_dir_lut_lo
; Y LUT ---------------------------------
y_dir_lut_hi:
    .byte $FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$00
    .byte $FE,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FE,$00
    .byte $FE,$FE,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FE,$FE,$00
    .byte $FE,$FE,$FE,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FE,$FE,$FE,$00
    .byte $FE,$FE,$FE,$FE,$FD,$FD,$FD,$FD,$FD,$FD,$FD,$FE,$FE,$FE,$FE,$00
    .byte $FF,$FF,$FE,$FE,$FE,$FD,$FD,$00,$FD,$FD,$FE,$FE,$FE,$FF,$FF,$00
    .byte $FF,$FF,$FF,$FF,$FF,$FE,$00,$00,$00,$FE,$FF,$FF,$FF,$FF,$FF,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$00,$00,$01,$00,$00,$00,$01,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$01,$01,$01,$02,$02,$00,$02,$02,$01,$01,$01,$00,$00,$00
    .byte $01,$01,$01,$01,$02,$02,$02,$03,$02,$02,$02,$01,$01,$01,$01,$00
    .byte $01,$01,$01,$02,$02,$02,$02,$03,$02,$02,$02,$02,$01,$01,$01,$00
    .byte $01,$01,$02,$02,$02,$02,$02,$03,$02,$02,$02,$02,$02,$01,$01,$00
    .byte $01,$02,$02,$02,$02,$02,$02,$03,$02,$02,$02,$02,$02,$02,$01,$00
    .byte $02,$02,$02,$02,$02,$02,$02,$03,$02,$02,$02,$02,$02,$02,$02,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_y_dir_lut_hi = y_dir_lut_hi
y_dir_lut_lo:
    .byte $E1,$B9,$8F,$65,$3E,$1E,$08,$00,$08,$1E,$3E,$65,$8F,$B9,$E1,$00
    .byte $0C,$E1,$B2,$81,$51,$27,$0A,$00,$0A,$27,$51,$81,$B2,$E1,$0C,$00
    .byte $42,$14,$E1,$A8,$6D,$37,$0F,$00,$0F,$37,$6D,$A8,$E1,$14,$42,$00
    .byte $83,$56,$20,$E1,$9A,$51,$17,$00,$17,$51,$9A,$E1,$20,$56,$83,$00
    .byte $D1,$A9,$75,$33,$E1,$81,$27,$00,$27,$81,$E1,$33,$75,$A9,$D1,$00
    .byte $2D,$0D,$E3,$A9,$56,$E1,$51,$00,$51,$E1,$56,$A9,$E3,$0D,$2D,$00
    .byte $93,$82,$69,$46,$0D,$A9,$00,$00,$00,$A9,$0D,$46,$69,$82,$93,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    .byte $6D,$7E,$97,$BA,$F3,$57,$00,$00,$00,$57,$F3,$BA,$97,$7E,$6D,$00
    .byte $D3,$F3,$1D,$57,$AA,$1F,$AF,$00,$AF,$1F,$AA,$57,$1D,$F3,$D3,$00
    .byte $2F,$57,$8B,$CD,$1F,$7F,$D9,$00,$D9,$7F,$1F,$CD,$8B,$57,$2F,$00
    .byte $7D,$AA,$E0,$1F,$66,$AF,$E9,$00,$E9,$AF,$66,$1F,$E0,$AA,$7D,$00
    .byte $BE,$EC,$1F,$58,$93,$C9,$F1,$00,$F1,$C9,$93,$58,$1F,$EC,$BE,$00
    .byte $F4,$1F,$4E,$7F,$AF,$D9,$F6,$00,$F6,$D9,$AF,$7F,$4E,$1F,$F4,$00
    .byte $1F,$47,$71,$9B,$C2,$E2,$F8,$00,$F8,$E2,$C2,$9B,$71,$47,$1F,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_y_dir_lut_lo = y_dir_lut_lo


   .segment "CODE"



.proc _move_m0
    LDX _zp_lut_index ; X is used to get velocity components.
    LDY #0            ; Y is missile slot, just doing misile 0 here

    clc     ; get ready to add
    LDA _missile_x_lo, Y ; get M0 fractional component 
    ADC x_dir_lut_lo, X     ; add low velocity to it
    STA _missile_x_lo, Y ; save it

    LDA _missile_x_hi, Y ; get M0 integer component
    ADC x_dir_lut_hi, X     ; add hi velocity to it
    STA _missile_x_hi, Y ; save it

    
    clc     ; get ready to add
    LDA _missile_y_lo, Y ; get M0 fractional component 
    ADC y_dir_lut_lo, X     ; add low velocity to it
    STA _missile_y_lo, Y ; save it

    LDA _missile_y_hi, Y ; get M0 integer component
    ADC y_dir_lut_hi, X     ; add hi velocity to it
    STA _missile_y_hi, Y ; save it

    rts
.endproc




