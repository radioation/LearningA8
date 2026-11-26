
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

; X LUT ---------------------------------
x_dir_lut_hi:
    .byte $FD,$FD,$FD,$FE,$FE,$FE,$FF,$00,$00,$01,$01,$01,$02,$02,$02,$00
    .byte $FC,$FD,$FD,$FD,$FE,$FE,$FF,$00,$00,$01,$01,$02,$02,$02,$03,$00
    .byte $FC,$FC,$FD,$FD,$FD,$FE,$FF,$00,$00,$01,$02,$02,$02,$03,$03,$00
    .byte $FC,$FC,$FC,$FD,$FD,$FE,$FF,$00,$00,$01,$02,$02,$03,$03,$03,$00
    .byte $FC,$FC,$FC,$FC,$FD,$FD,$FE,$00,$01,$02,$02,$03,$03,$03,$03,$00
    .byte $FC,$FC,$FC,$FC,$FC,$FD,$FE,$00,$01,$02,$03,$03,$03,$03,$03,$00
    .byte $FC,$FC,$FC,$FC,$FC,$FC,$00,$00,$00,$03,$03,$03,$03,$03,$03,$00
    .byte $FC,$FC,$FC,$FC,$FC,$00,$00,$00,$00,$00,$04,$04,$04,$04,$04,$00
    .byte $FC,$FC,$FC,$FC,$FC,$FC,$00,$00,$00,$03,$03,$03,$03,$03,$03,$00
    .byte $FC,$FC,$FC,$FC,$FC,$FD,$FE,$00,$01,$02,$03,$03,$03,$03,$03,$00
    .byte $FC,$FC,$FC,$FC,$FD,$FD,$FE,$00,$01,$02,$02,$03,$03,$03,$03,$00
    .byte $FC,$FC,$FC,$FD,$FD,$FE,$FF,$00,$00,$01,$02,$02,$03,$03,$03,$00
    .byte $FC,$FC,$FD,$FD,$FD,$FE,$FF,$00,$00,$01,$02,$02,$02,$03,$03,$00
    .byte $FC,$FD,$FD,$FD,$FE,$FE,$FF,$00,$00,$01,$01,$02,$02,$02,$03,$00
    .byte $FD,$FD,$FD,$FE,$FE,$FE,$FF,$00,$00,$01,$01,$01,$02,$02,$02,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_x_dir_lut_hi = x_dir_lut_hi
x_dir_lut_lo:
    .byte $2C,$66,$AD,$04,$6D,$E7,$6F,$00,$91,$19,$93,$FC,$53,$9A,$D4,$00
    .byte $F7,$2C,$70,$C8,$36,$BC,$58,$00,$A8,$44,$CA,$38,$90,$D4,$09,$00
    .byte $BF,$ED,$2C,$80,$F1,$84,$37,$00,$C9,$7C,$0F,$80,$D4,$13,$41,$00
    .byte $87,$AC,$E0,$2C,$9A,$36,$08,$00,$F8,$CA,$66,$D4,$20,$54,$79,$00
    .byte $53,$6C,$92,$CD,$2C,$C8,$BC,$00,$44,$38,$D4,$33,$6E,$94,$AD,$00
    .byte $27,$35,$49,$6C,$AC,$2C,$36,$00,$CA,$D4,$54,$94,$B7,$CB,$D9,$00
    .byte $0A,$0E,$14,$1F,$35,$6C,$00,$00,$00,$94,$CB,$E1,$EC,$F2,$F6,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    .byte $0A,$0E,$14,$1F,$35,$6C,$00,$00,$00,$94,$CB,$E1,$EC,$F2,$F6,$00
    .byte $27,$35,$49,$6C,$AC,$2C,$36,$00,$CA,$D4,$54,$94,$B7,$CB,$D9,$00
    .byte $53,$6C,$92,$CD,$2C,$C8,$BC,$00,$44,$38,$D4,$33,$6E,$94,$AD,$00
    .byte $87,$AC,$E0,$2C,$9A,$36,$08,$00,$F8,$CA,$66,$D4,$20,$54,$79,$00
    .byte $BF,$ED,$2C,$80,$F1,$84,$37,$00,$C9,$7C,$0F,$80,$D4,$13,$41,$00
    .byte $F7,$2C,$70,$C8,$36,$BC,$58,$00,$A8,$44,$CA,$38,$90,$D4,$09,$00
    .byte $2C,$66,$AD,$04,$6D,$E7,$6F,$00,$91,$19,$93,$FC,$53,$9A,$D4,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_x_dir_lut_lo = x_dir_lut_lo
; Y LUT ---------------------------------
y_dir_lut_hi:
    .byte $FD,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FD,$00
    .byte $FD,$FD,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FD,$FD,$00
    .byte $FD,$FD,$FD,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FD,$FD,$FD,$00
    .byte $FE,$FD,$FD,$FD,$FC,$FC,$FC,$FC,$FC,$FC,$FC,$FD,$FD,$FD,$FE,$00
    .byte $FE,$FE,$FD,$FD,$FD,$FC,$FC,$FC,$FC,$FC,$FD,$FD,$FD,$FE,$FE,$00
    .byte $FE,$FE,$FE,$FE,$FD,$FD,$FC,$00,$FC,$FD,$FD,$FE,$FE,$FE,$FE,$00
    .byte $FF,$FF,$FF,$FF,$FE,$FE,$00,$00,$00,$FE,$FE,$FF,$FF,$FF,$FF,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$00,$01,$01,$00,$00,$00,$01,$01,$00,$00,$00,$00,$00
    .byte $01,$01,$01,$01,$02,$02,$03,$00,$03,$02,$02,$01,$01,$01,$01,$00
    .byte $01,$01,$02,$02,$02,$03,$03,$04,$03,$03,$02,$02,$02,$01,$01,$00
    .byte $01,$02,$02,$02,$03,$03,$03,$04,$03,$03,$03,$02,$02,$02,$01,$00
    .byte $02,$02,$02,$03,$03,$03,$03,$04,$03,$03,$03,$03,$02,$02,$02,$00
    .byte $02,$02,$03,$03,$03,$03,$03,$04,$03,$03,$03,$03,$03,$02,$02,$00
    .byte $02,$03,$03,$03,$03,$03,$03,$04,$03,$03,$03,$03,$03,$03,$02,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
_y_dir_lut_hi = y_dir_lut_hi
y_dir_lut_lo:
    .byte $2C,$F7,$BF,$87,$53,$27,$0A,$00,$0A,$27,$53,$87,$BF,$F7,$2C,$00
    .byte $66,$2C,$ED,$AC,$6C,$35,$0E,$00,$0E,$35,$6C,$AC,$ED,$2C,$66,$00
    .byte $AD,$70,$2C,$E0,$92,$49,$14,$00,$14,$49,$92,$E0,$2C,$70,$AD,$00
    .byte $04,$C8,$80,$2C,$CD,$6C,$1F,$00,$1F,$6C,$CD,$2C,$80,$C8,$04,$00
    .byte $6D,$36,$F1,$9A,$2C,$AC,$35,$00,$35,$AC,$2C,$9A,$F1,$36,$6D,$00
    .byte $E7,$BC,$84,$36,$C8,$2C,$6C,$00,$6C,$2C,$C8,$36,$84,$BC,$E7,$00
    .byte $6F,$58,$37,$08,$BC,$36,$00,$00,$00,$36,$BC,$08,$37,$58,$6F,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    .byte $91,$A8,$C9,$F8,$44,$CA,$00,$00,$00,$CA,$44,$F8,$C9,$A8,$91,$00
    .byte $19,$44,$7C,$CA,$38,$D4,$94,$00,$94,$D4,$38,$CA,$7C,$44,$19,$00
    .byte $93,$CA,$0F,$66,$D4,$54,$CB,$00,$CB,$54,$D4,$66,$0F,$CA,$93,$00
    .byte $FC,$38,$80,$D4,$33,$94,$E1,$00,$E1,$94,$33,$D4,$80,$38,$FC,$00
    .byte $53,$90,$D4,$20,$6E,$B7,$EC,$00,$EC,$B7,$6E,$20,$D4,$90,$53,$00
    .byte $9A,$D4,$13,$54,$94,$CB,$F2,$00,$F2,$CB,$94,$54,$13,$D4,$9A,$00
    .byte $D4,$09,$41,$79,$AD,$D9,$F6,$00,$F6,$D9,$AD,$79,$41,$09,$D4,$00
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




