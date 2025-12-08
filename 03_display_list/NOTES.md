# C
* put DLIST in page 6
  * *IMP* Use of GR.8 means this list will require TWO `DL_LMS()` lines.

* Put screen memory in two 4K blocks to handle GR.8 portion
  * kept it entirely out of MAIN memory block by using $2000-$3FFF
    and modifying `atari-xex.cfg` to set `STARTADDRESS` to $4000

  * The initial block of screen memory starts at $2010 instead of $2000
    to make it line up properly with the 4K boundary. (simplifying drawing
    routines)


* standard C char's don't actually copy well into screen memory. 
  included "<atari_screen_charmap.h>" to have display show the right 
  letters"


# Assembly
A display list can be easily made in assembly as well:

```s
        .segment "DLIST"
.align $0100

main_dlist:
        .byte $70, $70, $70          ; 3 lines at top blank (24 blank SCAN lines provide for "overscan"

        .byte $47                    ; LMS + sets ANTIC 7 and one line (would be DL-1)
        .word screen_block0          ; gives address of start of screen memory. ( DL and DL+1)
        .byte $7                     ; second ANTIC 7 line  ( DL+2)


        .repeat 101
            .byte $0f                ; first 101 of 128 lines of GR.8 / antic 15
        .endrepeat

        .byte $4f                   ; second LMS and address for 4K boundray (and GR.8)
        .word screen_block1

        .repeat 27
            .byte $0f                ; remaining lines of GR.8
        .endrepeat

        .byte $6, $6, $6, $6         ; 4 lines of GR.1

        .byte $41
        .word main_dlist             ; JVB ( vertical blank jump to start of display list
```



