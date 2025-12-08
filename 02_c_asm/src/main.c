#include <conio.h>
#include <stdio.h>



unsigned char __fastcall__ u_add8(unsigned char a, unsigned char b);


void main(void)
{
    unsigned char a = 10;
    unsigned char b = 13;
    unsigned char result = u_add8(a,b);

    cprintf("%u + %u = %u\r\n", a,b,result);

    while(1){
    }
}


