#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


extern unsigned char __fastcall__ u_add8(unsigned char a, unsigned char b);
extern void __fastcall__ zero10( uint8_t* msg );
extern void __fastcall__ filler( const uint8_t val, const uint8_t count, uint8_t* msg );

void main(void)
{
    unsigned char i;
    unsigned char message[10];

    unsigned char a = 10;
    unsigned char b = 13;

    // Aseembly call with two char params
    unsigned char result = u_add8(a,b);
    cprintf("%u + %u = %u\r\n", a,b,result);

    // pointer call
    memset( message, 1, 10 );
    for( i=0; i < 10; ++i ) {
        cprintf("%d,", message[i] );
    }
    cprintf("\r\n");

    zero10( message );
    for( i=0; i < 10; ++i ) {
        cprintf("%d,", message[i] );
    }
    cprintf("\r\n");

    filler( 10, 5, message );

    for( i=0; i < 10; ++i ) {
        cprintf("%d,", message[i] );
    }
    cprintf("\r\n");

    while(1){
    }
}


