#include <atari5200.h>
#include <string.h>
#include <stdint.h>

#include "display.h"


extern int8_t sin_tab[256];
extern int8_t sin_tab2[256];

void set_pixel(uint8_t x, uint8_t y ) {

  if( (x & 0x03) == 3 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b00000011;
  }else if( (x & 0x02) == 2 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b00001100;
  }else if( (x & 0x01) == 1 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b00110000;
  }else if( (x & 0x03) == 0 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b11000000;
  }
}
void set_pixel2(uint8_t x, uint8_t y ) {

  if( (x & 0x03) == 3 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b00000010;
  }else if( (x & 0x02) == 2 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b00001000;
  }else if( (x & 0x01) == 1 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b00100000;
  }else if( (x & 0x03) == 0 ) {
    screen_ram[ (int) y * 40 + (x>>2)  ] = 0b10000000;
  }
}


int main(void)
{
  uint8_t i;
  const uint8_t center_x_byte =  20;
  const uint8_t center_y =  40;
  const uint8_t center_px =  80;

  init_dlist();

  memset(screen_ram, 0,   (80 * 40 ) + ( 4* 20) );

    OS.color1 = 0x0F; // text
    OS.color2 = 0x48; // bg
    OS.color3 = 0x88; // bg
    OS.color4 = 0x00;



  //screen_ram[0] = 0xFF;
  screen_ram[40] = 0b01000000; // higher bits are leftward.
  screen_ram[80] = 0b10000000;
  screen_ram[120] = 0b11000000;

  //    screen_ram[ 20 ] = 0xff;
  //  ANTIC D portion is 160 x 80 so mid point is 80,40
  //  but a row is 40 bytes, so midpoint of a row is ~20.
  //  so (80/2)*40 for centered row, +20 for center of the current row
  screen_ram[(int)center_x_byte + (int)center_y *40 ] = 0b01000000;

  set_pixel( 0, 0 );
  set_pixel( 0, 10 );
  set_pixel( 4, 11 );
  set_pixel( 5, 12 );
  set_pixel( 6, 13 );
  set_pixel( 7, 14 );
  set_pixel( 0, 79 );

  // sin(0) is sin_tab[0]
  // cos(0) just +64  sin_tab[0+64]

  set_pixel( center_px, center_y );
  for( i = 0; i < 255; ++i ) {
    set_pixel( center_px + ( 10 * sin_tab[(uint8_t)(i+64)] )/127, center_y + ( 10 * sin_tab[(uint8_t)(i)] )/127 );
  }
  for( i = 0; i < 255; ++i ) {
    set_pixel2( center_px + ( 10 * sin_tab2[(uint8_t)(i+64)] )/127, center_y + ( 10 * sin_tab2[(uint8_t)(i)] )/127 );
  }

  while (1) { }

}


