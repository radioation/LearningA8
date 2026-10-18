#include <atari.h>
#include <string.h>
#include <stdint.h>

#include "chars.h"
#include "pmg.h"

#include <peekpoke.h>

#define joy0 PEEK(632)
#define trig0 PEEK(644)
// display list
void init_dlist(void);
void init_interrupts(void);
void coarse_scroll_right(void);
void fine_scroll_right(void);


// mwp vars
extern uint8_t screen_memory[];
extern uint8_t sline0[];
extern uint8_t message_memory[];
//extern uint8_t horz_scroll;
//extern uint8_t delay_count;
//extern uint8_t delay;
uint8_t dir;
extern volatile uint8_t mwpx;
extern volatile uint8_t mwpy;
extern volatile uint8_t finex;
extern volatile uint8_t finey;
extern uint8_t main_dlist[];
extern uint8_t lms1[];
extern uint8_t mwp_toplo[];
extern uint8_t mwp_tophi[];



extern volatile uint8_t pend_vdir;    /* 0xFF = no coarse step */
extern volatile uint8_t pend_hdir;    /* 0xFF = no coarse step */
extern volatile uint8_t pend_finex, pend_finey;
extern volatile uint8_t commit;
void __fastcall__ mwp(unsigned char dir);   /* 0 down, 1 up, 2 right, 3 left */


// world chunks
extern void init_chunks();
extern uint8_t fill_column(uint16_t wx, uint16_t wy, uint16_t o);
extern uint8_t fill_row(unsigned wx, unsigned wy, int16_t o);



static void submit(uint8_t vdir, uint8_t hdir, uint8_t fx, uint8_t fy)
{
  while (commit) ;          /* previous update not applied yet */
  pend_vdir   = vdir;
  pend_hdir   = hdir;
  pend_finex = fx;
  pend_finey = fy;
  commit     = 1;           /* written last */
}


static void puthex(unsigned char *dst, unsigned char v)
{
  unsigned char n = v >> 4;
  dst[0] = n < 10 ? 0x10 + n : 0x21 + (n - 10);
  n = v & 0x0F;
  dst[1] = n < 10 ? 0x10 + n : 0x21 + (n - 10);
}




static void putdec2(unsigned char *dst, unsigned char v)
{
  dst[0] = 0x10 + v / 10;
  dst[1] = 0x10 + v % 10;
}


static void putdec3(unsigned char *dst, unsigned char v)
{
  dst[0] = 0x10 + v/100 ;
  dst[1] = 0x10 + (v/10) %10;
  dst[2] = 0x10 + v % 10;
}

static void putdec4(unsigned char *dst, unsigned short v)
{
  dst[0] = 0x10 + v/1000 ;
  dst[1] = 0x10 + (v/100) %10 ;
  dst[2] = 0x10 + (v/10) %10;
  dst[3] = 0x10 + v % 10;
}



int main(void)
{
  uint8_t stick, vdir, hdir, flag;
  uint16_t i, j, k , addr;
  uint16_t player_x, player_y;
  uint16_t world_x, world_y;
  uint16_t lms1_addr; 
  int16_t lms1_mem_offset = 0;
  //const uint16_t base_screen_address = (uint16_t)sline0;
  const uint16_t base_screen_address = (uint16_t)screen_memory;
  uint16_t p = (uint16_t) pmgMem;


  ///////////////////////////////////////////////////
  // INITIALIZE CHARS ///////////////////////////////
  // point to our charset.
  addr = (uint16_t) charset;
  OS.chbas = addr >> 8;

  memset( message_memory, 0, 4*40 );

  // setup characterset and playfield
  init_chunks();

  j = 33;
  k= 0; 
  world_x = 0;//40;
  world_y = 0;//54;
  for( i=0; i < 20; ++i ) {
    //  memset( sline0 + k, j, 48 );
    //fill_row( 0,i,k );  
    fill_row( world_x, world_y + i,k );  
    putdec4( message_memory + 20, k );
    k+=48;
    j++;
  }
  //fill_row( world_x, world_y+20,0 );  

  //memset( sline0 + k,  33 + 128 , 48 );
  //fill_row( 0, 0, 0 );
  //fill_row( 0, 1, 48 );
  // for( i=0; i < 48; ++i ) {
  //   fill_column( i,0,i );  
  // }
  set_colors();
  // setup dlist
  init_dlist();

  init_interrupts();
  //ANTIC.nmien = 0xc0;  // activate BOTH display list ($80) and vertical blank $(40) interrupts
  ANTIC.nmien = 0x40;  // activate  vertical blank $(40) interrupts

  //// set pmbase with ANTIC
  ANTIC.pmbase = p >> 8;
  memset( pmgMem, 0, 2048 );
  OS.sdmctl = 62;    // POKE 559, 46 single line resolution
  GTIA_WRITE.gractl = 0x03; // POKE 53277,3 players and missile

  GTIA_WRITE.sizep0 = 0x00;
  OS.pcolr0= 0x98;
  POKE( 623, 1 ); 
  player_x = 80;
  GTIA_WRITE.hposp0=player_x;
  memcpy( pmgMem + 1145, spr_0_frm_0, 16 );

  POKE( 0xD404, 15 ); // scrolling offset due to hscroll being on in Dlist  
                      //
                      //GTIA_WRITE.hposm2 = 0x50;
                      //GTIA_WRITE.hposm3 = 0x52 

  puthex( message_memory + 120, mwp_tophi[0] );
  puthex( message_memory + 124, mwp_toplo[2] );

  flag = 0;
  while(1) {
    waitvsync();
    lms1_addr  = (uint16_t)lms1[1] | ((uint16_t)lms1[2] << 8);
    lms1_mem_offset = lms1_addr - base_screen_address;
    putdec4( message_memory + 81, lms1_mem_offset );
    stick = OS.stick0;
    vdir = 255;
    hdir = 255;
    if( !(stick & JOY_DOWN_MASK) && world_y > 0 ) {
      if( finey == 0 ) {
        finey = 7;
        vdir = 0;
        world_y--;
        if( world_y < 0 ) world_y = 0;
        // tiles are moving down the screen, pull world tiles/chars
        // from the top ( no y offset)
        if( mwpy == 19 ) { 
        fill_row( world_x,world_y, (int16_t)lms1_mem_offset + 912 );
        } else {
        fill_row( world_x,world_y, (int16_t)lms1_mem_offset-48 );
        }
      } else {
        finey--;
      }
    }else if( !(stick & JOY_UP_MASK) && world_y < 108 ) {
      if( finey == 7 ) {
        finey = 0;
        vdir = 1;
        world_y++;
        if( world_y > 108) world_y = 108;
        // tiles are moving up the screen, pull world tiles/chars
        // from the bottom
        fill_row( world_x,world_y+19, lms1_mem_offset );
      } else {
        finey++;
      }
    }else if( !(stick & JOY_RIGHT_MASK) && world_x > 0 ) {
      if( finex == 3 ) {
        finex = 0;
        hdir =3;
        world_x--;
        if( world_x < 0 ) world_x = 0;
        // moving screen to the right, so pull world tiles from the left
        // ecgte (world_x)
        fill_column( world_x,world_y, lms1_mem_offset-1);
      } else {
        finex++;
      }
    }else if( !(stick & JOY_LEFT_MASK) && world_x < 143) {
      if( finex == 0 ) {
        finex = 3;
        hdir =2;
        world_x++;
        if( world_x > 143 ) world_x = 143;
        //putdec3( message_memory + 40, world_x );
        //putdec3( message_memory + 45, world_y );
        // moving screen to the left, so pull world tiles from the right
        // world x + 47 for offset because we incrementedx
        fill_column( world_x+47,world_y, lms1_mem_offset+48);
      } else {
        finex--;
      }
    }
    submit( vdir, hdir, finex, finey);

    putdec2( message_memory + 0, mwpx );
    putdec2( message_memory + 4, mwpy );
    putdec3( message_memory + 40, world_x );
    putdec3( message_memory + 45, world_y );
    //      puthex( message_memory + 50, lms1[0] );
    //      puthex( message_memory + 53, lms1[2] );
    //      puthex( message_memory + 55, lms1[1] );
  }
}
