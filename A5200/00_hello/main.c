
#include <conio.h>
#include <atari5200.h>



void main() {


    // https://cc65.github.io/doc/funcref.html#ss3.84
    //   clear the text screen.
    clrscr();

    // https://cc65.github.io/doc/funcref.html#ss3.38
    //   Set the background text color
    bgcolor( 2 );     // 0 through 14 (even) shades of grey.


    // https://cc65.github.io/doc/funcref.html#ss3.155
    //   Move the text mode cursor to a new position.
    gotoxy(10,1);

    // https://cc65.github.io/doc/funcref.html#cputs
    //   Output a string directly to the console
    cputs("Hello World!\r\n\r\n");



    // Loop forever
    while (1) {
    }
}

