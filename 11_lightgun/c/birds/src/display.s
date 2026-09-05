; displaylist.s

.setcpu "6502"
.include "atari.inc"

.export _init_dlist, _init_interrupts, _x0, _x1, _x2, _x3
.import _img_data, _img_data2,  _text_memory


; ------------------------------------------------------------
; DLIST: 
; ------------------------------------------------------------
        .segment "DLIST"
.align 1024

;  a couple of GR.0 lines followed by Antic 4
main_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $42                    ; LMS + sets ANTIC 2 (GR.0)
        .word _text_memory            ; memory address 2 bytes
        .byte $02                    ; second line of ANTIC 2  ( 16 scanlines at this point )

        .byte $4e                    ; Second  LMS + sets ANTIC mode E
        .word _img_data          ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 99  
            .byte $0e               
        .endrepeat

        .byte $4e                 ; #rd LMS and address for 4K boundary
        .word _img_data2          ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 75 
            .byte $0e               
        .endrepeat

        
        .byte $41
        .word main_dlist             ; JVB ( vertical blank jump to start of display list


; ------------------------------------------------------------
; CODE: init routine called from C
; ------------------------------------------------------------

    .segment "BSS"
x0:
  .res 1
_x0 = x0
x1:
  .res 1
_x1 = x1
x2:
  .res 1
_x2 = x2
x3:
  .res 1
_x3 = x3


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


.proc _init_interrupts

   ; setup VBI
   lda #7              ; Load A with 7 for Deferred VBI
   ldx #>vbi           ; Load X with the high byte of the handler's address
   ldy #<vbi           ; Load Y with the low byte of the handler's address
   jsr SETVBV          ; Call the OS routine to install the vector

   lda #$F0                   ; SET INTERRUPT $80 and last  blank $70
   sta main_dlist + 2         ; move to the line we want the DLI to fire

   lda #$8e                   ; SET INTERRUPT $80 and last moed e $0e
   sta main_dlist + 32        ; move to the line we want the DLI to fire

   lda #$8e                   ; SET INTERRUPT $80 and last moed e $0e
   sta main_dlist + 74        ; move to the line we want the DLI to fire

   lda #$8e                   ; SET INTERRUPT $80 and last moed e $0e
   sta main_dlist + 116       ; move to the line we want the DLI to fire

   lda #<dli_1                 ; set address of the dli
   sta VDSLST
   lda #>dli_1
   sta VDSLST+1
   rts
.endproc


.proc dli_1
  pha           ; save A to the stack

  lda #$98      ; setup color
  sta WSYNC     
  sta COLPF2    ; change GR.0 color


  lda x0
  sta HPOSP0
  sta HPOSP1
  adc #15
  sta HPOSP2
  sta HPOSP3
 
   lda #<dli_2                 ; set address of the next dli
   sta VDSLST
   lda #>dli_2
   sta VDSLST+1

  pla    ; restore A
  rti
.endproc



.proc dli_2
  ; back up the registers you're using
  pha           ; save A
  txa           ; get X 
  pha           ; save X 

  lda x1
  sta WSYNC
  sta HPOSP0
  sta HPOSP1
  adc #15
  sta HPOSP2
  sta HPOSP3
 
   lda #<dli_3                 ; set address of the next dli
   sta VDSLST
   lda #>dli_3
   sta VDSLST+1

  ; Restore the registeres you used.
  pla            ; restore A and X
  tax
  pla
  rti

.endproc

.proc dli_3
  ; back up the registers you're using
  pha           ; save A
  txa           ; get X 
  pha           ; save X 

  lda x2
  sta WSYNC
  sta HPOSP0
  sta HPOSP1
  adc #15
  sta HPOSP2
  sta HPOSP3
 
   lda #<dli_4                 ; set address of the next dli
   sta VDSLST
   lda #>dli_4
   sta VDSLST+1

  ; Restore the registeres you used.
  pla            ; restore A and X
  tax
  pla
  rti

.endproc


.proc dli_4
  ; back up the registers you're using
  pha           ; save A
  txa           ; get X 
  pha           ; save X 

  lda x3
  sta WSYNC
  sta HPOSP0
  sta HPOSP1
  adc #15
  sta HPOSP2
  sta HPOSP3
 

  ; Restore the registeres you used.
  pla            ; restore A and X
  tax
  pla
  rti

.endproc


.proc vbi
;        ;  x0 += 1; if (x0 > 213) x0 = 25; ---
;        inc     x0                  ; x0++
;        lda     x0
;        cmp     #214                ; compare to 214 (i.e., test x0 >= 214)
;        bcc     :+                  ; if x0 < 214 don't wrap (branch to next)
;        lda     #25
;        sta     x0
;
;:       ;  x1 -= 1; if (x1 < 25) x1 = 213; ---
;        lda     x1
;        sec                         ; prepare borrow-less subtraction
;        sbc     #1                  ; x1 = x1 - 1
;        sta     x1
;        cmp     #25                 ; compare new x1 to 25
;        bcs     :+                  ; if x1 >= 25, keep it
;        lda     #213
;        sta     x1
;:       ;  x2 
;        inc     x2
;        lda     x2
;        cmp     #214
;        bcc     :+
;        lda     #25
;        sta     x2
;:       ;  x3
;        lda     x3
;        sec
;        sbc     #1
;        sta     x3
;        cmp     #25
;        bcs     :+
;        lda     #213
;        sta     x3


;:
  lda #<dli_1                 ; set address of the first dli
  sta VDSLST
  lda #>dli_1
  sta VDSLST+1
  jmp XITVBV
.endproc
