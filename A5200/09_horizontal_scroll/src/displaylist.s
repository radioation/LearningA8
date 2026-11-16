; displaylist.s
; attempthing to recreate  https://www.atarimagazines.com/vbook/displaylistssimplified.html

.setcpu "6502"
.include "atari5200.inc"


.export _rom_dlist, _ram_dlist, _init_dlist, _screen_memory, _coarse_scroll_right, _fine_scroll_right, _horz_scroll, _init_interrupts, _delay_count, _delay


    .segment "BSS"
.align 256
ram_dlist:
   .res 78
_ram_dlist = ram_dlist

horz_scroll:
    .res(1)    ; store HSCROLL value
_horz_scroll = horz_scroll 

delay_count:
    .res(1)   
_delay_count = delay_count


delay:
    .res 1  
_delay = delay





HORZ_SCROLL_MAX = 4  ; will vary depending on antic mode

; ------------------------------------------------------------
; SCREEN: we keep screen RAM in a loaded rw segment 
;  just a single ANTIC 5 screen
; ------------------------------------------------------------
        .segment "SCREEN"
.align 256
screen_memory:
    .res( 256*24)

_screen_memory = screen_memory

        .segment "DLIST"
rom_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $54                    ; LMS + sets ANTIC 4
        .word screen_memory          ; gives address of start of screen memory. ( DL and DL+1)

        .repeat 23, I
            .byte $54                ; total of 24 lines
            .word screen_memory + (I*256)          ; gives address of start of screen memory. ( DL and DL+1)
        .endrepeat

        .byte $41                    
        .word ram_dlist             ; JVB ( vertical blank jump to start of display list

_rom_dlist = rom_dlist
; 6 bytes + (23 * 3 bytes) + 3 bytes 
; 6 bytes + (69  bytes) + 3 bytes
;  78 bytes

; ------------------------------------------------------------
; CODE: init routine called from C
; ------------------------------------------------------------
        .segment "CODE"

.proc _init_dlist

    lda #0                                       ; stop DMA
    sta DMACTL
    lda #<ram_dlist                             ; install the new display list
    sta SDLSTL
    lda #>ram_dlist
    sta SDLSTH

    lda #$22                                     ; resume DAM
    sta DMACTL

    rts


.endproc


.proc _coarse_scroll_right
        ldy #10          ; 10 lines to modify
        ldx #34          ; 34th byte after start of display list is low byte of address 
cs_loop:
        inc ram_dlist,x
        inx             ; skip to next low byte which is 3 bytes away
        inx
        inx
        dey
        bne cs_loop
        rts

.endproc


; scroll one color clock right and check if at HSCROL limit
.proc _fine_scroll_right
        dec horz_scroll   ; decrease by 1
        lda horz_scroll
        bpl fs_done       ; if non-negative, still in the middle of the character

        jsr _coarse_scroll_right ; wrapped around to negative ($ff), do a coarse scroll...

        lda #HORZ_SCROLL_MAX-1  ;  reset the horizontal scrolling value 
        sta horz_scroll
fs_done:
        sta HSCROL      ; store vertical scroll value in hardware register
        rts
.endproc


.proc _init_interrupts
;;        lda #7                  ; Load A with 7 for Deferred VBI
;;        ldx #>vbi               ; Load X with the high byte of the handler's address
;;        ldy #<vbi               ; Load Y with the low byte of the handler's address
;;        jsr SETVBV              ; Call the OS routine to install the vector

   lda #$00              ; disable NMIs while updating VBI
   sta NMIEN
   lda #<vbi             ; set low byte of hander 
   sta VVBLKD            
   lda #>vbi            ; set high byte of hander 
   sta VVBLKD+1       


        lda #<dli               ; set address of the dli
        sta VDSLST
        lda #>dli
        sta VDSLST+1

   lda #$40              ; renable VBI and dli
   sta NMIEN 

    rts
.endproc

.proc vbi
        dec delay_count         ; wait a few VBLANKS before updating scrolling
        bne vbi_exit    

        jsr _fine_scroll_right  ; do the scroll
        
        lda delay
        sta delay_count
vbi_exit:
    ;     jmp XITVBV              ; NOT ON 5200!
    pla
    tay
    pla
    tax
    pla
    rti


.endproc

.proc dli
    pha              ; store Accumulator
    lda #$00         ; normal playfield
    sta WSYNC        ; trigger pause 
    ;sta COLBK       ; change background color
    sta COLPF3       ; change playfield 3
    lda #$0c         ; normal playfield
    sta WSYNC        ; Wait several lines befor setting to white
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    sta WSYNC       
    lda #$f4    ; normal playfield
    sta WSYNC   ; wait again
    sta WSYNC   ; 
    sta WSYNC   ; 
    sta WSYNC   ; 
    sta WSYNC   ; 
    sta WSYNC   ; 
    sta WSYNC   ; 
    sta WSYNC   ; 
    sta COLPF3  ; change playfield 3
    lda #$00
    sta WSYNC   ; trigger pause 
    sta WSYNC   ; trigger pause 
    sta WSYNC   ; trigger pause 
    sta WSYNC   ; trigger pause 
    sta COLBK   ; change background color

 
    pla         ; restore
    rti         ; exit DLI with RTI
.endproc




