# Atual Display List
2 lines of GR.0 followed by 176 lines of ANTIC E
80 bytes + 7040 bytes -> 7120 bytes, so we definitely cross the 
4k boundary.

We can put 2 GR.0 lines and 100 ANTIC E lines in the first 4096. with 16 left over.

[atariage](https://forums.atariage.com/topic/233653-pm-graphics-unused-memory-after-pmbase/)
> Display lists don't have to START on a 1K boundary, they just *can't __cross__ a 1K
> boundary without using the JUMP instruction*

# Display List Interrupts

From [Player Missile Podcast](https://playermissile.com/dli_tutorial/#our-first-display-list)
> Display list interrupts are not enabled by default. To use a DLI, the address
> vector at VDSLST ($200 and $201


We only really have one DLI vector, so somtimes you make your DLIs
set the register to the next one, making a chain of them.

*IMP* DLI doesn't know what scan line caused the interrupt. The only trigger
  is the DLI bit on the display list instruction
  DLI bit is bit 7 ($80).  
*IMP* DLIs must save any registers it clobbers PHA, tya pha, txa pha
*IMP* DLIs must restor any registers it clobbers pla tax, pla tay, pla
*IMP* DLIs must exit with RTI

*IMP* DLIs don't have to be short. somehwat arbitrary amounts of code can be
executed in a DLI [playermissile](https://playermissile.com/dli_tutorial/index.html)

So really we need:
1. A DLI routine. Write it in assembly
```s
.proc dli_1
   pha           ; Save A
   lda #$00
   sta WSYNC     ; wait for sync
   sta COLBK     ; change color
   pla
   ;  could set another DLI routine here
   rti 
```

2. A Display List line to trigger the DLI (bit 8 must be set)

```s
   lda #$C2                   ; SET INTERRUPT $80 and MODE $40 + 2 (GR.0/antic 2)
   sta main_dlist + 3         ; move to the line we want the DLI to fire
```
3. Set VDSLST to the address of the DLI routine. 
```s
   lda #<dli_1                 ; set address of the dli
   sta VDSLST
   lda #>dli_1
   sta VDSLST+1

```

4. turn both DLI and VBI 

```c
ANTIC.nmien = 0xc0;  // activate BOTH display list ($80) and vertical blank $(40) interrupts
```


## change colors for top two lines 
*IMP* chanign the very first GR.0 line doesn't kcik in a color
change until the second one. So I set the 3rd Top Blank to 
be the interrupt.
```s

   lda #$F0                   ; SET INTERRUPT $80 and last  blank $70
   sta main_dlist + 2         ; move to the line we want the DLI to fire

```


## do a second interrupt on the logo

had to set a notherline in the interupt 
```s

   lda #$8e                   ; SET INTERRUPT $80 and last moed e $0e
   sta main_dlist + 32        ; move to the line we want the DLI to fire

```

and first dli has to setup next dli address

```s

.proc dli_1
  pha           ; save A to the stack
  lda #$00      ;
  sta WSYNC
  sta COLPF2    ; change GR.0 color

   lda #<dli_2                 ; set address of the next dli
   sta VDSLST
   lda #>dli_2
   sta VDSLST+1

  pla
  rti
.endproc

```


## VBI
I used the VBI ro rest the address of the first dli

```s

.proc vbi

   lda #<dli_1                 ; set address of the first dli
   sta VDSLST
   lda #>dli_1
   sta VDSLST+1
  jmp XITVBV
.endproc


```





# py for image
Each pixel in the PNG represents 1 of 4 palette indexes ( 2-bits) 
So each byte output must be 4 pixels of PNG imaage
```py
    for x in range(0, width, 4 ):
        outfile.write(f'{px[x,y]*64+px[x+1,y]*16+px[x+2,y]*4+px[x+3,y]}' )
```

generate with

```bash
python3 -m venv env

. ./env/bin/activate

pip3 install

python3 png_to_bytes.py -i paletteized.png  -o img_data.c

```
