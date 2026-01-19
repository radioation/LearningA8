#include <atari.h>
#include <string.h>
#include <stdint.h>
#include <peekpoke.h>
#include <conio.h>

volatile uint8_t gun_x;
volatile int16_t corrected_x;
volatile uint8_t gun_y;
//volatile uint8_t corrected_y;
volatile uint8_t old_y;

#include "pmg.h"

int main(void)
{
  // pmg address handled by linker.
  uint16_t p = (uint16_t) pmgMem;

  clrscr();

  ///////////////////////////////////////////////////
  // INITIALIZE PMG /////////////////////////////////

  // set pmbase with ANTIC
  ANTIC.pmbase = p >> 8;
  memset( pmgMem, 0, 2048 );
  // set DMACTL - 46 double line resolution, 62 single line
  OS.sdmctl = 46;  

  // set GRACTL
  GTIA_WRITE.gractl = 0x03; // POKE 53277,e players and missile

  // widths
  GTIA_WRITE.sizep0 = 0x00;

  // colors
  OS.color2 = 0x98;
  OS.color4 = 0x98;
  OS.gprior = 1;
  OS.pcolr0= 0x7a;


  old_y = 0;

  while(1) {
    gun_x = PEEK( 564 ) ;         
    corrected_x = gun_x - 42 ;  // 40 may need to change on different systems
    if( corrected_x < 1 ) corrected_x += 227; 

    gun_y = PEEK( 565 );        

    //gotoxy(1,1);
    //cprintf("x: %d cx: %d y: %d  ", gun_x, corrected_x, gun_y );

    // draw pointer
    if (gun_y != old_y ) {
      *( pmgMem + 512 + old_y  ) = 0;
      *( pmgMem + 513 + old_y  ) = 0;

      *( pmgMem + 512 + gun_y  ) = 192;
      *( pmgMem + 513 + gun_y  ) = 192;
      old_y = gun_y;
    }
    GTIA_WRITE.hposp0=corrected_x;

  }

}
