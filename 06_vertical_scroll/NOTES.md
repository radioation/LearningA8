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

