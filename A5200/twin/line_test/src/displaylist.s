; displaylist.s


.setcpu "6502"
.include "atari5200.inc"

;.import popa      ; pops 1 byte from cc65 param stack

.export _init_dlist


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
; assuming all ANTIC D gets 3840 (but I will beusing some
; ANTIC 6 at tyhe bottom, just using 3840 in case I go full screen
; ------------------------------------------------------------
; HARDCODED AT $2000 and $3000

; ------------------------------------------------------------
; DLIST: display list bytes live here directly (no copy)
; 
; ------------------------------------------------------------
        .segment "DLIST"
;.align $0100

_dlist_0:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $4D                    ; LMS + sets ANTIC D   (2 scanlines)
        .word $2000                  ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 79 
        ;.repeat 96 
            .byte $0D                ; rest of ANTIC D lines ( 2 * 79 scanlines) total 159 + 2 = 160 scanlines
        .endrepeat


        .byte $6, $6, $6, $6         ; 4 lines of GR.1 / ANTIC 6 (32 scanlines)

        .byte $41                    
        .word _dlist_0             ; JVB ( vertical blank jump to start of display list


_dlist_1:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $4D                    ; LMS + sets ANTIC D   (2 scanlines)
        .word $3000                  ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 79 
            .byte $0D                ; rest of ANTIC D lines ( 2 * 79 scanlines) total 159 + 2 = 160 scanlines
        .endrepeat


        .byte $6, $6, $6, $6         ; 4 lines of GR.1 / ANTIC 6 (32 scanlines)

        .byte $41                    
        .word _dlist_1             ; JVB ( vertical blank jump to start of display list


; ------------------------------------------------------------
; CODE: init routine called from C
; ------------------------------------------------------------
        .segment "CODE"

.proc _init_dlist

    lda #0                                       ; stop the dma
    sta DMACTL
    lda #<_dlist_0                             ; install the new display list
    sta SDLSTL
    lda #>_dlist_0
    sta SDLSTH
    lda #$22                                     ; resume the DMA
    sta DMACTL

    rts


.endproc
