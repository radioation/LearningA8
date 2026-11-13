
#include <conio.h>
#include <atari.h>
#include <string.h>
#include <stdio.h>

void main() {
    char message[40]; 
    unsigned char a;


    // https://cc65.github.io/doc/funcref.html#ss3.84
    //   clear the text screen.
    clrscr();

    // https://cc65.github.io/doc/funcref.html#ss3.38
    //   Set the background text color
    bgcolor( 2 );     // 0 through 14 (even) shades of grey.
    // https://cc65.github.io/doc/funcref.html#textcolor
    // textcolor( 15 ); // compiler wanrs Stement has no effect
    bordercolor( 5*16+2 ); // 

    // https://cc65.github.io/doc/funcref.html#ss3.155
    //   Move the text mode cursor to a new position.
    gotoxy(10,1);

    // https://cc65.github.io/doc/funcref.html#cputs
    //   Output a string directly to the console
    cputs("Hello World!\r\n\r\n");

    // https://cc65.github.io/doc/funcref.html#cprintf
    //   Formatted output to the console
    // https://cc65.github.io/doc/funcref.html#get_ostype
    //   The function returns the operating system, the program runs on.
    //   vlaues defined in atari/ostype.s
    cprintf("OS TYPE: %d\r\n", get_ostype() );
 
    // https://cc65.github.io/doc/funcref.html#ss3.138
    //   The function returns the system's vertical blank frequency 
    //
    //   Return TV_NTSC for 60Hz systems, TV_PAL for 50Hz systems, or TV_OTHER
    //   if the scan frequency can not be determined.  
    a = get_tv(); 
    sprintf( message, "GET TV: %d", a ); // 0 == TV_NTSC, 1 == TV_PAL
    cprintf("%s\r\n", message );



    cputs("Enter name: ");
    memset( message, 0, sizeof( message ) );
    // https://cc65.github.io/doc/funcref.html#cgets
    //   Input a string directly from the console.
    cgets( message, sizeof(message) );
    cprintf("Hi %s. Press any key.\r\n", message ); 
     
    // Loop forever
    while (1) {
        // https://cc65.github.io/doc/funcref.html#kbhit
        //   Check if there's a key waiting in the keyboard buffer.
        if( kbhit() ) {
            // https://cc65.github.io/doc/funcref.html#cgetc
            //   Read a character from the keyboard. (blocks/waits)
            a = cgetc();
            gotoxy(16, 10 );
            cprintf("Key: %d  ", a );
            if( a >= 48 && a <= 57 ) {
                bgcolor(a -48 );
            }

        }
    }
}

