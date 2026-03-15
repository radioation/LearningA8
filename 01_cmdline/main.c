// cl65 -t atari -O main.c -o 01_helloarg.xex


#include <stdio.h>
#include <string.h>
#include <atari.h>
#include <conio.h>


static void print_args(int argc, char* argv[])
{
  int i;

  cprintf("argc = %d\r\n", argc);

  for (i = 0; i < argc; ++i) {
    cprintf(" argv[%d] = \"%s\"\r\n", i, argv[i]);
  }
}



int main(int argc, char* argv[])
{
  const char* name = "Usami Fuji";
  clrscr();

  cprintf("cc65 Atari argv[] example\r\n");
  cprintf("-------------------------\r\n");


  if (_is_cmdline_dos()) {
    cprintf("DOS supports command lines.\r\n");
  } else {
    cprintf("DOS does NOT support command lines.\r\n");
    cprintf("Run with SpartaDOS or variant.\n");
    cprintf("\r\nPress any key to exit.\r\n");
    cgetc();
    return 1;
  }

  cprintf("\r\n");

  // argv[0] is the program name 
  if (argc > 1 && argv[1] != 0 && argv[1][0] != '\0') {
    name = argv[1];
  }

  cprintf("Hello %s!\r\n\r\n", name);

  print_args(argc, argv);

  cprintf("\r\nPress any key to exit\r\n");
  cgetc();

  return 0;
}
