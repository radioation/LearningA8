#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


extern unsigned char __fastcall__ u_add8(unsigned char a, unsigned char b);
extern void __fastcall__ clear10( uint8_t* msg );
extern void __fastcall__ filler( const uint8_t val, const uint8_t count, uint8_t* msg );


extern void zfiller( void );

extern char zval;
#pragma zpsym("zval");
extern char zcount;
#pragma zpsym("zcount");
extern uint8_t* zptr;
#pragma zpsym("zptr");


void main(void)
{
    unsigned char i;
    unsigned char message[10];

    unsigned char a = 10;
    unsigned char b = 13;

    // Aseembly call with two char params
    unsigned char result = u_add8(a,b);
    cprintf("%u + %u = %u\r\n", a,b,result);


    cputs("Message data\r\n");
    memset( message, 1, 10 );
    for( i=0; i < 10; ++i ) {
        cprintf("%d,", message[i] );
    }
    cprintf("\r\n");

    cputs("Message data after clear10()\r\n");
    // Call assembly with pointer parameter
    clear10( message );
    for( i=0; i < 10; ++i ) {
        cprintf("%d,", message[i] );
    }
    cprintf("\r\n");


    cputs("Message data after filler()\r\n");
    // Call assembly with pointer parameter
    filler( 10, 5, message );

    for( i=0; i < 10; ++i ) {
        cprintf("%d,", message[i] );
    }
    cprintf("\r\n");



    // Call assembly and use Zero Page to pass values
    cputs("Message data after ZERO PAGE zfiller()\r\n");
    zptr = (uint8_t*)message;
    zval = 20;
    zcount = 7;
    zfiller();
    for( i=0; i < 10; ++i ) {
        cprintf("%d,", message[i] );
    }
    cprintf("\r\n");

    // inline assembly
    asm("lda #$FF");
    asm("sta $02C5"); // text  - COLOR1
    asm("lda #$30");
    asm("sta $02C6"); // background - COLOR2

    while(1){
    }
}


