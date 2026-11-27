#include <stdint.h>
#include "fs1line.h"

#define SCREEN_WIDTH   160
#define SCREEN_HEIGHT   80
#define LINES_PER_FRAME 60

#define RANDOM_SEED 0x5E6Au

static uint16_t rng_state = RANDOM_SEED;

// Diagnostic variables for inspection in Altirra.
volatile uint32_t test_line_count = 0;
volatile uint16_t test_frame_count = 0;

volatile uint8_t test_x1;
volatile uint8_t test_y1;
volatile uint8_t test_x2;
volatile uint8_t test_y2;


// 
// https://www.moria.us/articles/demystifying-the-lfsr/
static uint8_t random8()
{
    uint8_t bit = rng_state & 1;

    rng_state >>= 1;

    if (bit)
        rng_state ^= 0xB400u;

    return (uint8_t)rng_state;
}



uint8_t get_random_m(uint8_t limit)
{
    uint8_t result;

    do {
        result = random8();
    } while (result >= limit);

    return result;

}



//void render_random()
void stress_test()
{
    uint8_t i;

    rng_state = RANDOM_SEED;

    test_line_count = 0;
    test_frame_count = 0;

    while (1)
    {
        fs1_clear_0();
        for (i = 0; i < LINES_PER_FRAME; ++i)
        {
            // Generate valid ANTIC D coordinates.

            test_x1 = get_random_m(SCREEN_WIDTH);
            test_y1 = get_random_m(SCREEN_HEIGHT);

            test_x2 = get_random_m(SCREEN_WIDTH);
            test_y2 = get_random_m(SCREEN_HEIGHT);

            // Copy coordinates into FS1's working variables.
            fs1_setcol( i%3+1 );

            fs1_x1 = test_x1;
            fs1_y1 = test_y1;
            fs1_x2 = test_x2;
            fs1_y2 = test_y2;
            fs1_plotl();


            ++test_line_count;

        }


        ++test_frame_count;
    }
}
const uint8_t edge_cases[][4] = {
    {   0,  0, 159,  0 },  // Top edge
    {   0, 78, 159, 78 },  // Bottom edge
    {   0,  0,   0, 78 },  // Left edge
    { 159,  0, 159, 78 },  // Right edge

    {   0,  0, 159, 78 },  // Diagonal
    {   0, 78, 159,  0 },  // Opposite diagonal
    { 159, 78,   0,  0 },  // Reversed endpoints
    { 159,  0,   0, 78 },

    {  40, 40,  40, 40 },  // Single pixel, mask C0
    {  41, 40,  41, 40 },  // Single pixel, mask 30
    {  42, 40,  42, 40 },  // Single pixel, mask 0C
    {  43, 40,  43, 40 },  // Single pixel, mask 03

    {  40, 40,  41, 40 },  // Two adjacent pixels
    {  43, 40,  44, 40 },  // Cross byte boundary
    {  40, 40,  40, 41 },  // Short vertical
    {  40, 41,  40, 40 },  // Short vertical, reversed
    {  40, 40,  41, 41 },  // 45-degree line
    {  40, 41,  41, 40 }
};

void test_edge_cases()
{
    uint8_t i;

    for (i = 0; i < sizeof(edge_cases) / sizeof(edge_cases[0]); ++i)
    {
        test_x1 = fs1_x1 = edge_cases[i][0];
        test_y1 = fs1_y1 = edge_cases[i][1];
        test_x2 = fs1_x2 = edge_cases[i][2];
        test_y2 = fs1_y2 = edge_cases[i][3];

        ++test_line_count;
            fs1_plotl();

    }
}
