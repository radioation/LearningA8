; displaylist.s
; attempthing to recreate  https://www.atarimagazines.com/vbook/displaylistssimplified.html

.setcpu "6502"
.include "atari.inc"

;.import popa      ; pops 1 byte from cc65 param stack

.export _init_dlist, _screen_ram


; ------------------------------------------------------------
; ZEROPAGE: we keep screen RAM in a loaded rw segment 
; ------------------------------------------------------------
;        .segment "ZEROPAGE"
;textptr:
;    .res 2 ; reserve 2 bytes for the 16-bit char array ptr
;textoff: 
;    .res 1

; ------------------------------------------------------------
; SCREEN: we keep screen RAM in a loaded rw segment 
;
;  * two lines of Graphics mode 2 (ANTIC 7)
;  * 128 lines of Graphics mode 8  (will cross 4096 boundary)
;  * four lines of Graphics mode 1
;
; ------------------------------------------------------------
        .segment "SCREEN"
.align 256
screen_ram:
        .res (80 * 40 ) + ( 4* 20)              ; 40bytes per line ANTICD.  20 Bytes ANTIC6


_screen_ram =screen_ram 


; ------------------------------------------------------------
; DLIST: display list bytes live here directly (no copy)
; 
; ------------------------------------------------------------
        .segment "DLIST"
.align $0100

main_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $4D                    ; LMS + sets ANTIC D   (2 scanlines)
        .word screen_ram             ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 79 
            .byte $0D                ; rest of ANTIC D lines ( 2 * 79 scanlines) total 159 + 2 = 160 scanlines
        .endrepeat


        .byte $6, $6, $6, $6         ; 4 lines of GR.1 / ANTIC 6 (32 scanlines)

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
