        .setcpu "6502"


.export _init_keypad, _last_key


POKMSK  = $00
VKEYPD  = $020A

CONSOL  = $C01F
IRQEN   = $E80E
SKCTL   = $E80F

.segment "BSS"
last_key: .res 1       ; $FF = no key, $00-$0E = decoded key
_last_key = last_key



; ------------------------------------------------------------
; CODE: init routine called from C
; ------------------------------------------------------------
.segment "CODE"

.proc _init_keypad:
    sei          ; disable 

    lda #$FF     ; default to no key pressed 
    sta _last_key


    ; Setup our handler:  $020A
    lda #<keypad_isr
    sta VKEYPD
    lda #>keypad_isr
    sta VKEYPD+1

    lda #$04       ; Select Controller Port 0 at start.
    sta CONSOL

    lda #$02      ; Enable keyboard scan WITHOUT debounce ($02)
    sta SKCTL

    lda POKMSK     ; Enable Keyboard IRQ ($40) in POKMSK shadow 
    ora #$40
    sta POKMSK
    sta IRQEN     ; enable in POKEY

    cli           ; must be unmaks.ed


    rts

; BIOS jumps here via ($020A)  pushing A, X, Y and decoding key into A
keypad_isr:
    sta _last_key       ; Save decoded key ($00-$0E)
    pla                 ; pop out  Y, X, A 
    tay
    pla
    tax
    pla
    rti

