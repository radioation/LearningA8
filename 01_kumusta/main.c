
#include <conio.h>
#include <atari.h>
#include <string.h>
#include <stdio.h>


void main() {
    char message[40]; 
    unsigned char a;
    clrscr();
    // position the cursor
    gotoxy(10,1);

    // 
    cputs("Kumusta Diagdig!\r\n\r\n");

    cprintf("OS TYPE: %d\r\n", get_ostype() );
  
    a = get_tv(); 
    sprintf( message, "TV: %d", a );
    cprintf("%s\r\n", message );


    bgcolor( 0 );
    // textcolor( 15 ); no effect.

    cputs("Enter name: ");
    memset( message, 0, sizeof( message ) );
    cgets( message, sizeof(message) );
    cprintf("Hi %s. Press any key.\r\n", message ); 
     
    // Loop forever
    while (1) {
        if( kbhit() ) {
            a = cgetc();
            gotoxy(16, 10 );
            cprintf("Key: %d  ", a );
            if( a >= 48 && a <= 57 ) {
                bgcolor(a -48 );
            }

        }
    }
}

