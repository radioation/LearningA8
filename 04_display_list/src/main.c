#include <atari.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>

#include <atari_screen_charmap.h>    // Pesky Atari mapping.

#define DLIST_ADDR    0x0600                // store DLIST in page 6
#define SCREEN_MEMORY0 0x2010           // start of memory
#define SCREEN_MEMORY1 0x3000           // cross 4k boundary 


// https://www.atarimagazines.com/vbook/displaylistssimplified.html
// 2 lines of GR.2  -> 2x16 = 32 scanlines   (20 bytes per line, 40 total)
// 128 lines of GR.8 -> 128x1 = 128 scanlines (40 bytes per line, 5120 total, > 4k req)
// 4 lines of GR. 1 -> 4x8 = 32 scanlines  ( 20 bytes per line, 160 total );


static const void display_list = {
    // get mode info  at https://gury.atari8.info/refs/graphics_modes.php
    // get defines from https://github.com/cc65/cc65/blob/master/include/_antic.h

    // Blank scan lines provide for "overscan"
    DL_BLK8,   // 112
    DL_BLK8,   // 112
    DL_BLK8,   // 112

    // LMS: 64+7 sets ANTIC mode 7 / GR.2
    DL_LMS( DL_CHR20x16x2 ),
    SCREEN_MEMORY0,      // address of start of screen memory
    DL_CHR20x16x2,      // Sets ANTIC 7 for second mode line (equiv. of GR.2 )

    // 101 lines to 4k boundary
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1,


    DL_LMS( DL_MAP320x1x1 ), // LMS and address for 4K boundary jump.
    SCREEN_MEMORY1,      // includes one line ofi ANTIC 15/F GR.8

    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1, DL_MAP320x1x1,
    DL_MAP320x1x1, DL_MAP320x1x1, 


    DL_CHR20x8x2,   // sets ANTIC mode 6 for 4 lines
    DL_CHR20x8x2,
    DL_CHR20x8x2,
    DL_CHR20x8x2,

    DL_JVB, DLIST_ADDR
};


void draw_rect( unsigned char x, unsigned char y ) {
    unsigned char i, j;
    unsigned int offset = 0;
    unsigned int y_offset = 0;
    unsigned char *ptr = (unsigned char*)(SCREEN_MEMORY0  + 40); // point to start of GR.8 region
    for( j = 0; j < y; ++j ) {
        offset += 40;
    }
    offset += x;
    for( j = 0; j < 15; ++ j ) {
        *(ptr + offset + y_offset  ) |= 0x80; 
        *(ptr + offset + y_offset + 5  ) |= 0x01;
        y_offset += 40; 
    }
    for( i = 0; i < 6; ++i ) {
        *(ptr + offset + i  ) = 0xff; 
        *(ptr + offset + i + 600  ) |= 0xff; 
    }


} 

void main(void) {
    char msg1[] = "ATARI";   // used #include <atari_screen_charmap.h>
    char msg2[] = "IS FUN";

    memset((void *)SCREEN_MEMORY0, 0, 4096 );
    memset((void *)SCREEN_MEMORY1, 0, 4096 );
    memset((void *)DLIST_ADDR,0x00,256);
    memcpy((void *)DLIST_ADDR, &display_list, sizeof(display_list));
    OS.sdlst = (void *)DLIST_ADDR;

    // copy text to screen memory
    memcpy((void*)(SCREEN_MEMORY0 + 27), msg1, strlen( msg1 ) );
    memcpy((void*)(SCREEN_MEMORY1 + 28*40 + 27   ), msg2, 6 );

    draw_rect( 5, 30 );
    draw_rect( 8, 40 );
    draw_rect( 11, 50 );
    draw_rect( 14, 60 );
    draw_rect( 17, 70 );
    draw_rect( 20, 80 );
    draw_rect( 23, 90 );


    while(1) {
    }
}

