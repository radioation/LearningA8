
#include <conio.h>
#include <stdint.h>
#include <atari.h>
#include <stdint.h>


void init_interrupts(void);


extern uint8_t snd_cmd;
extern uint8_t snd_addr_lo;
extern uint8_t snd_addr_hi;
extern uint8_t snd_tick_count;
extern uint8_t dummeh;

#pragma zpsym("snd_cmd");
#pragma zpsym("snd_addr_lo");
#pragma zpsym("snd_addr_hi");
#pragma zpsym("snd_tick_count");
#pragma zpsym("dummeh");

extern uint8_t up_snd[];
extern uint8_t down_snd[];


void main() {

  uint16_t addr = 0; 
  uint8_t c = 0;
  clrscr();
  // position the cursor, output text
  gotoxy(0,1);

  // basic sound
  cputs("cc65 _sound() function\r\n");
  cgetc();
  _sound( 0, 100, 10, 8 );
  cgetc();
  _sound( 0, 0, 0, 0 );
  cgetc();


  // asssembly
  cputs("inline assembly sound\r\n");
  asm("lda #100");   // set note 
  asm("sta $D200");  // AUDF1
  // volume is the lower 4 bits.  high 3 bits are distortion control
  asm("lda #$A8");
  asm("sta $D201");  // AUDC1
  cgetc();
  asm("lda #$00");
  asm("sta $D201");  // AUDC1


  cputs("VBI Sound Test\r\n\r\nPress 1 or 2\r\n");
  init_interrupts();

  while (1) {
    c = cgetc(); 
    cprintf("You typed  %d!\r\n", c );

    // 49 == 1,  50 == 2   
    if( c == 49 ) {
      addr = (uint16_t)  &up_snd;
      snd_addr_lo = (unsigned char)(addr & 0xff);
      snd_addr_hi = (unsigned char)( addr >> 8);
      cprintf(" addr lo: %d hi: %d = %d \r\n", snd_addr_lo, snd_addr_hi, up_snd[0] );
      // 29 -> 41, 9d -> 157
      snd_tick_count = 1;
      snd_cmd = 1;  // NEW SOUND REQUIEST
    }else if ( c == 50 ) {
      addr = (uint16_t) &down_snd;
      snd_addr_lo = (unsigned char)(addr & 0xff);
      snd_addr_hi = (unsigned char)( addr >> 8);
      // 29 -> 41, aa -> 170
      cprintf(" addr lo: %d hi: %d = %d \r\n", snd_addr_lo, snd_addr_hi, down_snd[0] );
      snd_tick_count = 1;
      snd_cmd = 1;  // NEW SOUND REQUIEST
    }else if ( c == 32 ) {
      cprintf("  stored %d\r\n", dummeh );
    }
  }
}

