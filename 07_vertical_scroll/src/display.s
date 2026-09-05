; displaylist.s
; attempthing to recreate  https://www.atarimagazines.com/vbook/displaylistssimplified.html

.setcpu "6502"
.include "atari.inc"


.export _init_dlist, _text_memory, _screen_memory, _dlist_scroll_address



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

;  a couple of GR.0 lines followed by Antic 4
main_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $42                    ; LMS + sets ANTIC 2
        .word text_memory            ; memory address 2 bytes
        .byte $02                    ; second line of antic 2

        .byte $64                    ; LMS + sets ANTIC 4
_dlist_scroll_address:               ; LABEL THE ADDRESS OF SCREEN MEMORY SO WE CAN UPDATE IT LATER.
        .word screen_memory          ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 23
            .byte $24                ; total of 24 lines.
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




