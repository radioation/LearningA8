;

	;
	; Line draw routines from SubLogic A2-FS1
	; 1979, Bruce Artwick
	; Atari 800 port, 2023, Claus Buchholz
	;

; ca65 port
; v2: row tables, color support, color-1 wrap fix
;
; *IMP*  Plots with ORA ad assumes a cleared screen. Overlapping colors
; will combine AND not erase (1 | 2 = 3). 
;Btmap is at $A010: boot with BASIC disabled.
;

        .include "fs1line.inc"

;----------------------------------------------------------------
        .segment "ZEROPAGE"

X1:     .res 1          ; STArt point, x, 0 to 159
Y1:     .res 1          ; STArt point, y, 0 to 191
X2:     .res 1          ; end point, x, 0 to 159
Y2:     .res 1          ; end point, y, 0 to 191
COLZ:   .res 1          ; color fill byte: $00, $55, $AA or $FF

SPZ:    .res 2          ; screen pointer

BPZ:    .res 1          ; bit pattern (mask only, no color)
ACZ:    .res 1          ; accumulator (Bresenham error term)
XLZ:    .res 1          ; x length
YLZ:    .res 1          ; y length

FRMBUFHI:  .res 1          ; hi byte of screen buffer.

;----------------------------------------------------------------
        .segment "FS1CODE"

; SETCOL: A = color 0..3 -> sets COLZ. Trashes X.
SETCOL: TAX
        LDA COLTAB,x
        STA COLZ
        RTS

; PLOTL: draw line (X1,Y1)-(X2,Y2) in color COLZ
PLOTL:
; check if going right (x1 <= x2)
        ldx X2
        cpx X1
        bcs right
; going left, swap points
        lda X1
        stx X1
        sta X2
        tax
        ldy Y1
        lda Y2
        sty Y2
        sta Y1
right:

	LDA X2			; Sbr: plot line (X1,Y1) to (X2,Y2)
        SEC
        SBC X1
        BCC L1C3E
        STA XLZ			; x length

        LDA Y2
        SEC
        SBC Y1
L1C09:  BCC L1C22
        STA YLZ			; y length

        SEC
        SBC XLZ         ; yl-xl
        BCC L1C31       ; shallow line
        JMP L1ECC       ; steep line

L1C15:  LDA #256-40
        STA MODAD+1     ; mod row inc to -40
        LDA #255
        STA MODAD+5
        JMP L1C55       ; draw

L1C22:  EOR #255
        CLC
        ADC #1
        STA YLZ         ; y length

        SEC
        SBC XLZ         ; yl-xl
        BCC L1C15       ; shallow line
        JMP L1E4A       ; steep line

L1C3E:  LDX X2
        STX X1
        EOR #255
        CLC
        ADC #1
        STA XLZ         ; x length

        LDA Y1
        LDX Y2
        SEC
        SBC Y2
        STX Y1
        JMP L1C09

L1C31:  LDA #40
        STA MODAD+1     ; mod row inc to +40
        LDA #0
        STA MODAD+5
;        JMP L1C55      ; draw      

L1C55:  LDA #0          ; draw shallow line
        SEC
        SBC XLZ
        SEC
        ROR a
        STA ACZ         ; -xl/2
        JSR SCRNP       ; set up screen ptr (returns A = BPZ)

        LDX XLZ
        INX             ; xl+1
        AND #$7F        ; 7-bit pattern
        CMP #$18
        BMI L1C88       ; <18

        CMP #$40
        BEQ L1C7E

L1C79:  JSR L1CB3       ; =30  path2
        BNE L1C9B

L1C7E:  JSR L1CA0       ; =40  path1
        BNE L1C9B

L1C88:  CMP #$06
        BMI L1C93       ; <06
        JSR L1D09       ; >06 =0C  path3

        BNE L1C9B

L1C93:  JSR L1DDB       ; =03  path4

L1C9B:  AND COLZ        ; v2. color support:  apply color 
        ORA (SPZ),Y
        STA (SPZ),Y
        RTS

L1CA0:  LDA ACZ         ; Path1 Sbr: pattern=C0
L1CA2:  DEX
        BEQ L1CBF       ; P1 done w 1 pixel

        CLC
        ADC YLZ
        BCS L1CC8       ; P1 next row w 1 pixel
        DEX
        BEQ L1CC2       ; P1 done w 2 pixels
        ADC YLZ
        BCS L1CD3       ; P1 next row w 2 pixels
        BNE L1CE6       ; P1 continue w 3 pixels

L1CB3:  DEX         ; Path2 Sbr: pattern=30
        BEQ L1CC5       ; P2 done w 1 pixel

        LDA ACZ
        CLC
        ADC YLZ
        BCS L1CDB       ; P2 next row w 1 pixel
        BNE L1CFC       ; P2 continue w 2 pixels

L1CBF:  LDA #$C0        ; P1
        RTS

L1CC2:  LDA #$F0        ; P1
        RTS

L1CC5:  LDA #$30        ; P2
        RTS

L1CC8:  SBC XLZ
        STA ACZ

        LDA #$C0        ; P1
        JSR L1DEC       ; P1 draw

        BCS L1CB3       ; P1>2

L1CD3:  SBC XLZ
        STA ACZ
        LDA #$F0        ; P1
        BNE L1CE1

L1CDB:  SBC XLZ
        STA ACZ
        LDA #$30        ; P2

L1CE1:  JSR L1DEC       ; P12 draw

        BCS L1D09       ; P12>3

L1CE6:  DEX             ; P1
        BEQ L1CF6       ; P1 done w 3 pixels
        ADC YLZ
        BCS L1D32       ; P1 next row w 3 pixels
        STA ACZ

        LDA #$FF        ; P1
        JMP L1DB0       ; P1 continue w 4 pixels

L1CF3:  LDA #$3C
        RTS

L1CF6:  LDA #$FC        ; P1
        RTS

L1CF9:  LDA #$0C        ; P3
        RTS

L1CFC:  DEX             ; P2
        BEQ L1CF3       ; P2 done w 2 pixels
        ADC YLZ
        BCS L1D2A       ; P2 next row w 2 pixels
        STA ACZ

        LDA #$3F        ; P2
        JMP L1DB0       ; P2 continue w 3 pixels

L1D09:  DEX         ; Path3 Sbr: pattern=0C
        BEQ L1CF9       ; P3 done w 1 pixel
        LDA ACZ
        CLC
        ADC YLZ
        BCS L1D22       ; P3 next row w 1 pixel
        STA ACZ

        LDA #$0F        ; P3 continue w 2 pixels
     JMP L1DB0       ; P123 draw P123>4

L1D22:  SBC XLZ
        STA ACZ

        LDA #$0C        ; P3
        BNE L1D38

L1D2A:  SBC XLZ
        STA ACZ

        LDA #$3C        ; P2
        BNE L1D38

L1D32:  SBC XLZ
        STA ACZ
        LDA #$FC        ; P1
L1D38:  JSR L1DEC       ; P123 draw

L1DDB:  LDA #$03        ; Path4 Sbr: pattern=03

L1DB0:  AND COLZ        ; v2. ADD COLOR apply color
        ORA (SPZ),Y     ; P4 draw
        STA (SPZ),Y
        INY
        DEX
        BEQ L1DDF       ; P4 done

        LDA ACZ
        CLC
        ADC YLZ
        BCS L1DE2       ; P4 next row
        JMP L1CA2       ; P4 continue

L1DDF:  PLA
        PLA
        RTS

L1DE2:  SBC XLZ
        STA ACZ
        JSR L1DF0       ; P4 next row

        JMP L1CA0       ; P4 continue

L1DEC:  AND COLZ        ; v2. apply color
        ORA (SPZ),Y
        STA (SPZ),Y

L1DF0:  LDA SPZ
        CLC
MODAD:  ADC #40         ; self mod'd to -40      ; MODAD+1
        STA SPZ
	LDA #0						; MODAD+5
    ADC SPZ+1
    STA SPZ+1       ; next row
    SEC
    RTS

L1E4A:  LDA YLZ         ; draw steep line, going up
        CLC
        ROR a
        STA ACZ         ; half y length
        JSR SCRNP       ; set up screen ptr

        LDX YLZ
        INX
L1E58:  LDA BPZ
        AND COLZ        ; v2. apply color
        ORA (SPZ),Y
        STA (SPZ),Y     ; draw pixel
        DEX
        BEQ L1E8D

L1E63:  LDA SPZ         ; 1 row up (-40)
        SEC
        SBC #40
        STA SPZ
        BCS :+
        DEC SPZ+1
:
        LDA ACZ
        SEC
        SBC XLZ
        STA ACZ
        BCS L1E58
        ADC YLZ
        STA ACZ

        CLC
        LDA BPZ
        ROR a
        ROR a
        BCC L1E84
        ROR a
        INY
L1E84:  STA BPZ         ; 1 col right
        AND COLZ        ; v2. apply color
        ORA (SPZ),Y
        STA (SPZ),Y     ; draw pixel
        DEX
        BNE L1E63
L1E8D:  RTS

L1ECC:  LDA YLZ         ; draw steep line, going down
        CLC
        ROR a
        STA ACZ         ; half y length
        JSR SCRNP       ; set up screen ptr

        LDX YLZ
        INX
L1EDA:  LDA BPZ
        AND COLZ        ; v2. apply color
        ORA (SPZ),Y
        STA (SPZ),Y     ; draw pixel
        DEX
        BEQ L1F0F

L1EE5:  LDA SPZ         ; 1 row down (+40)
    CLC
    ADC #40
    STA SPZ
    BCC :+
    INC SPZ+1
:
;	LDA #0
;	ADC SPZ+1
;	STA SPZ+1		; 1 row down

        LDA ACZ
        SEC
        SBC XLZ
        STA ACZ
        BCS L1EDA
        ADC YLZ
        STA ACZ

        CLC
        LDA BPZ
        ROR a
        ROR a
        BCC L1F06
        ROR a
        INY
L1F06:  STA BPZ         ; 1 col right
        AND COLZ        ; v2. apply color
        ORA (SPZ),Y
        STA (SPZ),Y     ; draw pixel
        DEX
        BNE L1EE5
L1F0F:  RTS

;        .segment "CODE"
; SCRNP: set up for first pixel at (X1,Y1)
;   out: SPZ = row address, Y = byte within row,
;        BPZ = A = 2-bit mask for the pixel ($C0,$30,$0C,$03)
SCRNP:  LDX Y1   ; vertical address calc

        
        LDA ROWLO,x
        STA SPZ

        ; add framebuffer hibyte to row high byte
        LDA ROWHI,x
        ORA FRMBUFHI  ; can ORA instead of Add as $2000 and $3000 are 4KB aligned
        STA SPZ+1
       
        ; horizontal address calc 
        LDX X1
        LDY X_OFFSET, X
    
        LDA X_MASK, x
        STA BPZ

        RTS


;; can't really use this in ROM.
;;; INICL: patch CLEAR's 30 store addresses to cover BMP..BMP+$1DFF
;;INICL:  LDX #>BMP
;;        LDY #90
;;@loop:  DEY
;;        TXA
;;        STA CLOOP,y     ; high byte of operAND
;;        DEY
;;        LDA #<BMP
;;        STA CLOOP,y     ; low byte of operAND
;;        INX
;;        DEY             ; skip opcode
;;        BNE @loop
;;
;;        RTS
;;
;;; CLEAR: clear the bitmap (7680 bytes). Call INICL once first.
;;CLEAR:  LDA #0
;;        TAY
;;CLOOP:
;;        .repeat 30
;;        STA $FFFF,y     ; operAND patched by INICL
;;        .endrepeat
;;        INY
;;        BNE CLOOP
;;        RTS
;;


CLEAR0: LDA #0
        TAY 
CLOOP0:
        .repeat 15,  I
            STA $2000 + (I * $100),y     ; hardcoding $2000
        .endrepeat
        INY
        BNE CLOOP0
        RTS

    

CLEAR1: LDA #0
        TAY 
CLOOP1:
        .repeat 15,  I
            STA $3000 + (I * $100),y     ; hardcoding $3000
        .endrepeat
        INY
        BNE CLOOP1
        RTS

;----------------------------------------------------------------
        .segment "RODATA"

BITS:   .byte $C0, $30, $0C, $03        ; mask for each x&3
COLTAB: .byte $00, $55, $AA, $FF        ; fill byte for each color

; row address tables: row y STARTS at BMP + y*40
; page aligned so LDA table,x never pays the page-cross cycle
        .align 256
ROWLO:
    .repeat 96,I 
        .byte <(I * 40)
    .endrepeat

ROWHI:
    .repeat 96, I
        .byte >( I * 40)
    .endrepeat

.align 256

X_OFFSET:
    .repeat 160, COL
        .byte COL / 4
    .endrepeat

.align 256

X_MASK:
    .repeat 160, COL
        .byte $C0 >> ((COL & 3) * 2)
    .endrepeat
