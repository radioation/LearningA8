import math
# 5200 ideal/altirra emulated.
# MIN = 0
# MAX = 228 
# MID = 114
# DEAD_ZONE = 40   

# (228 - 0) >> 5 gets us 7   (0-7  = 8 positions. 8x8 = 64 vals)
# (228 - 0) >> 4 gets us 14  (0-14 = 15 pos. 15x15  = 225 vals
# (228 - 0) >> 3 gets us 28  ( 0-28 = 29 pos  841 vals )


# Shift by 4 is probably our best shot and fits in 256 bytes
# instead of calculating 16*16. As we'll only get 0-14 
# I should calcluate 0-14 only with '7' as the center. 
  

# more pessimistic?
# (210 - 10) >> 4 gets us 12  (0-14 = 13 pos. 15x15  = 169 vals

# dead zone should be at least 1 around 7 


# we could access with  LUT[ x + y << 4 ]

speed = 3.0
x_dir_lut = []
y_dir_lut = []
for y in range( 0, 16 ):
    for x in range( 0, 16 ):
        if x < 15 and y < 15 :
            # add a value
            dx = x - 7
            dy = y - 7
            dist = math.sqrt( dx*dx +dy*dy)
            if  dist > 2.0 :
                print('#',end = "")
                x_dir_lut.append( round( 16 * speed * dx/dist  ) & 0xFF   )
                y_dir_lut.append( round( 16 *speed * dy/dist )&  0xFF )
            else:
                print(' ',end = "")
                x_dir_lut.append( 0 )
                y_dir_lut.append( 0 )
        else:
            # nothing needed here
            x_dir_lut.append( 0 )
            y_dir_lut.append( 0 )
            print('.',end = "")
    print("")


print("; X LUT ---------------------------------")
print("x_dir_lut:")
#print("    .byte ", end= "")
#for xd in x_dir_lut:
#    print(xd, end=", ")
for i in range( 0, len( x_dir_lut) , 16 ):
    print("    .byte " + ",".join(f'${x:02X}' for x in x_dir_lut[i:i+16]))

print("; Y LUT ---------------------------------")
print("y_dir_lut:")
for i in range( 0, len( y_dir_lut) , 16 ):
    print("    .byte " + ",".join(f'${y:02X}' for y in y_dir_lut[i:i+16]))



