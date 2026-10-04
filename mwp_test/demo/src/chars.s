
.export _set_colors

; ------------------------------------------------------------
; CODE
; ------------------------------------------------------------
    .segment "CODE"

COL_BAK = $00   ; e.g., Black (Hue 0, Lum 0)
COL_PF0 = $76   
COL_PF1 = $1A  
COL_PF2 = $0E 
COL_PF3 = $B6
;  COL_PF0 = $EE   
;  COL_PF1 = $C8  
;  COL_PF2 = $FA 
;  COL_PF3 = $F2
;  
COLORBAK= $02C8  
COLOR0  = $02C4
COLOR1  = $02C5
COLOR2  = $02C6
COLOR3  = $02C7


.proc _set_colors
    lda #COL_BAK
    sta COLORBAK
    lda #COL_PF0
    sta COLOR0
    lda #COL_PF1
    sta COLOR1
    lda #COL_PF2
    sta COLOR2
    lda #COL_PF3
    sta COLOR3

    rts

.endproc
