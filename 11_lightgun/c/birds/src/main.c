#include <atari.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <peekpoke.h>

#include  <atari_screen_charmap.h>    // Pesky Atari mapping.
#define joy0 PEEK(632)

void init_dlist(void);
void init_interrupts(void);
extern uint8_t img_data[];
extern uint8_t text_memory[];

extern uint8_t x0;
extern uint8_t x1;
extern uint8_t x2;
extern uint8_t x3;
uint8_t y0 = 20;
uint8_t y1 = 40;
uint8_t y2 = 60;
uint8_t y3 = 80;

uint8_t p0 = 1;
uint8_t p1 = 1;
uint8_t p2 = 1;
uint8_t p3 = 1;

volatile uint8_t gun_x;
volatile int16_t corrected_x;
volatile uint8_t gun_y;
//volatile uint8_t corrected_y; not needed for double line resolution
volatile uint8_t old_y;
#include "pmg.h"

int main(void)
{
  char msg[] = "Atari Computers!";
  //uint8_t i,j,count,type, offset
  uint16_t i, offset;
  uint8_t joydir = 0;
  // pmg address handled by linker.
  uint16_t p = (uint16_t) pmgMem;

  x0 =25;
  x1 = 100;
  x2 = 100;
  x3 = 100;

  memset( text_memory, 0, 80 ); 
  memcpy( text_memory + 52, msg, 16 );
  /*
     _setcolor(0, 0, 12);
     _setcolor(1, 0, 8);
     _setcolor(2, 0, 4);
     _setcolor(3, 2, 10);
     */

  ///////////////////////////////////////////////////
  // INITIALIZE PMG /////////////////////////////////

  // set pmbase with ANTIC
  ANTIC.pmbase = p >> 8;
  memset( pmgMem, 0, 2048 );
  // set DMACTL
  OS.sdmctl = 46;    // POKE 559, 46 // 46 double line resolution, 62 single line
                     // set GRACTL
  GTIA_WRITE.gractl = 0x03; // POKE 53277,e players and missile

  // widths
  GTIA_WRITE.sizep0 = 0x01;
  GTIA_WRITE.sizep1 = 0x01;
  GTIA_WRITE.sizep2 = 0x01;
  GTIA_WRITE.sizep3 = 0x01;


  OS.color4 = 0x98;
  // priority
  OS.gprior = 49; // 32 overlap has color, 1 players over playfield, 16 combine

  GTIA_WRITE.hposp0=x0;
  OS.pcolr0= 0x74;
  GTIA_WRITE.hposp1=x0;
  OS.pcolr1= 0x3c;

  GTIA_WRITE.hposp2=x0+8;
  OS.pcolr2= 0x74;
  GTIA_WRITE.hposp3=x0+8;
  OS.pcolr3= 0x3c;




  init_dlist();

  init_interrupts();
  ANTIC.nmien = 0xc0;  // activate BOTH display list ($80) and vertical blank $(40) interrupts
  old_y = 0;

  while(1) {
    // update lightgun 
    // GET X position of lightgun
    gun_x = PEEKW( 564 ) ;         
    corrected_x = gun_x - 42;  // 40 may need to change on different systems
    if( corrected_x < 1 ) corrected_x += 227; 

    // get Y position of lightpen
    gun_y = PEEK( 565 );        
    //corrected_y = gun_y;  // Y - needs correction for single line resolution sprites

    GTIA_WRITE.hposm3 = corrected_x;
    if (gun_y != old_y ) {
      *( pmgMem + 384 + old_y  ) = 0;
      *( pmgMem + 385 + old_y  ) = 0;

      *( pmgMem + 384 + gun_y  ) = 192;
      *( pmgMem + 385 + gun_y  ) = 192;
      old_y = gun_y;
    }

    // tick
    i += 1;
    // Update Duck positions
    if ( i%20 == 0 ) {
      // TODO: copy/paste is the worst form of reuse. rewrite this
      if( p0 ) {
        x0 +=1; 
        if ( x0 > 213 ) x0 = 25;
      }
      if( p1 ) {
        x1 -=1;
        if ( x1 < 25 ) x1 = 213; 
      }
      if( p2 ) {
        x2 +=1;
        if ( x2 > 213 ) x2 = 25; 
      }
      if( p3 ) {
        x3 -=1; 
        if ( x3 < 25 ) x3 = 213;
      }
    }
    joydir = 15 & joy0;
    if( joydir == 15 ) {
      // TODO: copy/paste is the worst form of reuse. rewrite this
      if( corrected_x > x0 && corrected_x < x0 + 32 && gun_y > y0 && gun_y < y0 + 16 ) {
        p0 = 0;
        x0 = 0; 
      }
      if( corrected_x > x1 && corrected_x < x1 + 32 && gun_y > y1 && gun_y < y1 + 16 ) {
        p1 = 0;
        x1 = 0; 
      }
      if( corrected_x > x2 && corrected_x < x2 + 32 && gun_y > y2 && gun_y < y2 + 16 ) {
        p2 = 0;
        x2 = 0; 
      }
      if( corrected_x > x3 && corrected_x < x3 + 32 && gun_y > y3 && gun_y < y3 + 16 ) {
        p3 = 0;
        x3 = 0; 
      }

    }

    // update animation frame once in a while
    if ( i%120 == 0 ) {

      offset+=spr_height;
      if( offset > 32  ) {
        offset = 0;
      }
      // TODO: copy/paste is the worst form of reuse. rewrite this
      memcpy( pmgMem + 512 + y0, spr_fr_0_frm_0 + offset, spr_height );
      memcpy( pmgMem + 640 + y0, spr_fr_1_frm_0 + offset, spr_height );
      memcpy( pmgMem + 768 + y0, spr_fr_2_frm_0 + offset, spr_height );
      memcpy( pmgMem + 896 + y0, spr_fr_3_frm_0 + offset, spr_height );

      memcpy( pmgMem + 512 + y1, spr_fl_0_frm_0 + offset, spr_height );
      memcpy( pmgMem + 640 + y1, spr_fl_1_frm_0 + offset, spr_height );
      memcpy( pmgMem + 768 + y1, spr_fl_2_frm_0 + offset, spr_height );
      memcpy( pmgMem + 896 + y1, spr_fl_3_frm_0 + offset, spr_height );

      memcpy( pmgMem + 512 + y2, spr_fr_0_frm_0 + offset, spr_height );
      memcpy( pmgMem + 640 + y2, spr_fr_1_frm_0 + offset, spr_height );
      memcpy( pmgMem + 768 + y2, spr_fr_2_frm_0 + offset, spr_height );
      memcpy( pmgMem + 896 + y2, spr_fr_3_frm_0 + offset, spr_height );

      memcpy( pmgMem + 512 + y3, spr_fl_0_frm_0 + offset, spr_height );
      memcpy( pmgMem + 640 + y3, spr_fl_1_frm_0 + offset, spr_height );
      memcpy( pmgMem + 768 + y3, spr_fl_2_frm_0 + offset, spr_height );
      memcpy( pmgMem + 896 + y3, spr_fl_3_frm_0 + offset, spr_height );

    }
  }

}


