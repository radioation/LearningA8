;
; fs1line_c.s - C bindings for fs1line.s
;

        .include "fs1line.inc"

        .exportzp _fs1_x1  := X1
        .exportzp _fs1_y1  := Y1
        .exportzp _fs1_x2  := X2
        .exportzp _fs1_y2  := Y2
        .exportzp _fs1_col := COLZ
        .exportzp _fs1_fb_hi := FRMBUFHI

        .export _fs1_plotl  := PLOTL    ; void fs1_plotl(void)
        .export _fs1_setcol := SETCOL   ; void fs1_setcol(unsigned char c): c arrives in A
        .export _fs1_clear_0  := CLEAR0   ; void fs1_clear_0(void)
        .export _fs1_clear_1  := CLEAR1   ; void fs1_clear_1(void)


