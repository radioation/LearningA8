        .setcpu "6502"
        .include "atari.inc"

.export _init_interrupts, _snd_cmd, _snd_addr_lo, _snd_addr_hi, _snd_tick_count, _up_snd, _down_snd, vbi, _dummeh

; ------------------------------------------------------------
; ZEROPAGE: control bytes
; ------------------------------------------------------------
        .segment "ZEROPAGE"


_snd_cmd:         .res   1   ; 0 = idle, 1 = new sound request, 2 = playing
; _snd_addr:        .res 2      ; 16-bit address
_snd_addr_lo:     .res   1
_snd_addr_hi:     .res   1
_snd_tick_count:  .res   1   ;
_dummeh:          .res  1

; --- VBI-private working vars (only VBI writes these) ---
;cur_snd_addr:     .res 2      ; copy of snd address
cur_snd_addr_lo:     .res 1      ; copy of snd address
cur_snd_addr_hi:     .res 1      ; copy of snd address
cur_snd_ticks:    .res   1
cur_snd_tick_count:    .res   1

; ------------------------------------------------------------
; RODATA: sound bytes
; ------------------------------------------------------------
        .segment "RODATA"
; put = join( range(150,30,-7), ',')
_up_snd: 
    .byte  150,139,128,117,106,95,84,73,62,51,40, 30, 0, 0

_down_snd: 
    .byte  30,41,52,63,74,85,96,107,118,129,140,150, 0, 0

; ------------------------------------------------------------
; CODE
; ------------------------------------------------------------
        .segment "CODE"

.proc vbi
    ; if not playing, lewts exit
    lda _snd_cmd
    beq @done            ; 0, get out

    cmp #1       
    bne @playing   ; not equal to one so not new, skip to @playing
   
 
    ; new sound found, so load the address
    lda _snd_addr_lo
    sta cur_snd_addr_lo  
    lda _snd_addr_hi
    sta cur_snd_addr_hi


    ldy #0
    lda  (cur_snd_addr_lo), y  ; get current note/frequency
    sta _dummeh                  ; set the sound register.

    lda _snd_tick_count  ; how many ticks
    sta cur_snd_ticks 
    sta cur_snd_tick_count  

    lda #2
    sta _snd_cmd         ; we're now playing a sound.

    lda #$A8             ; smooth tone, mid voluime
    sta AUDC1

@playing:
    ldy #0
    lda  (cur_snd_addr_lo), y  ; get current note/frequency
    sta AUDF1                  ; set the sound register.


    dec cur_snd_ticks       ; cut down the tick count.
    bmi @next_note          ; if BRANCH MINUS/negative get a new note.
     

 

    jmp @done
@next_note:
    lda cur_snd_addr_lo
    adc #1              ; move up
    sta cur_snd_addr_lo
    lda cur_snd_addr_hi
    adc #0               ; add carry bit only
    sta cur_snd_addr_hi

    lda cur_snd_tick_count
    sta cur_snd_ticks

    ldy #0
    lda (cur_snd_addr_lo), y
    beq @stop_playing


@done:
    jmp XITVBV      ; alyways exit VBI through OS routiner


@stop_playing:
    lda #0
    sta AUDF1 
    lda #0
    sta _snd_cmd
    jmp XITVBV      ; alyways exit VBI through OS routiner

.endproc


.proc _init_interrupts
        lda #7              ; Load A with 7 for Deferred VBI
        ldx #>vbi  ; Load X with the high byte of the handler's address
        ldy #<vbi  ; Load Y with the low byte of the handler's address
        jsr SETVBV          ; Call the OS routine to install the vector
        rts
.endproc




