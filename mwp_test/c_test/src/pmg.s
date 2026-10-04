.setcpu "6502"
.include "atari.inc"

.export _pmgMem, _spr_0_frm_0



; ------------------------------------------------------------
; PMG: Player Missile Graphics
; ------------------------------------------------------------
    .segment "PMG"
_pmgMem: .res 2048

;; SPRITE DATA
;; frames, height, gap
;  .BYTE $01,$10,$00
;
;; SPRITE COLORS 0
;COL_0
;  .BYTE $ee
;; SPRITE COLORS 1
;COL_1
;  .BYTE $c8

; SPRITE 0
; FRAME 0
_spr_0_frm_0:
  .BYTE $3a, $3a, $39, $11, $39, $7d, $ff, $bb, $b9, $39, $39, $29, $2a, $28, $28, $6c



