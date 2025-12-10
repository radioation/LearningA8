# Vertical Scrolling

*THE* page to learn about scrolling is [Player Missile](https://playermissile.com/scrolling_tutorial/index.html#topic-1-vertical-coarse-scrolling)


Vertical scrolling works out pretty easily because the way A8s lay out video
memory. Each line in a playfield to to bottom is just a straight run of bytes,
one line following the other. 

Since Display Lists are provided with a start-address (LMS), we can just change 
the adress following the LMS to move up and down the field.


Put a label on the screen memory address after your display list LMS
```s
.export _dlist_scroll_address

main_dlist:
        .byte $70, $70, $70         

        .byte $42                  
        .word text_memory        
        .byte $02                 

        .byte $44               
_dlist_scroll_address:               ; LABEL THE ADDRESS OF SCREEN MEMORY SO WE CAN UPDATE IT LATER.
        .word screen_memory    
        .repeat 23
            .byte $04         
        .endrepeat

        .byte $41
        .word main_dlist       


```

In C (but really do assembly) refer to both `_screen_memory` and `_dlist_scroll_address` as 
arrays.

```c
extern uint8_t screen_memory[];
extern uint8_t dlist_scroll_address[2];
```

Simple additions and subtractions will move the screen up and down
```c

      scroll_pos += 40;
      screen_scroll = (uint16_t) (screen_memory + scroll_pos);
      dlist_scroll_address[0] = (uint8_t)(screen_scroll&0xFF);
      dlist_scroll_address[1] = (uint8_t)(screen_scroll >> 8);

```

Assembly scrolling

```s
        clc
        lda dlist_scroll_address
        adc #40
        sta dlist_scroll_address
        lda dlist_scroll_address+1
        adc #0
        sta dlist_scroll_address+1
        rts

```


# Fine scrolling
# VSCROLL

From [Mapping the atari](https://www.atariarchives.org/mapping/memorymap.php)
> 54277          	D405          	VSCROL
> 
>      (W) Vertical scroll enable, *POKE VSCROL with from zero to 16
>      scan lines, depending on the GRAPHICS mode of the screen for
>      the number of scan lines to scroll.* Vertical fine scrolls can be
>      used *only if BIT 5 of the display list instruction has been set.*


*IMP* we need to change the display list lines to have bit 5( $20 ) set

So `$44` becamse `$64`  and `$04` becomes  `$24` 
```s
        .byte $64                    ; LMS + sets ANTIC 4
_dlist_scroll_address:               ; LABEL THE ADDRESS OF SCREEN MEMORY SO WE CAN UPDATE IT LATER.
        .word screen_memory          ; gives address of start of screen memory. ( DL and DL+1)
        .repeat 23
            .byte $24                ; total of 24 lines.
        .endrepeat
```
ON the C side, you want to update `$D405` with the current fine scrolling value
and update coarse scrolling when you hit the max 

```c
      vert_scroll += 1;
      // if we're at max fine vert scroll, reset fine to 0 and do a coarse scroll
      if( vert_scroll == VERT_SCROLL_MAX ) {
          vert_scroll = 0;
          scroll_pos += 40;
          screen_scroll = (uint16_t) (screen_memory + scroll_pos);
          dlist_scroll_address[0] = (uint8_t)(screen_scroll&0xFF);
          dlist_scroll_address[1] = (uint8_t)(screen_scroll >> 8);
      }
      // store/poke current fine scroll value into 0xd405
      //POKE( 0xD405, vert_scroll );
      *(uint8_t*)(0xD405) = vert_scroll;
      

```



## delays
Sleep(1) is probably not the best delay. I deally you'd want the CPU 
doing other work and just update scrolling after some time has elapsed.
The atari has a rela time clock `RTCLOK` at locations 18,19, and 20 (0x12, 
0x13, and 0x14). From [Mapping the atari](https://www.atariarchives.org/mapping/memorymap.php)

>      Internal realtime clock. *Location 20 increments every stage one
>      VBLANK interrupt (1/60 second = one jiffy)* until it reaches 255
>      ($FF); then location 19 is incremented by one and 20 is reset to
>      zero (every 4.27 seconds). When location 19 reaches 255, it and
>      20 are reset to zero and location 18 is incremented by one (every
>      18.2 minutes or 65536 TV frames). To use these locations as a
>      timer of seconds, try:
> 
>      TIME = INT((PEEK(18) * 65536 + PEEK(19) * 256 +
>             PEEK(20) )/60)
>


 






