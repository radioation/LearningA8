#include <atari5200.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <peekpoke.h>

#include  <atari_screen_charmap.h>    // Pesky Atari mapping.

void init_dlist(void);
void init_interrupts(void);
extern uint8_t img_data[];
extern uint8_t img_data_rom[];
extern uint8_t text_memory[];



int main(void)
{
    char msg[] = "Atari 5200!";
    uint8_t i,j,count,type;

    memset( text_memory, 0, 80 ); 
    memcpy( text_memory + 55, msg, 16 );
    memcpy( img_data, img_data_rom, 7040 );    
    
    init_dlist();

    init_interrupts();
    ANTIC.nmien = 0xc0;  // activate BOTH display list ($80) and vertical blank $(40) interrupts
    
    while(1) {

    }

}


