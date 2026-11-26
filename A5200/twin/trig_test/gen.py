# make a 256 bin lookup table for SIN/COS lookp.  

import math

STEPS = 256
rad_step = math.pi * 2 / 256
rads = 0

print("sin_tab:")
print( "    .byte ", end = "") 
for i in range(0,STEPS): 
    #print (i, math.sin(rads))
    print( round(math.sin( rads ) * 127) & 0xFF , end =', ' )
    rads += rad_step


#vals = [round(math.sin(i * 2 * math.pi / 256) * 127) & 0xFF for i in range(256)]
#
#
#for i in range(0, 256, 16):
#    print("        .byte " + ",".join(f"${v:02X}" for v in vals[i:i+16]))
