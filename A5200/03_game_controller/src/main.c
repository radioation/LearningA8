#include <atari5200.h>
#include <stdint.h>
#include <conio.h>
#include <peekpoke.h>

// star raiders used 11 IIRC
// https://atarimuseum.ctrl-alt-rees.com/videogames/consoles/5200/conv_to_5200.html
// $11-$18   Shadows for POT0 through POT7
#define POT_SHADOW ((volatile uint8_t*)0x0011)
#define POKEY_SKCTL (*(volatile unsigned char*)0xE80F)

// https://www.atarimax.com/jindroush.atari.org/data/a5200/5200os.asm
#define POKMSK  0x00    // Shadow for POKEY IRQEN ($E80E)
#define VKPD    0x20A  //  Keypad routine continuation vector 

extern void init_keypad();
extern uint8_t last_key;

// Lookup table for 5200 keypad codes: index = (kbcode >> 1) & 0x0F
const char KEYMAP[16] = {
    '?', '#', '0', '*',   // 0x00 - 0x03
    'R', '9', '8', '7',   // 0x04 - 0x07 (R = Reset)
    'P', '6', '5', '4',   // 0x08 - 0x0B (P = Pause)
    'S', '3', '2', '1'    // 0x0C - 0x0F (S = Start)
};

void wait_vblank(void) {
    // Wait until scanline 240 (vcount == 120), after the 228-line pot charge finishes
    while (ANTIC.vcount != 120);
    while (ANTIC.vcount == 120);
}



int main(void)
{
    uint8_t pot0;
    uint8_t pot1;
    uint8_t pot2;
    uint8_t pot3;
    uint8_t trig0;
    uint8_t trig1;
    uint8_t top0;
    uint8_t top1;
    char key0;
    char key1;
    uint8_t tick = 0;

    clrscr();

    // set colors
    OS.color1 = 0x0F; // text
    OS.color2 = 0x98; // bg
    OS.color4 = 0x98;

    gotoxy(0,0);
    cputs("5200 Controler Test");

init_keypad();
///    // set consol
///    // bit 2 must be set 0x04
///    // which port is scanning (0x00 port 1)
///    GTIA_WRITE.consol = 0x04 | 0x00;  // port 1 will be next
///
///    // https://forums.atariage.com/topic/354064-code-for-reading-5200-joysticks/
///    // #1 mistake is writing %11 to the low bits of SKCTL. That's how you configure the keyboard for the computer, but the 5200 requires debounce to be off (%10).
///    POKEY_WRITE.skctl = 0x02; // Keyboard scan enabled, debounce disabled 
///                              //POKEY_SKCTL = 0x02; 
///
///    POKE(POKMSK, PEEK(POKMSK) | 0x40);
///    POKEY_WRITE.irqen = PEEK(POKMSK);
///
///    asm("cli");  // unmask

    while (1) {
        //  A52: DON'T potgo to start read
        //    POKEY_WRITE.potgo = 0;
        wait_vblank();

        //  pot0 = POKEY_READ.pot0;
        //  pot1 = POKEY_READ.pot1;
        //  pot2 = POKEY_READ.pot2;
        //  pot3 = POKEY_READ.pot3;

        pot0 = POT_SHADOW[0];
        pot1 = POT_SHADOW[1];
        pot2 = POT_SHADOW[2];
        pot3 = POT_SHADOW[3];

        // triggers Read bottom fire buttons (0 = pressed)
        trig0 = GTIA_READ.trig0;
        trig1 = GTIA_READ.trig1;

//        // alternate keypad reads
//        if ( (tick &1) == 0 ) {
            top0 = !(POKEY_READ.skstat & 0x08);
            key0 = last_key;

//            if ( ! (POKEY_READ.skstat & 0x04) )  {
//                key0 = KEYMAP[ ( POKEY_READ.kbcode >> 1 ) & 0x0F ];
//            } 
//
//
//            GTIA_WRITE.consol = 0x04 | 0x01; // port 2 will be next
//        } else {
//            top1 = !(POKEY_READ.skstat & 0x08);
//            GTIA_WRITE.consol = 0x04 | 0x00;  // port 1 will be next
//        } 
        tick +=1;


        // Display controller 1
        gotoxy(0, 3);
        cprintf("P1 X:%3u Y:%3u", pot0, pot1);
        gotoxy(0, 4);
        cprintf("   B:%c T:%c KEY:%c", trig0 ? '-' : '1', top0 ? '1' : '-', key0);

        // Display controler 2
        gotoxy(0, 7);
        cprintf("P2 X:%3u Y:%3u", pot2, pot3);
        gotoxy(0, 8);
        cprintf("   B:%c T:%c KEY:%c", trig1 ? '-' : '1', top1 ? '1' : '-', key1);





    }

    // nevah!
    return 0;
}
