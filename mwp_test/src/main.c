#include <atari.h>
#include <string.h>
#include <stdint.h>

#include "chars.h"
#include "pmg.h"

#include <peekpoke.h>

#define joy0 PEEK(632)
#define trig0 PEEK(644)

void init_dlist(void);
void init_interrupts(void);
void coarse_scroll_right(void);
void fine_scroll_right(void);

extern uint8_t screen_memory[];
extern uint8_t sline0[];
extern uint8_t message_memory[];
extern uint8_t horz_scroll;
extern uint8_t delay_count;
extern uint8_t delay;
extern uint8_t mwpx;
extern uint8_t mwpy;
extern uint8_t main_dlist[];
extern uint8_t lms1[];

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


int main(void)
{
    uint16_t i, j, k , addr;
    uint16_t player_x, player_y;
   

    uint16_t p = (uint16_t) pmgMem;
    

    ///////////////////////////////////////////////////
    // INITIALIZE CHARS ///////////////////////////////
    // point to our charset.
    addr = (uint16_t) charset;
    OS.chbas = addr >> 8;

    memset( message_memory, 0, 4*40 );

    // setup characterset and playfield
    j = 33;
    k= 0;
    for( i=0; i < 20; ++i ) {
      memset( sline0 + k, j, 48 );
      k+=48;
      j++;
    }
    memset( sline0 + k,  33 + 128 , 48 );
    set_colors();

    // setup dlist
    delay_count = 2;
    delay = 1;
    init_dlist();
    horz_scroll = 0;

    init_interrupts();
    //ANTIC.nmien = 0xc0;  // activate BOTH display list ($80) and vertical blank $(40) interrupts
    ANTIC.nmien = 0x40;  // activate  vertical blank $(40) interrupts

    //// set pmbase with ANTIC
    ANTIC.pmbase = p >> 8;
    memset( pmgMem, 0, 2048 );
    OS.sdmctl = 62;    // POKE 559, 46 single line resolution
    GTIA_WRITE.gractl = 0x03; // POKE 53277,3 players and missile

    GTIA_WRITE.sizep0 = 0x00;
    player_x = 0x44;
    OS.pcolr0= 0x92;
    GTIA_WRITE.hposp0=player_x;
    memcpy( pmgMem + 1145, spr_0_frm_0, 16 );

    POKE( 0xD404, 15 ); // scrolling offset due to hscroll being on in Dlist  
                      //
    //GTIA_WRITE.hposm2 = 0x50;
    //GTIA_WRITE.hposm3 = 0x52 


    while(1) {
       waitvsync();
      putdec2( message_memory + 40, mwpx );
      putdec2( message_memory + 44, mwpy );
      puthex( message_memory + 50, lms1[0] );
      puthex( message_memory + 53, lms1[2] );
      puthex( message_memory + 55, lms1[1] );
    }
}
