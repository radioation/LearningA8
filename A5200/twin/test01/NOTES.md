# notas
## player movement
I'm using the difference between the current potentiometer
reading and midpoint (will need calibration in real game) 
and down shifting that to add to the 8.8 position of the 
player

```c
        if( xdir1 > 12 || xdir1 < -12 ) {
            p0_x +=  ( xdir1 << 2 ) ; (looks like an up shift here, but when setting player position I'm shiftin 8 in the other dir.)
        }
        if( ydir1 > 12 || ydir1 < -12 ) {
            p0_y +=  ( ydir1 << 2 ) ;
        }
```

```c
        GTIA_WRITE.hposp0=(uint8_t)( p0_x >> 8 );
        memset( pmg_memory + 1020 + (uint8_t)(p0_y >> 8 ), 0, 16 ); // wasteful clear.
        memcpy( pmg_memory + 1024 + (uint8_t)( p0_y >> 8 ), frames0_0, height );
```


## shooting 

* player/missile visible bounds X: 48 to 207  Y: 32 to 223

* when generating 8.8 LUT with negative values generate the 
  whole 16bit int together and split bytes into separte tables.
```py
                temp_x = int(round( 256.0 * speed * dx/dist ))
                twos_comp_x = temp_x &0xFFFF
                temp_y = int(round( 256.0 * speed * dy/dist ))
                twos_comp_y = temp_y &0xFFFF
                x_dir_lut_hi.append( ( twos_comp_x >>8 ) & 0xFF   )
                y_dir_lut_hi.append( ( twos_comp_y >> 8 )&  0xFF )
                x_dir_lut_lo.append(  twos_comp_x & 0xFF   )
                y_dir_lut_lo.append(  twos_comp_y &  0xFF )
```

* FIX8.8 addition in assembly
  * I used X for 8.8 LUTs
  * I use Y to keep track of the current missile. 
```asm
    LDX _zp_lut_index ; X is used to get velocity components.
    LDY #0            ; Y is missile slot, just doing misile 0 here

    clc     ; get ready to add
    LDA _missile_x_lo, Y ; get M0 fractional component
    ADC x_dir_lut_lo, X     ; add low velocity to it
    STA _missile_x_lo, Y ; save it

    LDA _missile_x_hi, Y ; get M0 integer component
    ADC x_dir_lut_hi, X     ; add hi velocity to it
    STA _missile_x_hi, Y ; save it

```
       
