#include <stdint.h>
                         
#define MWP_COLS 48
#define MWP_ROWS 20  // not including sline0
#define MWP_NUM_TILES ( MWP_COLS * MWP_ROWS )
#define MWP_LAST_ROW  912    // assumes screen_memory is the start,

#define CHUNK_SHIFT 5     /// 32 metatiles = 1 <<5
#define CHUNK_TOTAL 6
#define CHUNK_SIZE 32
#define CMASK 31
enum { CHUNK_EMPTY, CHUNK_PENDING, CHUNK_READY };

typedef struct {
  uint16_t chunk_x;   // chunk coordinates?
  uint16_t chunk_y;
  uint8_t state;
  uint8_t *data; 
} chunk_data;

static chunk_data chunks[CHUNK_TOTAL];
static chunk_data *last_chunk;

extern uint8_t sline0[];   // mwp start in memory, I do wnat to draw here
extern uint8_t screen_memory[];   // mwp start in memory, I do wnat to draw here
extern uint8_t chunk_0_0[];
extern uint8_t chunk_32_0[];
extern uint8_t chunk_64_0[];
extern uint8_t chunk_0_32[];
extern uint8_t chunk_32_32[];
extern uint8_t chunk_64_32[];



const uint8_t meta_top_left[256] = 
{ 2, 27, 187, 141, 10,  1,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, };
const uint8_t meta_top_right[] = 
{ 3, 28, 188, 140, 11, 1,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, };
const uint8_t meta_bottom_left[]= 
{ 2, 29, 189, 141, 12, 1,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, };
const uint8_t meta_bottom_right[] =
{ 3, 30, 190, 140, 13, 1,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, };


#define MWP_SET( o, t )  do { uint8_t _t = (t);          \
                            screen_memory[o] = _t;                    \
                            if ((o) >= MWP_LAST_ROW) sline0[(o)-MWP_LAST_ROW ] = _t; } while (0)

void init_chunks() {
 chunks[0].chunk_x = 0;
 chunks[0].chunk_y = 0;
 chunks[0].state = CHUNK_READY;
 chunks[0].data = chunk_0_0;

 chunks[1].chunk_x = 1;
 chunks[1].chunk_y = 0;
 chunks[1].state = CHUNK_READY;
 chunks[1].data = chunk_32_0;

 chunks[2].chunk_x = 2;
 chunks[2].chunk_y = 0;
 chunks[2].state = CHUNK_READY;
 chunks[2].data = chunk_64_0;

 chunks[3].chunk_x = 0;
 chunks[3].chunk_y = 1;
 chunks[3].state = CHUNK_READY;
 chunks[3].data = chunk_0_32;

 chunks[4].chunk_x = 1;
 chunks[4].chunk_y = 1;
 chunks[4].state = CHUNK_READY;
 chunks[4].data = chunk_32_32;

 chunks[5].chunk_x = 2;
 chunks[5].chunk_y = 1;
 chunks[5].state = CHUNK_READY;
 chunks[5].data = chunk_64_32;

}





// TODO: when we get a chunk loader from disk or fujinet/network
//    we need to rset last_chunk to 0 if a chunk[] is loaded new.
static const uint8_t *find_chunk(uint16_t chunk_x, uint16_t chunk_y)
{
    uint8_t  i;

    if (last_chunk && last_chunk->chunk_x == chunk_x && last_chunk->chunk_y == chunk_y) {
      return last_chunk->data;
    }
    for (i = 0; i < CHUNK_TOTAL; ++i) {
        chunk_data *s = &chunks[i];
        if (s->state == CHUNK_READY && s->chunk_x == chunk_x && s->chunk_y == chunk_y) {
            last_chunk = s; 
            return s->data;
        }
    }
    return 0;     
}



// metatile byte 
uint8_t meta_tile_get(uint16_t mx, uint16_t my)   
{
    const uint8_t *c = find_chunk(mx >> CHUNK_SHIFT, my >> CHUNK_SHIFT);
    return c ? c[((my & CMASK) << CHUNK_SHIFT) | (mx & CMASK)] : 0;
}

// slow map lookup one char.
uint8_t map_get(uint16_t wx, uint16_t wy)     /* character */
{
      uint8_t m = meta_tile_get(wx >> 1, wy >> 1);
    if (wy & 1) {
      return (wx & 1) ? meta_bottom_right[m] : meta_bottom_left[m];
    }
    else{
      return (wx & 1) ? meta_top_right[m] : meta_top_left[m];
    }
}



// WORLD x and WORLD Y are in actual tile steps (so double the metatiles)
// fill COLS chars of world column wx starting at world row wy,
//   into the MWP buffer starting at offset o.  (o and wx, wy are same unit.
//   Meta tiles are half the world/offste unit)
//
//   returns 0 if a chunk isn't ready 
uint8_t fill_column(uint16_t wx, uint16_t wy, uint16_t o)
{
    // we're dealing with half of the metatile:
    const uint8_t *top = (wx & 1) ? meta_top_right : meta_top_left;
    const uint8_t *bot = (wx & 1) ? meta_bottom_right : meta_bottom_left;

    // divide world by 2 for meta tile lookup.
    uint16_t mx = wx >> 1;
    uint8_t n = MWP_ROWS;

    while (n) {
        uint16_t my = wy >> 1; // divide world y by 2 for meta-tile

        // divide meta position by 32 to get right chuunk
        const uint8_t *p = find_chunk(mx >> CHUNK_SHIFT, my >> CHUNK_SHIFT);
        uint8_t left = 32 - (my & CMASK);  // metatile rows left in chunk 
        if (!p) {
            return 0;
        }

        p += ((my & CMASK) << CHUNK_SHIFT) | (mx & CMASK);

        if (wy & 1) {                    // starting on a bottom half 
            MWP_SET(o, bot[*p]);
            o += MWP_COLS; if (o >= MWP_NUM_TILES) o -= MWP_NUM_TILES;
            ++wy; --n; p += 32; --left;
        }
        while (n && left) {
            uint8_t m = *p;
            MWP_SET(o, top[m]);
            o += MWP_COLS; if (o >= MWP_NUM_TILES) o -= MWP_NUM_TILES;
            ++wy; if (!--n) break;
            MWP_SET(o, bot[m]);
            o += MWP_COLS; if (o >= MWP_NUM_TILES) o -= MWP_NUM_TILES;
            ++wy; --n; p += 32; --left;
        }
    }
    return 1;
}


// WORLD x and WORLD Y are in actual tile steps (so double the metatiles)
uint8_t fill_row(uint16_t wx, uint16_t wy, int16_t o)
{
    // for each world coord we're dealing with half of the metatile:
    const uint8_t *a = (wy & 1) ? meta_bottom_left : meta_top_left;   
    const uint8_t *b = (wy & 1) ? meta_bottom_right : meta_top_right;   

    // divide world y by 2 to get meta-tile coordinate
    unsigned my = wy >> 1;
    uint8_t n = MWP_COLS;

    while (n) {
        // divide world x by 2 
        unsigned mx = wx >> 1;
        const uint8_t *p = find_chunk(mx >> CHUNK_SHIFT, my >> CHUNK_SHIFT);
        uint8_t left = 32 - (mx & CMASK);  // metatiles left in chunk row
        if (!p) return 0;
        p += ((my & CMASK) << CHUNK_SHIFT ) | (mx & CMASK);

        if (wx & 1) {                    // starting on a right half 
            MWP_SET(o, b[*p]);
            if (++o == MWP_NUM_TILES) o = 0;
            ++wx; --n; ++p; --left;
        }
        while (n && left) {
            uint8_t m = *p++;
            MWP_SET(o, a[m]);
            if (++o == MWP_NUM_TILES) o = 0;
            ++wx; if (!--n) break;
            MWP_SET(o, b[m]);
            if (++o == MWP_NUM_TILES) o = 0;
            ++wx; --n; --left;
        }
    }
    return 1;
}

