# PMG

*IMP* PMBASE must be on a 1k boundary for double line resolution. *2k* for single-line


According to COmpute's first book of atari graphics
* first 384/768 bytes are *unused* and safe to use :
  [seconded here](https://forums.atariage.com/topic/233653-pm-graphics-unused-memory-after-pmbase/)

> Yes, it's safe to do what you want with the unused space. I've put character
> sets, display lists, whatever there. ANTIC doesn't care.




SO `PMBASE + 384` gets you to the missle bytes for double-line resolution
   `PMBASE + 768` gets you to the missle bytes for Single-line resoultion

DOUBLE: 128 lines high
    PMBASE
     + 384 : missle bytes
     + 512 : Player 0
     + 640 : Player 2
     + 768 : Player 3
     + 896 : Player 4


SINGLE : 256 lines tall
    PMBASE
     +  768 : missle bytes
     + 1024 : Player 0
     + 1280 : Player 2
     + 1536 : Player 3
     + 1792 : Player 4

Important locations





From COmpute's First Book of Atari Graphics and [Gury's](https://gury.atari8.info/refs/pmg.php)
* `559`: put a 62 here for a single-line, a 46 for double-line resolution
   *IMP* Shadow Register for 54272 (DMACTL) 

    Option                          Decimal   Bit
    ----------------------------------------------------
    Enable missle DMA                     4   2
    Enable player DMA                     8   3
    Enable player and missile DMA        12   2,3
    One line player resolution           16   4
    
    Note that two-line player resolution is the default 

  `OS.sdmctl = 46` 

* `623`: sets player/playfield priorities (only one bit on!)
  * 1: all players have priority over all playfield registers
  * 4: all playfield registers have priority over all players
  * 2: mixed. P0 & P1, then all playfield, then P2 & P3
  * 8: mixed, PF0 & PF1, then all players, then PF2 & PF3
  * 16 : COMBINE MISSILES INTO A 5th player ( they use field color OS.color4 )
  * 32 : MIX OVERLAP 

`OS.gprior = 49; // 32 overlap has color, 1 players over playfield, 16 combine`

* `704`: color of player/missile 0
    `OS.pcolr0= 0x08;`
* `705`: color of player/missile 1
    `OS.pcolr1= 0x08;`
* `706`: color of player/missile 2
    `OS.pcolr2= 0x08;`
* `707`: color of player/missile 3
    `OS.pcolr3= 0x08;`

* `53248`: horizontal position of player 0
    `GTIA_WRITE.hposp0=0x3C;`
* `53249`: horizontal position of player 1
    `GTIA_WRITE.hposp1=0x3C;`
* `53250`: horizontal position of player 2
    `GTIA_WRITE.hposp2=0x3C;`
* `53251`: horizontal position of player 3
    `GTIA_WRITE.hposp3=0x3C;`

* `53252`: horizontal position of missile 0
    ` GTIA_WRITE.hposm0 = 0x4F`
* `53253`: horizontal position of missile 1
    ` GTIA_WRITE.hposm1 = 0x4F`
* `53254`: horizontal position of missile 2
    ` GTIA_WRITE.hposm2 = 0x4F`
* `53255`: horizontal position of missile 3
    ` GTIA_WRITE.hposm3 = 0x4F`

* `53256`: size of player 0 (0 = normal, 1 = double, 3 = quadruple)
    `GTIA_WRITE.sizep0 = 0x01`
* `53257`: size of player 1 (0 = normal, 1 = double, 3 = quadruple)
    `GTIA_WRITE.sizep1 = 0x01`
* `53258`: size of player 2 (0 = normal, 1 = double, 3 = quadruple)
    `GTIA_WRITE.sizep2 = 0x01;`
* `53259`: size of player 3 (0 = normal, 1 = double, 3 = quadruple)
    `GTIA_WRITE.sizep3 = 0x01;`

* `53277`: A 3 here enables player/missile graphics, a 0 disables them.

                           Decimal   Bit
   To turn on missiles        1       0
   To turn on players         2       1
   To latch trigger inputs    4       2

    ` GTIA_WRITE.gractl = 0x03;`

* `54279`: put high byte of PMBASE here




