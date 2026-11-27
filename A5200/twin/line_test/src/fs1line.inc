; fs1line.inc - include in any module that draws lines
;
; Usage:
;   lda #x1 / sta X1, etc. for X1 Y1 X2 Y2 (x 0..159, y 0..191)
;   lda #color (0..3) / jsr SETCOL   (or store $00/$55/$AA/$FF in COLZ)
;   jsr PLOTL
; PLOTL may swap the endpoints, so reload all four before every call.
; PLOTL trashes A, X, Y.

BMP = $2000             ; bitmap address

.globalzp X1, Y1, X2, Y2, COLZ
.global   PLOTL, SETCOL, INICL, CLEAR0, CLEAR1
