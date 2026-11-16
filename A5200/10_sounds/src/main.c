
#include <conio.h>
#include <stdint.h>
#include <atari5200.h>


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



void delay_frames(uint8_t n) {
    uint8_t i;
    for (i = 0; i < n; ++i) {
       while (ANTIC.vcount >= 112);
       while (ANTIC.vcount < 112);
    }
}

// Use buttons instead of keyboard/cgetc() 
void wait_trigger(void) {
    while (GTIA_READ.trig0 == 0); // Wait for release 
    delay_frames(2); // debounce delay
    while (GTIA_READ.trig0 != 0); // Wait for press
    delay_frames(2);
}


void main() {

  uint16_t addr = 0; 
  //uint8_t c = 0;
   uint8_t curr_bottom, curr_top;
  uint8_t prev_bottom = 0, prev_top = 0;

    POKEY_WRITE.audctl = 0x00;

    POKEY_WRITE.skctl  = 0x02; // Keep keypad/top-button scan active (debounce OFF)
    GTIA_WRITE.consol  = 0x04; // Select Port 1 + pot power

  clrscr();
  gotoxy(0,1);

  // THERE IS NO  basic sound() function for A52
  //cputs("cc65 _sound() function\r\n");
  //cgetc();
  //_sound( 0, 100, 10, 8 );
  //cgetc();
  //_sound( 0, 0, 0, 0 );
  //cgetc();
    cputs("POKEY C REG SOUND\r\n");
    wait_trigger();
    POKEY_WRITE.audf1 = 100; // frequency
    POKEY_WRITE.audc1 = 0xA8; // Pure tone (10 is A in high nibble) + volume 8
    wait_trigger();
    POKEY_WRITE.audc1 = 0x00; // turn off.
    wait_trigger();


  // asssembly
  cputs("INLINE ASM SOUND\r\n");
  asm("lda #100");   // set note 
  asm("sta $E800");  // AUDF1 on 5200 is $E800 vs A8 $D200
  // volume is the lower 4 bits.  high 3 bits are distortion control
  asm("lda #$A8");
  asm("sta $D201");  // AUDC1 on 5200 is $E801 vs A8 $D201
    wait_trigger();
  asm("lda #$00");
  asm("sta $E801");  // AUDC1


  cputs("VBI SOUND TEST\r\n");
  cputs("PRESS TRIGGERS\r\n");
  init_interrupts();

  while (1) {
    delay_frames(1);

            curr_bottom = (GTIA_READ.trig0 == 0);
        curr_top = !(POKEY_READ.skstat & 0x08);

    if( curr_bottom && !prev_bottom ) {
      addr = (uint16_t)  &up_snd;
      snd_addr_lo = (unsigned char)(addr & 0xff);
      snd_addr_hi = (unsigned char)( addr >> 8);
      cprintf(" addr lo: %d hi: %d = %d \r\n", snd_addr_lo, snd_addr_hi, up_snd[0] );
      // 29 -> 41, 9d -> 157
      snd_tick_count = 1;
      snd_cmd = 1;  // NEW SOUND REQUIEST
    } else if( curr_top && !prev_top ) {
      addr = (uint16_t) &down_snd;
      snd_addr_lo = (unsigned char)(addr & 0xff);
      snd_addr_hi = (unsigned char)( addr >> 8);
      // 29 -> 41, aa -> 170
      cprintf(" addr lo: %d hi: %d = %d \r\n", snd_addr_lo, snd_addr_hi, down_snd[0] );
      snd_tick_count = 1;
      snd_cmd = 1;  // NEW SOUND REQUIEST
    }
    prev_top = curr_top;
    prev_bottom = curr_bottom;
  }
}

