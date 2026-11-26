#ifndef FS1LINE_H
#define FS1LINE_H

/* Line endpoints and color fill byte. They live in zero page;
 *    the pragmas tell cc65 so it uses fast zero-page addressing. */
extern unsigned char fs1_x1, fs1_y1, fs1_x2, fs1_y2, fs1_col;
#pragma zpsym ("fs1_x1")
#pragma zpsym ("fs1_y1")
#pragma zpsym ("fs1_x2")
#pragma zpsym ("fs1_y2")
#pragma zpsym ("fs1_col")

void fs1_init(void);                /* call once before fs1_clear */
void fs1_clear(void);               /* clear the bitmap */
void fs1_show(void);                /* switch the display to the bitmap */
void fs1_setcol(unsigned char c);   /* color 0..3 */
void fs1_plotl(void);               /* draw fs1_x1,fs1_y1 -> fs1_x2,fs1_y2 */

/* Convenience: set endpoints and draw. PLOTL may swap the endpoints,
 *    so always set all four before each call (this macro does). */
#define fs1_line(ax, ay, bx, by) \
      do { fs1_x1 = (ax); fs1_y1 = (ay); fs1_x2 = (bx); fs1_y2 = (by); fs1_plotl(); } while (0)

#endif
