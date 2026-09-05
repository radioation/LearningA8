# basics
* Created charset data with [Atari FontMaker](http://matosimi.websupport.sk/atari/atari-fontmaker/)
* used `#pragma rodata-name("CHARSET")`  to prevet overwrite/garbage
* used highbyte for `OS.chbas = addr >> 8`

* `setcolor` hues are in `_gtia.h`

```c

/* Luminance values go from 0 (black) to 7 (white) */

/* Hue values */
/* (These can vary depending on TV standard (NTSC vs PAL),
** tint potentiometer settings, TV tint settings, emulator palette, etc.)
*/
#define HUE_GREY        0
#define HUE_GOLD        1
#define HUE_GOLDORANGE  2
#define HUE_REDORANGE   3
#define HUE_ORANGE      4
#define HUE_MAGENTA     5
#define HUE_PURPLE      6
#define HUE_BLUE        7
#define HUE_BLUE2       8
#define HUE_CYAN        9
#define HUE_BLUEGREEN   10
#define HUE_BLUEGREEN2  11
#define HUE_GREEN       12
#define HUE_YELLOWGREEN 13
#define HUE_YELLOW      14
#define HUE_YELLOWRED   15

```

