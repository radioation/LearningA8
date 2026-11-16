# DLI
*IMP* unlike the A8 version, I put the image data into ROM 
 and manually loaded it with a memcpy.  There's nothing 
 to load it otherwise.
*IMP* I could also just point to ROM as the image itself
  is static, but I chose to use RAM as interactive programs
  I write are unlikely to have static graphics.

*IMP* As we need to set the high bit in the display-list
  for the DLIs, either hardcode them in the DLIST in ROM
  or make a RAM copy and change  the high bit as needed.
  for this example, I chose to use ROM

```s
main_dlist:
        .byte $70, $70               ;  (16 blank SCAN lines provide for "overscan"
        .byte $F0                    ; 8 blank lines $70 + interrupt.
 
        .byte $42                    ; LMS + sets ANTIC 2 (GR.0)
        .word _text_memory            ; memory address 2 bytes
        .byte $02                    ; second line of ANTIC 2  ( 16 scanlines at this point )

        ; 100 ANTIC E lines  here
        .byte $4e                    ; Second  LMS + sets ANTIC mode E
        .word _img_data          ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 22  
            .byte $0e               
        .endrepeat
        .byte $8e                ; set interrupt $80 and mode E
        .repeat 76  
            .byte $0e               
        .endrepeat
```
I had previously set them later when initializing the interrupts.
this works on A8 which is entirely RAM based

```s
   lda #$F0                   ; SET INTERRUPT $80 and last  blank $70
   sta main_dlist + 2         ; move to the line we want the DLI to fire

   lda #$8e                   ; SET INTERRUPT $80 and last moed e $0e
   sta main_dlist + 32        ; move to the line we want the DLI to fire
```


