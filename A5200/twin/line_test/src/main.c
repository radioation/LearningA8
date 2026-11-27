#include <atari5200.h>
#include <string.h>
#include <stdint.h>

#include "display.h"
#include "fs1line.h"

extern void stress_test();
extern void test_edge_cases();

extern int8_t sin_tab[256];
extern int8_t sin_tab2[256];

void set_pixel(uint8_t x, uint8_t y ) {

    if( (x & 0x03) == 3 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b00000011;
    }else if( (x & 0x02) == 2 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b00001100;
    }else if( (x & 0x01) == 1 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b00110000;
    }else if( (x & 0x03) == 0 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b11000000;
    }
}
void set_pixel2(uint8_t x, uint8_t y ) {

    if( (x & 0x03) == 3 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b00000010;
    }else if( (x & 0x02) == 2 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b00001000;
    }else if( (x & 0x01) == 1 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b00100000;
    }else if( (x & 0x03) == 0 ) {
        FRAME_BUFFER_0[ (int) y * 40 + (x>>2)  ] = 0b10000000;
    }
}

extern unsigned char _FS1CODE_LOAD__[];
extern unsigned char _FS1CODE_RUN__[];
extern unsigned char _FS1CODE_SIZE__[];

void fs_load_code()
{
    memcpy(
            _FS1CODE_RUN__,
            _FS1CODE_LOAD__,
            (uint16_t)_FS1CODE_SIZE__
          );
}


int main(void)
{
    uint8_t i, j;
    const uint8_t center_x_byte =  20;
    const uint8_t center_y =  40;
    const uint8_t center_px =  80;

    fs_load_code();
    init_dlist();
    //    memset( FRAME_BUFFER_0, 0, 3840 );   // no-zero 
    fs1_clear_0();

    OS.color1 = 0x0F; // text
    OS.color2 = 0x48; // bg
    OS.color3 = 0x88; // bg
    OS.color4 = 0x00;

    fs1_fb_hi = 0x20; 
    fs1_setcol( 1 );

   //stress_test(); // test



    //
    FRAME_BUFFER_0[0] = 0xFF;
    FRAME_BUFFER_0[40] = 0b01000000; // higher bits are leftward.
    FRAME_BUFFER_0[80] = 0b10000000;
    FRAME_BUFFER_0[120] = 0b11000000;

    //    FRAME_BUFFER_0[ 20 ] = 0xff;
    //  ANTIC D portion is 160 x 80 so mid point is 80,40
    //  but a row is 40 bytes, so midpoint of a row is ~20.
    //  so (80/2)*40 for centered row, +20 for center of the current row
    FRAME_BUFFER_0[(int)center_x_byte + (int)center_y *40 ] = 0b00000001;

    //fs1_fb_hi = 0x30; 

    fs1_setcol( 1 );
    //fs1_line( 10, 10, 149, 10 );
    //fs1_line( 10, 10, 149, 10 );
    //fs1_line( 10, 10, 149, 10 );
    fs1_x1 = 10; fs1_y1 = 10, fs1_x2 = 150, fs1_y2 = 10; fs1_plotl();

    fs1_line( 10, 10, 10, 70 );
    fs1_line( 10, 70, 150, 70 );
    fs1_line( 150, 10, 150, 70 );


   //stress_test(); // test
   test_edge_cases(); // test



/*
    j= 0;
    for( i = 10; i <= 150; i += 10, j++ ) {
        fs1_setcol( j %3 + 1 );
        fs1_line( i, 10, 160-i, 70 );
    }
    for( i = 10; i <= 70; i += 10, j++ ) {
        fs1_setcol( (j %3) + 1 );
        fs1_line( 10, i, 150, 80 -i );
    }

*/

    fs1_setcol( 2 );
    for( i = 0; i < 8; i++ ) {
        //set_pixel( center_px + ( 10 * sin_tab[(uint8_t)(j+64)] )/127, center_y + ( 10 * sin_tab[(uint8_t)(i)] )/127 );
        fs1_line( center_px + ( 30 * sin_tab[(uint8_t)(j+64)] )/127, 
                center_y + ( 30 * sin_tab[(uint8_t)(j)] )/127,
                center_px + ( 30 * sin_tab[(uint8_t)(j+128)] )/127,
                center_y + ( 30 * sin_tab[(uint8_t)(j+64)] )/127 );
        j+=32;

    }
    
        fs1_setcol( 3 );
        j+= 16;
        for( i = 0; i < 8; i++ ) {
            fs1_line( center_px + ( 20 * sin_tab[(uint8_t)(j+64)] )/127, 
                    center_y + ( 20 * sin_tab[(uint8_t)(j)] )/127,
                    center_px + ( 20 * sin_tab[(uint8_t)(j+128)] )/127,
                    center_y + ( 20 * sin_tab[(uint8_t)(j+64)] )/127 );
            j+=32;
    
        }
    

    while (1) { }

}


