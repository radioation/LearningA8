; displaylist.s

.setcpu "6502"
.include "atari.inc"


.export _init_dlist, _sline0, _screen_memory, _message_memory, _horz_scroll, _init_interrupts, _delay_count, _delay, _tdir, _mwpy, _mwpx, _finex, _finey, _main_dlist, _lms1


    .segment "BSS"
horz_scroll:
    .res(1)    ; store HSCROLL value
_horz_scroll = horz_scroll 

delay_count:
    .res(1)   
_delay_count = delay_count


delay:
    .res 1  
_delay = delay


tdir:
  .res(1)
_tdir = tdir
; X position 0-19
mwpy:
  .res(1)
_mwpy = mwpy

; Y position  0-19
mwpx:
  .res(1)
_mwpx = mwpx
finex:
  .res(1)
_finex = finex
finey:
  .res(1)
_finey = finey




; ------------------------------------------------------------
; SCREEN: we keep screen RAM in a loaded rw segment 
;  just a single ANTIC 5 screen
; ------------------------------------------------------------
        .segment "SCREEN"
.align 256

; 20 + 1 extra line to be offscreen
; screen line 0
sline0:
   .res( 48 ) 
_sline0 = sline0
screen_memory:
    .res( 20 * 48)
_screen_memory = screen_memory


; reserve 4 lines for GR.0/ANTIC2
message_memory:
   .res( 4*40)    
_message_memory = message_memory



main_dlist:
        .byte $70, $70, $70        ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"
        .byte 64+4                 ; LMS (64) + ANTIC4 (not scrolling)
        .word message_memory       ;
        .byte $04, $04, $04        ; 3 more ANTIC4 non-scrolling lines ( 4 lines )

lms1:   .byte 64 + $34             ; LMS  64 +  HSCROLL + VSCRLL + ANTIC 4 line
        .word screen_memory        ; gives address of start of screen memory. ( DL and DL+1)

        .repeat 18, I
            .byte $34              ; total of 24 lines
        .endrepeat
        
        .byte 64 + $14             ; LMS  64 +  HSCROLL + VSCRLL + ANTIC 4 line
        .word screen_memory        ; gives address of start of screen memory. ( DL and DL+1)

        .byte $41                    
        .word main_dlist             ; JVB ( vertical blank jump to start of display list

_main_dlist = main_dlist
_lms1 =lms1 

; ------------------------------------------------------------
; CODE: init routine called from C
; ------------------------------------------------------------
        .segment "CODE"

.proc _init_dlist

    lda #0                                       ; stop DMA
    sta DMACTL
    lda #<main_dlist                             ; install the new display list
    sta SDLSTL
    lda #>main_dlist
    sta SDLSTH

    lda #$22                                     ; resume DAM
    sta DMACTL

    rts


.endproc


.proc _init_interrupts
        lda #7                  ; Load A with 7 for Deferred VBI
        ldx #>vbi               ; Load X with the high byte of the handler's address
        ldy #<vbi               ; Load Y with the low byte of the handler's address
        jsr SETVBV              ; Call the OS routine to install the vector



    rts
.endproc

.proc vbi
    ; read stick
    lda STICK0
   
    and #$02    ;down bit ( 1101 in https://atariwiki.org/wiki/Wiki.jsp?page=STICK0 )
    bne check_up    ; not DOWN bit, so check if stick is pushed UP
  
    lda finey       ; current fine scroll pos in y direction
    bne stick_down  ; have we fine scrolled down to 0? If not no need for MWP
    lda #8          ; reset fine scroll y pos
    sta finey       ; store it
    lda #0          ; use accumulator to pass dir to MWP ( comments says '0')
    jsr mwp         ; Do MWP update to screen memory
stick_down:
    dec finey   ; decrement fine scrolling position for y direction


check_up:
    lda STICK0
    and #$01    ; up bit ( 1110)
    bne check_right ; not UP bit, so check if stick is pushed RIGHT
  
    lda finey       ; current fine scroll pos in y dir
    cmp #7          ; have we fine scrolled 8 lines (0-7)
    bne stick_up    ; no, don't call MWP
    lda #$ff        ; 255? so that next inc will roll over to 0?
    sta finey
    lda #1          ; use accumulator to pass direction to MWP ( comments says '1')
    jsr mwp         ; Do MWP update to screen memory.
stick_up:
    inc finey   ; increment file scrolling position for y direction


check_right:
    lda STICK0
    and #$08    ; right bit ( 0111  )
    bne check_left  ; not RIGHT bit, so check if stick is pushed LEFT
  
    lda finex       ; current fine scroll position in X direction
    cmp #7          ; have we fine scrolled 8 cols (0-7)
    bne stick_right ; NO, don't call MWP
    lda #$ff        ; 255, so next increment will roll over to 0?
    sta finex
    lda #3          ; use accumulator to pass direction to MWP ( comments says '2 - right'?)
    jsr mwp         ; Do MWP update to screen memory
stick_right:
    inc finex   ; increment fine scrolling  pos for x dir


check_left:
    lda STICK0
     and #$04    ;left bit (1011 )
     bne vdone       ; nope, no more directions to try so we're done
   
           lda finex   ; current fine scroll position in X direction
     bne stick_left  ; have we fine-scrolled left to 0? if not no need for mwp call
     lda #8          ; reset fine scroll X position
     sta finex
     lda #2           ; use accumulator to pass direction to MWP (comments says '3 - left'?)
     jsr mwp          ; do MWP udpate to screen memory
stick_left:
    dec finex  ;decrement fine scrolling pos for y dir

vdone:
    lda finex   ; update fine scroll registers
    sta $d404
    lda finey
    sta $d405
    jmp XITVBV         ; always exit VBI through OS routine.


.endproc




.proc mwp
        ldy mwpy     ; current MWP Y position? original comments say between 0-21
        sta tdir     ; Accumulator set by VBLANK call to mwp. Sets direction
        cmp #2         ; right left starts with 2, so if we're

        bcc mwp_erase  ; less than 2 we're moving  up/down (wi arae)
        beq mwp_left   ; if equal to 2, go to MWP left

; Note: When HSCROLL bit is set in the DLIST, for 40 column 
; width screen there are 48 characters per row instead of 40!
;
mwp_right:
        lda mwpx   ; current MWP X pos? Original comments says between 0-48
        bne mwp_r1 ; Branch not equal (if zer0 flag is clear, branch and skip the LMS erase call)

        lda #48    ;  We're HERE because mwpx == zero, so wrap around to
                   ;  48 ( and will decremnt, so effectively 47 )
        sta mwpx   ; save it
        lda #0     ; sets dir to down
        sta tdir   ; LMS? tdir tells it which way to go (0 down)
        beq mwp_erase  ; #0 also forces beq to branch (skip left processing and decrement for this iteration)(

mwp_r1:
        dec mwpx   ; decrease current MWP X pos
        lda #0
        jmp mwp_lms ; if we made it here, do MWP update without changing location of old LMS2 in DL

mwp_left:
        lda mwpx  ; current MWP X position (between 0 and 23 ).
        cmp #48   ; EDGE is 48,
        beq mwp_l1 ; are we equal to 48 (at the edge)? If so we want to wrap around to 0 and need to update LMS2

        inc mwpx    ; less than 48, increment.
        jmp mwp_lms ; if we made it here, do MWP update without changing location of old LMS2 in DL

mwp_l1:  
        lda #0     ; we were at the edge, so set mwpx to 0
        sta mwpx
        lda #1      ; tdir tells which way to go (1 up )
        sta tdir

;-- BEGIN ERASE LMS2/LMSBottom ---------------------------------------------
; *IMP* we must overwrite 3 bytes, the LMS instruction and the two address bytes following it

mwp_erase:
                           ;  basically copy $34 on top of all 3 to eliminate LMS bit and overwrite addresses.
     ; -- begin actual erasure ----------------------------
        lda mwp_idx,y     ; Get offset of LMS2/LMS-bottom in display list for current Y?
        tax                ; copy position to X

        lda #$34           ; erase old LMS by removing '64' from entry ( bits 4 and 5 ($30) are HSCROLL AND VSCROLL. $04 is GR1/Antic 4 )
        sta lms1+3,x       ;  lms1 is location of top LMS in DLIST. LMS+3/4/5 + X is the LMS2/LMS-bottom's current location.
                           ;  clear 2nd LMS:   lms1 *plus* hard coded 3, plus X ( +3 is a second lms)
        sta lms1+4,x       ;  clear LMS2/LMS-bottom address byte low: lms1  *plus* hard coded 4, plus X ( +4 isl-byte of 2nd lms)


        cpx #18            ; is BOTTOM LS at 18
        bne mwp_e1         ; LMS2 is Not at the bottom, SKIP the next line because we want to keep VSCROLL for most lines

        lda #$14           ; a bit annoying, but last line of scroll map
                           ; should not have vscroll set; this prevents
                           ; 'popping'  (gets rid of VSCROLL bit but preseves HSCROLL)

mwp_e1:
        sta lms1+5,x       ; clear LMS2/LMS-bottom  address byte hight: lms1 *plus* hard coded 5, plus X ( +5 is h-byte of 2nd lms) may or may not preserve VSCROLL.
     ; -- end actual erasure ----------------------------


        lda tdir           ; wer we moving right/left determines which way to move MWP

        beq mwp_down       ; if tdir 0 move down
        cmp #1
        bne mwp_lms        ;if tdir not 1, skip out

mwp_up:
        iny                ; Y has mwpy (stored at start of `mwp` routine), increment it
        cpy #20            ; compare Y with #20 OR "are we past last line?"
        bne mwp_lms        ; no, skip (current y becomes new mwpy)
        ldy #0             ; set Y to 0
        beq mwp_lms        ;  ( y is now 0 and  becomes new  mwpy )

mwp_down:
        dey                ; Y has mwpy, decrement it.
        bpl mwp_lms        ; still positive? Y becomes new mwpy
        ldy #19            ; set Y to 19 (19 becomes new mwpy)
;-- END ERASE LMS2/LMSBottom ---------------------------------------------


;-------------------
mwp_lms:
        sty mwpy        ; copy  current Y to mwpy (at this point we've gotten past mwp_erase and may have had an iny or dey to calc new Y position)


        ; --- update lower LMS ----------------------------
        lda mwp_idx,y   ; Get offset of LMS2/LMS-bottom in display list for current MWPY?
        tax             ; Store DLIST index in X
        lda mwp_botlo,y ; read LMS2/LMS-bottom low-byte for current MWPY in accumulator? ( will be $30/48, 0 (mostly), or $18/24  )
        clc             ; clear to get ready for add
        adc mwpx        ; botlo can be $0 or $30 (dec48) largest mwpx 48 so 96 max (carry should noet set)
        sta lms1+4,x    ;  SET  LMS2/LMS-bottom address byte low: lms1  *plus* hard coded 4, plus X ( +4 isl-byte of 2nd lms), mwpx lets LMS2/LMS-BOTTOM
                        ; point at different columns in a given row of screen memory.

        lda #>sline0    ; high byte of screen memory (currently defined as $4400. sline0 is top line, screen is second line of reserved memory
        adc #0          ; add curent state of carry flag to accumulator?
        sta lms1+5,x    ;  Set  LMS2/LMS-bottom High byte to sline0 (+ carry if carry actually occured, even though comment says overflow won't happen)


        ; --- update top LMS ----------------------------
        lda mwp_toplo+1,y   ; now, lookup top low byte lms and store in A.
                            ; mwp_toplo is multiples of 24 (0,24,48,72,96, etc) so a row per Y
;       clc                 ; carry cleared above, no need to clc here ad 48+48 won't carry
        adc mwpx           ;   mwpx is 0-48 but could overflow ( mwp_toplo has $F0 )
        sta lms1+1         ; set Lowbyte of LMS1

        lda mwp_tophi+1,y  ; lookup high byte for LMS1 and store in A
        adc #0          ; add curent state of carry flag to accumulator
        sta lms1+2        ; set highbyte of LMS1

        lda #64+$34         ; actual instruction for lower LMS
        cpx #18             ; see note above, Y 18 doesn't want VSCROLL
        bne mwp_lms2        ; skip if not 18
        lda #64+$14         ; use #64+$14 instead of #64+$34 to get rid of VSCROL but keep HSCROL
mwp_lms2:
        sta lms1+3,x        ; set  LMS2/LMS-bottom's LMS flag. ( recall tax stored DLIST Index in X register )
        rts
 
.endproc




    .segment "RODATA"

;==================================================================
; Look-up tables for MWP

; TOP LMS low byte is multiples of 48 (ANTIC4 is 40 + 16 color cloks = 48 bytes)
mwp_toplo:
    .repeat 20, I
        .byte < ( sline0 + ( I* 48 ))
    .endrepeat
    .byte $00

; TOP LMS high byte
mwp_tophi:
    .repeat 20, I
        .byte > ( sline0 + ( I* 48 ))
    .endrepeat
    .byte > ( sline0 )

; ENTRY/INDEX-INTO DISPLAY LIST for bottom LMS  by Y? if Y =1, the row we want to wrap is the bottom (18)
mwp_idx:
; relative to top/LMS which we don't move from that position.
       .byte 18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0

; screen memory offset for bottom LMS, only matters once. COuld we turn off 
mwp_botlo:
       .byte $00,$00,$00,$00,$00,$00
       .byte $00,$00,$00,$00,$00,$00
       .byte $00,$00,$00,$00,$00,$00,$00,$30




