#ifndef _FS1LINE_H_
#define _FS1LINE_H_

// Line endpoints and color fill byte. They live in zero page;
extern unsigned char fs1_x1, fs1_y1, fs1_x2, fs1_y2, fs1_col, fs1_fb_hi;
#pragma zpsym ("fs1_x1")
#pragma zpsym ("fs1_y1")
#pragma zpsym ("fs1_x2")
#pragma zpsym ("fs1_y2")
#pragma zpsym ("fs1_col")
#pragma zpsym ("fs1_fb_hi")

void fs1_clear_0(void);               // clear the bitmap
void fs1_clear_1(void);               // clear the bitmap


void fs1_setcol(unsigned char c);   // color 0..3 
void fs1_plotl(void);               // draw fs1_x1,fs1_y1 -> fs1_x2,fs1_y2

// Convenience: set endpoints and draw. PLOTL may swap the endpoints,
//    so always set all four before each call (this macro does). 
#define fs1_line(ax, ay, bx, by) \
      do { fs1_x1 = (ax); fs1_y1 = (ay); fs1_x2 = (bx); fs1_y2 = (by); fs1_plotl(); } while (0)

#endif // _FS1LINE_H_

