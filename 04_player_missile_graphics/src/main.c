#include <atari.h>
#include <string.h>
#include <stdint.h>


#pragma data-name( push, "MYPMG" )
unsigned char pmg_memory[ 2048 ]; // Double line resolution only needs 1024 bytes, single need 2048
#pragma data-name( pop );


// sprite data from https://bocianu.gitlab.io/spred/
const unsigned char height = 0x0e;

/*
   unsigned char colors0_0[0x0e] = {
   0x08
   };unsigned char colors1_0[0x0e] = {
   0x32
   };unsigned char colors2_0[0x0e] = {
   0x08
   };unsigned char colors3_0[0x0e] = {
   0x32
   };
*/

//sprite 0
const unsigned char frames0_0[0x0e] = {
    0x00, 0x01, 0x01, 0x03, 0x02, 0x03, 0x03, 0x03, 0x06, 0x0d, 0x1a, 0x3a, 0x38, 0x08
};


//sprite 1
const unsigned char frames1_0[0x0e] = {
    0x01, 0x00, 0x00, 0x01, 0x03, 0x00, 0x04, 0x0c, 0x19, 0x33, 0x67, 0x47, 0x07, 0x06
};


//sprite 2
const unsigned char frames2_0[0x0e] = {
    0x00, 0x80, 0x80, 0xc0, 0x40, 0xc0, 0xc0, 0xc0, 0x60, 0xb0, 0x58, 0x5c, 0x1c, 0x10
};


//sprite 3
const unsigned char frames3_0[0x0e] = {
    0x80, 0x00, 0x00, 0x80, 0xc0, 0x00, 0x20, 0x30, 0x98, 0xcc, 0xe6, 0xe2, 0xe0, 0x60
};





int main(void)
{
    ///////////////////////////////////////////////////
    // INITIALIZE PMG /////////////////////////////////
  
    // pmg address handled by linker.
    uint16_t p = (uint16_t) &pmg_memory;
    // set pmbase page with ANTIC
    ANTIC.pmbase = p >> 8; // >> for page
    memset( pmg_memory, 0, sizeof(pmg_memory) );

    // set DMACTL
    OS.sdmctl = 46;    // POKE 559, 46 // double line resolution

    // set GRACTL
    GTIA_WRITE.gractl = 0x03; // POKE 53277,e players and missile


    // widths
    GTIA_WRITE.sizep0 = 0x00;
    GTIA_WRITE.sizep1 = 0x00;
    GTIA_WRITE.sizep2 = 0x00;
    GTIA_WRITE.sizep3 = 0x00;


    OS.color2 = 0x0;
    OS.color3 = 0x4;
    //OS.color4 = 0x0F;
    OS.pcolr0= 0x08;
    OS.pcolr1= 0x32;
    OS.pcolr2= 0x08;
    OS.pcolr3= 0x32;

    // priority
    OS.gprior = 49; // 32 overlap has color, 1 players over playfield, 16 combine

    
    // horizontal positions
    GTIA_WRITE.hposp0=0x3C;
    GTIA_WRITE.hposp1=0x3C;
    GTIA_WRITE.hposp2=0x44;
    GTIA_WRITE.hposp3=0x44;


    // Copy shapes to PMG array
    memcpy( pmg_memory + 572 , frames0_0, height );
    memcpy( pmg_memory + 700 , frames1_0, height );
    memcpy( pmg_memory + 828 , frames2_0, height );
    memcpy( pmg_memory + 956 , frames3_0, height );

    while(1) {
    }
}


