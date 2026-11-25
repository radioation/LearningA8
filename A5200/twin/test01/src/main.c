#include <atari5200.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include <peekpoke.h>

//#define SHOW_VALS
#define SHOW_FPS

extern uint8_t zp_vel_lo;
extern uint8_t zp_vel_hi;
extern uint8_t zp_lut_index;
extern uint8_t zp_missile_index;

#pragma zpsym("zp_vel_lo");
#pragma zpsym("zp_vel_hi");
#pragma zpsym("zp_lut_index");
#pragma zpsym("zp_missile_index");

extern uint8_t missile_x_lo[];
extern uint8_t missile_x_hi[];
extern uint8_t missile_y_lo[];
extern uint8_t missile_y_hi[];


//extern uint8_t x_dir_lut[256];
//extern uint8_t y_dir_lut[256];
extern uint8_t x_dir_lut_lo[256];
extern uint8_t x_dir_lut_hi[256];
extern uint8_t y_dir_lut_lo[256];
extern uint8_t y_dir_lut_hi[256];


extern void move_m0();


#pragma bss-name (push, "MYPMG")
unsigned char pmg_memory[ 2048 ]; // Double line resolution only needs 1024 bytes, single need 2048
#pragma bss-name (pop)

// player data from https://bocianu.gitlab.io/spred/
// sprite 0
unsigned char height = 0x08;
unsigned char frames0_0[0x08] = {
    0x3c, 0x66, 0xdb, 0xbd, 0xbd, 0xdb, 0x66, 0x3c
};


// star raiders used 11 IIRC
// https://atarimuseum.ctrl-alt-rees.com/videogames/consoles/5200/conv_to_5200.html
// $11-$18   Shadows for POT0 through POT7
#define POT_SHADOW ((volatile uint8_t*)0x0011)
#define POKEY_SKCTL (*(volatile unsigned char*)0xE80F)

// https://www.atarimax.com/jindroush.atari.org/data/a5200/5200os.asm
#define POKMSK  0x00    // Shadow for POKEY IRQEN ($E80E)
#define VKPD    0x20A  //  Keypad routine continuation vector 

extern void init_keypad();
extern uint8_t last_key;

// Lookup table for 5200 keypad codes: index = (kbcode >> 1) & 0x0F
const char KEYMAP[16] = {
    '0', '1', '2', '3', 
    '4', '5', '6', '7',
    '8', '9', '*', '#', 
    'S', 'P', 'R', '-'  
};

void wait_vblank(void) {
    // Wait until scanline 240 (vcount == 120), after the 228-line pot charge finishes
    while (ANTIC.vcount != 120);
    while (ANTIC.vcount == 120);
}



int main(void)
{

    uint8_t start_tick;
    uint8_t frames;
    uint8_t fps;

    uint8_t pot0;
    uint8_t pot1;
    uint8_t pot2;
    uint8_t pot3;
    uint8_t trig0;
    uint8_t trig1;
    uint8_t top0;
    uint8_t top1;
    uint8_t x_lut;
    uint8_t y_lut;
    char key0;
    char key1;
    uint8_t tick = 0;
    uint16_t p = (uint16_t) &pmg_memory;
    int8_t mid = 114;
    int8_t xdir1 = 0;
    int8_t ydir1 = 0;
    int8_t xdir2 = 0;
    int8_t ydir2 = 0;
    uint16_t m0_x = 0;
    uint16_t m0_y = 0;
    uint16_t p0_x = (100 << 8 );
    uint16_t p0_y = (100 << 8 );;

    clrscr();

    // set colors
    OS.color1 = 0x0F; // text
    OS.color2 = 0x98; // bg
    OS.color4 = 0x98;

    gotoxy(0,0);
    cputs("5200 Controler Test");

    ///    // set consol
    ///    // bit 2 must be set 0x04
    ///    // which port is scanning (0x00 port 1)
    ///    GTIA_WRITE.consol = 0x04 | 0x00;  // port 1 will be next
    ///
    ///    // https://forums.atariage.com/topic/354064-code-for-reading-5200-joysticks/
    ///    // #1 mistake is writing %11 to the low bits of SKCTL. That's how you configure the keyboard for the computer, but the 5200 requires debounce to be off (%10).
    ///    POKEY_WRITE.skctl = 0x02; // Keyboard scan enabled, debounce disabled 
    ///                              //POKEY_SKCTL = 0x02; 
    ///
    ///    POKE(POKMSK, PEEK(POKMSK) | 0x40);
    ///    POKEY_WRITE.irqen = PEEK(POKMSK);
    ///
    ///    asm("cli");  // unmask
    init_keypad();


    ///////////////////////////////////////////////////
    // INITIALIZE PMG /////////////////////////////////
    // set pmbase page with ANTIC
    ANTIC.pmbase = p >> 8; // >> for page
    memset( pmg_memory, 0, sizeof(pmg_memory) );

    // set DMACTL
    OS.sdmctl = 62;    // POKE 559, 46 double line resolution 62 single

    // set GRACTL
    GTIA_WRITE.gractl = 0x03;   
    // widths
    GTIA_WRITE.sizep0 = 0x00;

    OS.pcolr0= 0x2E;
    // OS.gprior = 49; // 32 overlap has color, 1 players over playfield, 16 combine
    GTIA_WRITE.prior = 17; // no shadow register for the OS?

    //memset( pmg_memory+768, 255, 256); 
    // horizontal positions
    GTIA_WRITE.hposp0=(uint8_t)( p0_x >> 8 );
    //GTIA_WRITE.hposm0= (uint8_t)( p0_x >> 8 );

    // Copy shapes to PMG array
    //memcpy( pmg_memory + 768 + 30 , mplayer, 5 );
    memcpy( pmg_memory + 1024 + (uint8_t)( p0_y >> 8 ), frames0_0, height );

    frames = 0;
    fps = 0;
    wait_vblank();
    start_tick = OS.rtclok[1];
    while (1) {
        //  A52: DON'T potgo to start read
        //    POKEY_WRITE.potgo = 0;
        wait_vblank();
#ifdef SHOW_FPS
        ++frames;
        if ((uint8_t)(OS.rtclok[1] - start_tick) >= 60) {
            fps = frames;
            frames = 0;
            start_tick += 60;
            gotoxy(0, 1);
            cprintf("FPS:%d   ", fps);

        }
#endif
        GTIA_WRITE.hposp0=(uint8_t)( p0_x >> 8 );
        memset( pmg_memory + 1020 + (uint8_t)(p0_y >> 8 ), 0, 16 ); // wasteful clear.
        memcpy( pmg_memory + 1024 + (uint8_t)( p0_y >> 8 ), frames0_0, height );
        GTIA_WRITE.hposm0= (uint8_t)( missile_x_hi[0]  );
        memset( pmg_memory + 762 + (uint8_t)(missile_y_hi[0] ), 0, 13 ); // wasteful clear.
        pmg_memory [ 768 + (uint8_t)(missile_y_hi[0] )] = 0b0000011;
        pmg_memory [ 769 + (uint8_t)(missile_y_hi[0] )] = 0b0000011;
        //        // alternate keypad reads
        if ( (tick &1) == 0 ) {
            top0 = !(POKEY_READ.skstat & 0x08);
            key0 = KEYMAP[  last_key  & 0x0F];
            last_key = 0xFF; // CLEAR IT OUT!!! 
            GTIA_WRITE.consol = 0x04 | 0x01; // port 2 will be next
        } else {
            top1 = !(POKEY_READ.skstat & 0x08);
            key1 = KEYMAP[  last_key  & 0x0F];
            last_key = 0xFF; // CLEAR IT OUT!!! 
            GTIA_WRITE.consol = 0x04 | 0x00;  // port 1 will be next
        } 
        tick++;

        //  pot0 = POKEY_READ.pot0;
        //  pot1 = POKEY_READ.pot1;
        //  pot2 = POKEY_READ.pot2;
        //  pot3 = POKEY_READ.pot3;

        pot0 = POT_SHADOW[0];
        pot1 = POT_SHADOW[1];
        pot2 = POT_SHADOW[2];
        pot3 = POT_SHADOW[3];

        // triggers Read bottom fire buttons (0 = pressed)
        trig0 = GTIA_READ.trig0;
        trig1 = GTIA_READ.trig1;


        xdir1 = (int8_t)pot0 - (int8_t)mid;
        ydir1 = (int8_t)pot1 - (int8_t)mid;
        xdir2 = (int8_t)pot2 - (int8_t)mid;
        ydir2 = (int8_t)pot3 - (int8_t)mid;

        x_lut = pot2 >> 4;
        y_lut = pot3 >> 4;

#ifdef SHOW_VALS
        // Display controller 1
        gotoxy(0, 3);
        cprintf("P1 X:%3u Y:%3u", pot0, pot1);
        gotoxy(0, 4);
        cprintf("   B:%c T:%c KEY:%c", trig0 ? '-' : '1', top0 ? '1' : '-', key0);

        //  Display controler 2
        gotoxy(0, 7);
        cprintf("P2 X:%3u Y:%3u", pot2, pot3);
        gotoxy(0, 8);
        cprintf("   B:%c T:%c KEY:%c", trig1 ? '-' : '1', top1 ? '1' : '-', key1);
        gotoxy(0, 10);
        cprintf("xd1:%d yd1: %d   ", xdir1, ydir1 );
        gotoxy(0, 11);
        cprintf("xd2:%d yd2: %d   ", xdir2, ydir2 );


        gotoxy(0, 12);
        cprintf("xlut:%d ylut: %d   ", x_lut, y_lut );

        gotoxy(0, 13);
        cprintf(" xh:%d yh: %d  ", (int8_t)x_dir_lut_hi[zp_lut_index], (int8_t) y_dir_lut_hi[ zp_lut_index] );
#endif

        if( xdir1 > 12 || xdir1 < -12 ) {
            p0_x +=  ( xdir1 << 3 ) ;            
        }
        if( ydir1 > 12 || ydir1 < -12 ) {
            p0_y +=  ( ydir1 << 3 ) ;            
        }





        // m0_y > 0 means missile moving
        if( !trig0  && missile_y_hi[0] == 0 ) {
            zp_lut_index = x_lut | ( y_lut <<4 );
            if( x_dir_lut_lo[ zp_lut_index ] != 0  ||
                    x_dir_lut_hi[ zp_lut_index ] != 0  ||
                    y_dir_lut_lo[ zp_lut_index ] != 0  ||
                    y_dir_lut_hi[ zp_lut_index ] != 0  ) {


                missile_y_hi[0] = (p0_y >>8 ) +3;
                missile_y_lo[0] = 0;
                missile_x_hi[0] = (p0_x >> 8 ) +3;
                missile_x_lo[0] = 0;
            }
        }
        if( missile_y_hi[0] != 0 ) {
            //gotoxy(0, 14);
            //cprintf(" m0 x:%d y: %d   ", missile_x_hi[0], missile_y_hi[0] );
            //pmg_memory [ 768 + (uint8_t)(missile_y_hi[0] )] = 0b0000000;
            //pmg_memory [ 769 + (uint8_t)(missile_y_hi[0] )] = 0b0000000;
            move_m0();
            // visible bounds X: 48 to 207  Y: 32 to 223
            if( missile_x_hi[0]  < 48 || 
                    missile_x_hi[0] > 207 ||
                    missile_y_hi[0] < 32 ||
                    missile_y_hi[0] > 223 )
            {
                memset( pmg_memory + 762 + (uint8_t)(missile_y_hi[0] ), 0, 13 ); // wasteful clear.
                missile_y_hi[0] = 0;
            }

        }



    }

    // nevah!
    return 0;
}
