; displaylist.s
; attempthing to recreate  https://www.atarimagazines.com/vbook/displaylistssimplified.html

.setcpu "6502"
.include "atari.inc"


.export _init_dlist, _text_memory, _screen_memory, _main_dlist



; ------------------------------------------------------------
; SCREEN: we keep screen RAM in a loaded rw segment
; ------------------------------------------------------------
        .segment "SCREEN"
.align 1024

text_memory:
    .res( 2 * 40 )        
_text_memory = text_memory

screen_memory:
    .res( 4 * 22 * 40 )   ; 3520
_screen_memory = screen_memory

        .segment "DLIST"
.align 1024
;  just a single ANTIC 5 screen
_main_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $42                    ; LMS + sets ANTIC 4
        .word text_memory            ; memory address 2 bytes
        .byte $02                    ; total of 24 lines.

        .byte $44                    ; LMS + sets ANTIC 4
        .word screen_memory          ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 23
            .byte $04                ; total of 24 lines.
        .endrepeat

        .byte $41
        .word _main_dlist             ; JVB ( vertical blank jump to start of display list


; ------------------------------------------------------------
; CODE: init routine called from C
; ------------------------------------------------------------
        .segment "CODE"

.proc _init_dlist

    lda #0                                       ; stop the dma
    sta DMACTL
    lda #<_main_dlist                             ; install the new display list
    sta SDLSTL
    lda #>_main_dlist
    sta SDLSTH
    lda #$22                                     ; resume the DMA
    sta DMACTL

    rts


.endproc




