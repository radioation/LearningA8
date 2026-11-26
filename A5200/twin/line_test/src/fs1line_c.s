;
; fs1line_c.s - C bindings for fs1line.s
;
; cc65 puts a leading underscore on every C name, so C's "fs1_x1"
; is "_fs1_x1" here. These lines give the asm symbols those names.
;

        .include "fs1line.inc"

        .exportzp _fs1_x1  := X1
        .exportzp _fs1_y1  := Y1
        .exportzp _fs1_x2  := X2
        .exportzp _fs1_y2  := Y2
        .exportzp _fs1_col := COLZ

        .export _fs1_plotl  := PLOTL    ; void fs1_plotl(void)
        .export _fs1_setcol := SETCOL   ; void fs1_setcol(unsigned char c): c arrives in A
        .export _fs1_init   := INICL    ; void fs1_init(void)
        .export _fs1_clear  := CLEAR    ; void fs1_clear(void)


