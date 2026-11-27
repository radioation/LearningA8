#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <stdint.h>

void init_dlist(void);


#define FRAME_BUFFER_0 ((uint8_t *)0x2000)
#define FRAME_BUFFER_1 ((uint8_t *)0x3000)
#define PMG   ((uint8_t *)0x1800)



#endif // _DISPLAY_H_
