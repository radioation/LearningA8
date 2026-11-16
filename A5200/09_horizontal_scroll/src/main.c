#include <atari.h>
#include <string.h>
#include <stdint.h>

#include "chars.h"


void init_dlist(void);
void init_interrupts(void);
void coarse_scroll_right(void);
void fine_scroll_right(void);

extern uint8_t screen_memory[];
extern uint8_t horz_scroll;
extern uint8_t delay_count;
extern uint8_t delay;


int main(void)
{
    ///////////////////////////////////////////////////
    // INITIALIZE CHARS ///////////////////////////////
    uint16_t i, offset, addr;
    
    addr = (uint16_t) charset;

    // setup characterset and playfield
    memcpy( screen_memory+256*9, tile_data, sizeof( tile_data ) );
    for( i = 0; i < 256; ++i ) {
        screen_memory[ 3840 + i ] =  i%2 ? 1 : 129;
        screen_memory[ 4096 + i ] =  i%2 ? 129 : 1;
        screen_memory[ 4352 + i ] =  i%2 ? 1 : 129;
        screen_memory[ 4608 + i ] =  i%2 ? 129 : 1;
    }    
    set_colors();
    OS.chbas = addr >> 8;

    // setup dlist
    delay_count = 2;
    delay = 1;
    init_dlist();
    horz_scroll = 0;

    init_interrupts();
    ANTIC.nmien = 0xc0;  // activate BOTH display list ($80) and vertical blank $(40) interrupts



    //GTIA_WRITE.hposm2 = 0x50;
    //GTIA_WRITE.hposm3 = 0x52 

    while(1) {
        //if ( i%10 == 0 ) {
        //    fine_scroll_right();
        //}

    }
}


