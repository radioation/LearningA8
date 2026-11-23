
.setcpu "6502"
.include "atari5200.inc"

.export _x_dir_lut, _y_dir_lut, _zp_x_vel_lo, _zp_x_vel_hi, _zp_y_vel_lo, _zp_y_vel_hi, _zp_missile_index, _missile_x_lo ,_missile_x_hi ,_missile_y_lo ,_missile_y_hi

   .segment "ZEROPAGE"
_zp_x_vel_lo:
      .res 1  ;   velocity low
_zp_x_vel_hi:
  .res 1    ; velocity hi byte
_zp_y_vel_lo:
      .res 1  ;   velocity low
_zp_y_vel_hi:
  .res 1    ; velocity hi byte


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


   .segment "CODE"



.proc _move_m0



    rts
.endproc




