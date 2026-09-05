; displaylist.s
; attempthing to recreate  https://www.atarimagazines.com/vbook/displaylistssimplified.html

.setcpu "6502"
.include "atari.inc"


.export _init_dlist, _screen_memory



; ------------------------------------------------------------
; SCREEN: we keep screen RAM in a loaded rw segment
; ------------------------------------------------------------
        .segment "SCREEN"
.align 1024

screen_memory:
    .res( 40*24)
_screen_memory = screen_memory

        .segment "DLIST"
.align 1024
;  just a single ANTIC 5 screen
main_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $42                    ; LMS + sets ANTIC 2
        .word screen_memory          ; gives address of start of screen memory. ( DL and DL+1)

        .byte $02                    ; second GR.0 line
        .repeat 22
            .byte $04                ; total of 24 lines.
        .endrepeat

        .byte $41
        .word main_dlist             ; JVB ( vertical blank jump to start of display list


; ------------------------------------------------------------
; CODE: init routine called from C
; ------------------------------------------------------------
        .segment "CODE"

.proc _init_dlist

    lda #0                                       ; stop the dma
    sta DMACTL
    lda #<main_dlist                             ; install the new display list
    sta SDLSTL
    lda #>main_dlist
    sta SDLSTH
    lda #$22                                     ; resume the DMA
    sta DMACTL

    rts


.endproc




