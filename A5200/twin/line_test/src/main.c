#include <atari5200.h>
#include <string.h>
#include <stdint.h>

#include "display.h"
#include "fs1line.h"

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


int main(void)
{
  uint8_t i, j;
  const uint8_t center_x_byte =  20;
  const uint8_t center_y =  40;
  const uint8_t center_px =  80;

  init_dlist();
  fs1_init();
  fs1_clear();
  


  FRAME_BUFFER_0[0] = 0xFF;
  FRAME_BUFFER_0[40] = 0b01000000; // higher bits are leftward.
  FRAME_BUFFER_0[80] = 0b10000000;
  FRAME_BUFFER_0[120] = 0b11000000;

  //    FRAME_BUFFER_0[ 20 ] = 0xff;
  //  ANTIC D portion is 160 x 80 so mid point is 80,40
  //  but a row is 40 bytes, so midpoint of a row is ~20.
  //  so (80/2)*40 for centered row, +20 for center of the current row
  FRAME_BUFFER_0[(int)center_x_byte + (int)center_y *40 ] = 0b01000000;

  fs1_setcol( 1 );
  fs1_line( 10, 10, 149, 10 );
  fs1_line( 10, 10, 10, 70 );
  fs1_line( 10, 70, 149, 70 );
  fs1_line( 149, 10, 149, 70 );

  set_pixel( 0, 0 );
  set_pixel( 0, 10 );
  set_pixel( 4, 11 );
  set_pixel( 5, 12 );
  set_pixel( 6, 13 );
  set_pixel( 7, 14 );
  set_pixel( 0, 79 );
//
//  // sin(0) is sin_tab[0]
//  // cos(0) just +64  sin_tab[0+64]
//
//  set_pixel( center_px, center_y );
  fs1_setcol( 2 );
  j= 0;
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


