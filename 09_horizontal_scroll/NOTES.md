# Horizontal Scrolling
More complex than vertical scrolling as the DL needs to look at new areas 
of screen memory for each line. So each line gets an LMS bit ($40)
*IMP* add $40 to each antic mode you want involved in scrolling

From Player/Missile's [tutorial](https://www.playermissile.com/scrolling_tutorial/index.html) 
using 256 bytes of screen memory for each line simplifies LMS updates. Only the low
byteneeds to be modified for pure horizontal scrolling

```s
screen_memory:
    .res( 256*24)
```
and use LMS to tell each line where to look in RAM for the screen data

```s
main_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $44                    ; LMS + sets ANTIC 4
        .word screen_memory          ; gives address of start of screen memory. ( DL and DL+1)

        .repeat 23, I
            .byte $44                ; total of 12 lines
            .word screen_memory + (I*256)          ; gives address of start of screen memory. ( DL and DL+1)        .endrepeat

        .byte $41
        .word main_dlist             ; JVB ( vertical blank jump to start of display list

```
and this gets the screen looking right, but we still need to actually scroll things.

## Coarse scroll
from PM podcast, you can update each low byte

```s
.proc _coarse_scroll_right
        ;ldy #22         ; 22 lines to modify
        ;ldx #4          ; 4th byte after start of display list is low byte of address
        ldy #10         ; 7 lines to modify
        ldx #34          ; 34th byte after start of display list is LOW BYTE OF ADDRESS 
cs_loop:
        inc main_dlist,x
        inx             ; skip to next low byte which is 3 bytes away
        inx
        inx
        dey
        bne cs_loop
        rts

.endproc


```
### Fine scroll 
as with vertical scrolling we can use a hardware register to set 
fine scrolling increments
```s
   lda horz_scroll  
   sta HSCROLL
```


```s
; scroll one color clock right and check if at HSCROL limit
.proc _fine_scroll_right
        dec horz_scroll
        lda horz_scroll
        bpl fs_done       ; if non-negative, still in the middle of the character

        jsr _coarse_scroll_right ; wrapped to $ff, do a coarse scroll...
        lda #HORZ_SCROLL_MAX-1  ;  ...followed by reseting the HSCROL register
        sta horz_scroll
fs_done:
        sta HSCROL      ; store vertical scroll value in hardware register
        rts
.endproc
```
I used a VBI to update scrolling rather than trying to synchronize with it from the main loop

```s
.proc vbi
        dec delay_count  ; wait a few VBLANKS before updating scrolling
        bne vbi_exit    

        jsr _fine_scroll_right  ; do the scroll
        
        lda delay
        sta delay_count
vbi_exit:
        jmp XITVBV      ; alyways exit VBI through OS routiner


.endproc
```



