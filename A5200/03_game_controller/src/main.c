#include <atari5200.h>
#include <stdint.h>
#include <conio.h>




// Lookup table for 5200 keypad codes: index = (kbcode >> 1) & 0x0F
const char KEYMAP[16] = {
    '?', '#', '0', '*',   // 0x00 - 0x03
    'R', '9', '8', '7',   // 0x04 - 0x07 (R = Reset)
    'P', '6', '5', '4',   // 0x08 - 0x0B (P = Pause)
    'S', '3', '2', '1'    // 0x0C - 0x0F (S = Start)
};

void wait_frames(uint8_t f) {
    uint8_t i;
    for (i = 0; i < f; ++i) {
        uint8_t v = ANTIC.vcount;
        while (ANTIC.vcount == v);
        while (ANTIC.vcount != v);
    }
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

  clrscr();

  // set colors
  OS.color1 = 0x0F; // text
  OS.color2 = 0x98; // bg
  OS.color4 = 0x98;

  gotoxy(0,0);
  cputs("5200 Controler Test");

  while (1) {
    // potgo to start read?
        POKEY_WRITE.potgo = 0;
       wait_frames(1); // Wait 1 frame for RC capacitors + keypad scan

        pot0 = POKEY_READ.pot0;
        pot1 = POKEY_READ.pot1;
        pot2 = POKEY_READ.pot2;
        pot3 = POKEY_READ.pot3;

 // triggers Read bottom fire buttons (0 = pressed)
        trig0 = GTIA_READ.trig0;
        trig1 = GTIA_READ.trig1;

        GTIA_WRITE.consol = 0x04 | 0x00; // Select Port 1 (index 0) + Pot power
        wait_frames(1);
        top0 = !(POKEY_READ.skstat & 0x08);
        key0 = !(POKEY_READ.skstat & 0x04) ? KEYMAP[(POKEY_READ.kbcode >> 1) & 0x0F] : '-';

        GTIA_WRITE.consol = 0x04 | 0x01; // Select Port 2 (index 1) + Pot power
        wait_frames(1);
        top1 = !(POKEY_READ.skstat & 0x08);
        key1 = !(POKEY_READ.skstat & 0x04) ? KEYMAP[(POKEY_READ.kbcode >> 1) & 0x0F] : '-';



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




//
//    gotoxy( 0, 21);
//    cprintf( "PD0:%3u PD1:%3u", pot0, pot1);
//    gotoxy( 0, 22);
//    cprintf( "PT0:%3u PT1:%3u", ptrig0, ptrig1);
//    gotoxy( 24, 20);
//    cprintf( "PD2:%3u PD3:%3u", pot2, pot3);
//    gotoxy( 24, 22);
//    cprintf( "PT2:%3u PT3:%3u", ptrig2, ptrig3);
//
//    // sticks
//    stick0 = PEEK(STICK0_ADDR);
//    gotoxy( 9,10 );
//    cputc('U'+ ( ~stick0 & 0x01 ?  128 : 0));
//    gotoxy( 9,14 );                      
//    cputc('D'+ ( ~stick0 & 0x02 ?  128 : 0));
//    gotoxy( 7,12 );                      
//    cputc('L'+ ( ~stick0 & 0x04 ?  128 : 0));
//    gotoxy( 11,12 );                     
//    cputc('R'+ ( ~stick0 & 0x08 ?  128 : 0));
//    gotoxy( 13,10 );
//    cputc('F'+ (PEEK(STRIG0_ADDR) ? 0 : 128));
//    // sticks
//    stick1 = PEEK(STICK1_ADDR);
//   // stick1 = OS.stick1;
//    gotoxy( 29,10 );
//    cputc('U'+ ( ~stick1 & 0x01 ?  128 : 0));
//    gotoxy( 29,14 );
//    cputc('D'+ ( ~stick1 & 0x02 ?  128 : 0));
//    gotoxy( 27,12 );
//    cputc('L'+ ( ~stick1 & 0x04 ?  128 : 0));
//    gotoxy( 31,12 );
//    cputc('R'+ ( ~stick1 & 0x08 ?  128 : 0));
//    gotoxy( 33,10 );
//    //cputc('F'+ (PEEK(STRIG1_ADDR) ? 0 : 128));
//    cputc('F'+ (OS.strig1 ? 0 : 128));
  }

  // nevah!
  return 0;
}
