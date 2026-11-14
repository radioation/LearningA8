#include <atari.h>
#include <string.h>
#include <stdint.h>
#include <peekpoke.h>
#include <conio.h>


// https://www.atariarchives.org/mapping/appendix2.php
//  624-631 PADDL#  ( ONLY 4 work XL/XE )
//  632-635 STICK# 
//  636-639 PTRIG#  ( only 4 XL/XE )
//  644-647 STRIG#
#define PADDL0_ADDR 624
#define PADDL1_ADDR 625
#define PADDL2_ADDR 626
#define PADDL3_ADDR 627

#define STICK0_ADDR 632
#define STICK1_ADDR 633

#define PTRIG0_ADDR 636
#define PTRIG1_ADDR 637
#define PTRIG2_ADDR 638
#define PTRIG3_ADDR 639 


#define STRIG0_ADDR 644
#define STRIG1_ADDR 645


#define CH_ADDR     764



int main(void)
{
  uint8_t paddle0;
  uint8_t paddle1;
  uint8_t paddle2;
  uint8_t paddle3;
  uint8_t ptrig0;
  uint8_t ptrig1;
  uint8_t ptrig2;
  uint8_t ptrig3;


  uint8_t stick0;
  uint8_t stick1;
  //uint8_t strig0;
  //uint8_t strig1;
  clrscr();

  // set colors
  OS.color1 = 0x0F; // text
  OS.color2 = 0x98; // bg
  OS.color4 = 0x98;


  POKE(CH_ADDR, 255);

  gotoxy(0,0);
  cprintf("Joystick Paddle Input");

  while (1) {
    // paddles
    paddle0 = OS.paddl0; // cc65 equivalent
    //paddle0 = PEEK(PADDL0_ADDR);
    paddle1 = PEEK(PADDL1_ADDR);
    paddle2 = PEEK(PADDL2_ADDR);
    paddle3 = PEEK(PADDL3_ADDR);

    ptrig0 = PEEK( PTRIG0_ADDR );
    ptrig1 = PEEK( PTRIG1_ADDR );
    ptrig2 = PEEK( PTRIG2_ADDR );
    ptrig3 = PEEK( PTRIG3_ADDR );

    gotoxy( 0, 21);
    cprintf( "PD0:%3u PD1:%3u", paddle0, paddle1);
    gotoxy( 0, 22);
    cprintf( "PT0:%3u PT1:%3u", ptrig0, ptrig1);
    gotoxy( 24, 20);
    cprintf( "PD2:%3u PD3:%3u", paddle2, paddle3);
    gotoxy( 24, 22);
    cprintf( "PT2:%3u PT3:%3u", ptrig2, ptrig3);

    // sticks
    stick0 = PEEK(STICK0_ADDR);
    gotoxy( 9,10 );
    cputc('U'+ ( ~stick0 & 0x01 ?  128 : 0));
    gotoxy( 9,14 );                      
    cputc('D'+ ( ~stick0 & 0x02 ?  128 : 0));
    gotoxy( 7,12 );                      
    cputc('L'+ ( ~stick0 & 0x04 ?  128 : 0));
    gotoxy( 11,12 );                     
    cputc('R'+ ( ~stick0 & 0x08 ?  128 : 0));
    gotoxy( 13,10 );
    cputc('F'+ (PEEK(STRIG0_ADDR) ? 0 : 128));
    // sticks
    stick1 = PEEK(STICK1_ADDR);
   // stick1 = OS.stick1;
    gotoxy( 29,10 );
    cputc('U'+ ( ~stick1 & 0x01 ?  128 : 0));
    gotoxy( 29,14 );
    cputc('D'+ ( ~stick1 & 0x02 ?  128 : 0));
    gotoxy( 27,12 );
    cputc('L'+ ( ~stick1 & 0x04 ?  128 : 0));
    gotoxy( 31,12 );
    cputc('R'+ ( ~stick1 & 0x08 ?  128 : 0));
    gotoxy( 33,10 );
    //cputc('F'+ (PEEK(STRIG1_ADDR) ? 0 : 128));
    cputc('F'+ (OS.strig1 ? 0 : 128));
  }

  // nevah!
  return 0;
}
