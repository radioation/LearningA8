#include <atari.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <peekpoke.h>

#include  <atari_screen_charmap.h>    // Pesky Atari mapping.

void init_dlist(void);
void init_interrupts(void);
extern uint8_t img_data[];
extern uint8_t text_memory[];



int main(void)
{
    char msg[] = "Atari Computers!";
    uint8_t i,j,count,type;

    memset( text_memory, 0, 80 ); 
    memcpy( text_memory + 52, msg, 16 );
    /*
    _setcolor(0, 0, 12);
    _setcolor(1, 0, 8);
    _setcolor(2, 0, 4);
    _setcolor(3, 2, 10);
*/

    
    
    init_dlist();

    init_interrupts();
    ANTIC.nmien = 0xc0;  // activate BOTH display list ($80) and vertical blank $(40) interrupts
    
    while(1) {

    }

}


