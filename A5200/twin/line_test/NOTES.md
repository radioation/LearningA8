# Line drawing tests
* originaly tested on A800
* Using modded FS1 line drawing code from [atari age](https://forums.atariage.com/topic/231690-known-fast-line-algorithms/page/4/)


A5200 has some 'quirks'
* As a primarily ROM based system I'll need to copy the self-modifying flight 
  simulator code to RAM.
  * this can be done by putting it in its own segment `FS1CODE` and 
    a. defining it iwht the .cfg
```cfg
# FS1 code stored in ROM but needs to be in RAM DEFINE is imporatnt
    FS1CODE:   load = ROM, run = RAM, type = rw,  define = yes;
```
    b. using ld65 symbols `__FS1CODE_LOAD__` (in rom?) `__FS1CODE_RUN__` (in RAM)
       `__FSCODE_SIZE__` number of bytes to copy


* with only 16K of RAM, I probably don't want to use ANTIC E or F ever

* if I know at compile time the RAM locations, I don't have to make self-modifying 
  versions of CLOOP and can drop INICL

I should
* Reserve screen ram for ANTICD (probably two screens, 3840 bytes each, don't cross 
  4K boundary due to pesky restrictions)
* Reserve a range for PMG
* Store FS1 routines in ROM but copy to RAM at program start

```cfg

    PMG: file="", start=$1800, size = $0800, define = yes;
    SCREEN0: file="", start=$2000, size = $1000, define = yes;
    SCREEN1: file="", start=$3000, size = $1000, define = yes;

```


