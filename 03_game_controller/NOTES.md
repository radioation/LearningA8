# Controller input
* Atari ports are simple. Two pins for potentiometers, 5 for switches.
* potentiometers affect charge time for internal capcitors. Atari gives 
  us 0-228 levels.
* switches pull pins to ground. When pressed we see '0' for corresponding bits
  bytes.
```c
    stick0 = PEEK(STICK0_ADDR);
    gotoxy( 9,10 );
    cputc('U'+ ( ~stick0 & 0x01 ?  128 : 0));
    gotoxy( 9,14 );                      
    cputc('D'+ ( ~stick0 & 0x02 ?  128 : 0));
    gotoxy( 7,12 );                      
    cputc('L'+ ( ~stick0 & 0x04 ?  128 : 0));
    gotoxy( 11,12 );                     
    cputc('R'+ ( ~stick0 & 0x08 ?  128 : 0));
```
* paddle triggers are using the same pins as L/R joystick switches.

* cc65 gives us strig[0-4], stick[0-4], paddl[0-8], and ptrig[0-7]
  but it's just as easy to peek into their memory locations.


