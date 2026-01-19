.include "atari.inc"


.export _pmgMem, _spr_fr_0_frm_0, _spr_fr_0_frm_1, _spr_fr_0_frm_2, _spr_fr_1_frm_0, _spr_fr_1_frm_1, _spr_fr_1_frm_2, _spr_fr_2_frm_0, _spr_fr_2_frm_1, _spr_fr_2_frm_2, _spr_fr_3_frm_0, _spr_fr_3_frm_1, _spr_fr_3_frm_2, _spr_fl_0_frm_0, _spr_fl_0_frm_1, _spr_fl_0_frm_2, _spr_fl_1_frm_0, _spr_fl_1_frm_1, _spr_fl_1_frm_2, _spr_fl_2_frm_0, _spr_fl_2_frm_1, _spr_fl_2_frm_2, _spr_fl_3_frm_0, _spr_fl_3_frm_1, _spr_fl_3_frm_2


.SEGMENT "PMG"
_pmgMem: .res 2048


.segment "RODATA"

; sprite 0
; frame 0
_spr_fr_0_frm_0:
  .byte $00, $00, $01, $03, $03, $07, $07, $0f, $ff, $7f, $3f, $1f, $04, $00, $00, $00
; frame 1
_spr_fr_0_frm_1:
  .byte $00, $00, $00, $00, $01, $07, $0f, $ff, $7f, $3f, $1f, $04, $00, $00, $00, $00
; frame 2
_spr_fr_0_frm_2:
  .byte $00, $00, $00, $00, $00, $00, $0f, $ff, $7f, $3f, $1f, $07, $03, $01, $01, $00

; sprite 1
; frame 0
_spr_fr_1_frm_0:
  .byte $00, $00, $01, $03, $03, $07, $07, $07, $07, $47, $27, $1f, $0c, $18, $10, $00
; frame 1
_spr_fr_1_frm_1:
  .byte $00, $00, $00, $00, $00, $00, $04, $07, $47, $27, $1f, $0c, $18, $10, $00, $00
; frame 2
_spr_fr_1_frm_2:
  .byte $00, $00, $00, $00, $00, $00, $00, $00, $40, $20, $18, $0c, $18, $10, $01, $00

; sprite 2
; frame 0
_spr_fr_2_frm_0:
  .byte $80, $c0, $c0, $80, $80, $9c, $3c, $fc, $fc, $c0, $80, $00, $00, $00, $00, $00
; frame 1
_spr_fr_2_frm_1:
  .byte $00, $00, $00, $00, $1c, $bc, $fc, $fc, $c0, $80, $00, $00, $00, $00, $00, $00
; frame 2
_spr_fr_2_frm_2:
  .byte $00, $00, $00, $00, $1c, $3c, $fc, $fc, $c0, $80, $00, $80, $c0, $c0, $c0, $c0

; sprite 3
; frame 0
_spr_fr_3_frm_0:
  .byte $00, $00, $80, $80, $00, $0c, $0a, $03, $20, $00, $80, $00, $00, $00, $00, $00
; frame 1
_spr_fr_3_frm_1:
  .byte $00, $00, $00, $00, $0c, $0a, $03, $20, $00, $80, $00, $00, $00, $00, $00, $00
; frame 2
_spr_fr_3_frm_2:
  .byte $00, $00, $00, $00, $0c, $0a, $03, $20, $00, $80, $00, $00, $00, $00, $00, $80



; sprite 0
; frame 0
_spr_fl_0_frm_0:
  .byte $01, $03, $03, $01, $01, $39, $3c, $3f, $3f, $03, $01, $00, $00, $00, $00, $00
; frame 1
_spr_fl_0_frm_1:
  .byte $00, $00, $00, $00, $38, $3d, $3f, $3f, $03, $01, $00, $00, $00, $00, $00, $00
; frame 2
_spr_fl_0_frm_2:
  .byte $00, $00, $00, $00, $38, $3c, $3f, $3f, $03, $01, $00, $01, $03, $03, $03, $03

; sprite 1
; frame 0
_spr_fl_1_frm_0:
  .byte $00, $00, $01, $01, $00, $30, $50, $c0, $04, $00, $01, $00, $00, $00, $00, $00
; frame 1
_spr_fl_1_frm_1:
  .byte $00, $00, $00, $00, $30, $50, $c0, $04, $00, $01, $00, $00, $00, $00, $00, $00
; frame 2
_spr_fl_1_frm_2:
  .byte $00, $00, $00, $00, $30, $50, $c0, $04, $00, $01, $00, $00, $00, $00, $00, $01

; sprite 2
; frame 0
_spr_fl_2_frm_0:
  .byte $00, $00, $80, $c0, $c0, $e0, $e0, $f0, $ff, $fe, $fc, $f8, $20, $00, $00, $00
; frame 1
_spr_fl_2_frm_1:
  .byte $00, $00, $00, $00, $80, $e0, $f0, $ff, $fe, $fc, $f8, $20, $00, $00, $00, $00
; frame 2
_spr_fl_2_frm_2:
  .byte $00, $00, $00, $00, $00, $00, $f0, $ff, $fe, $fc, $f8, $e0, $c0, $80, $80, $00

; sprite 3
; frame 0
_spr_fl_3_frm_0:
  .byte $00, $00, $80, $c0, $c0, $e0, $e0, $e0, $e0, $e2, $e4, $f8, $30, $18, $08, $00
; frame 1
_spr_fl_3_frm_1:
  .byte $00, $00, $00, $00, $00, $00, $20, $e0, $e2, $e4, $f8, $30, $18, $08, $00, $00
; frame 2
_spr_fl_3_frm_2:
  .byte $00, $00, $00, $00, $00, $00, $00, $00, $02, $04, $18, $30, $18, $08, $80, $00
