from perlin_noise import PerlinNoise
import os, argparse
import random
from PIL import Image

chunk_data = []

WORLD_SEED = 84384892817394821



# ===== Biome Levels =====
def get_biome(value):
    if value < -0.10:
        return '~'  # Water
    elif value < 0.0:
        return '.'  # Sand
    elif value < 0.20:
        return ','  # Grassland
    elif value < 0.4:
        return '^'  # Hills
    else:
        return 'M'  # Mountain

def get_tile_id(value):
    if value < -0.10:
        return 0  # Water
    elif value < 0.0:
        return 1 # Sand
    elif value < 0.20:
        return 2  # Grassland
    elif value < 0.4:
        return 3  # Hills
    else:
        return 4  # Mountain

#def print_world(world):
#    for row in world:
#        print(''.join(row))
#
#def a8_world_row1( tile ):
#    match tile:
#        case '~': # Water
#            return "2,3,"
#        case '.': # Sand
#            return "27,28,"
#        case ',': # Grassland
#            return "187,188,"
#        case '^': # Hills
#            return "141,140,"
#        case 'M': # Mountain
#            return "10,11,"
#
#def a8_world_row2( tile ):
#    match tile:
#        case '~': # Water
#            return "2,3,"
#        case '.': # Sand
#            return "29,30,"
#        case ',': # Grassland
#            return "189,190,"
#        case '^': # Hills
#            return "141,140,"
#        case 'M': # Mountain
#            return "12,13,"
#

def save_world( world, basename ) :
    #print(world[0])
    #print(world[1])
    with open( basename + ".c", 'w') as ofile:
        for chunk in world:
            print(f'x: {chunk[0]} y: {chunk[1]}')
            ofile.write(f'uint8_t chunk_{chunk[0]}_{chunk[1]}[] = {{\n')
            for row in chunk[2]:
                for col in row:
                    ofile.write( f'{col}, ' )
                ofile.write('\n')
            ofile.write('\n|\n')

        

def main( args ):
    print(f"world seed: {args.seed}")

    x_chunks = int(args.width/args.chunk_size)
    y_chunks = int(args.height/args.chunk_size)
    print(f'x chunks: {x_chunks} y chunks: {y_chunks}')

    noise_generator = PerlinNoise(octaves=4, seed=args.seed)

    # these should eventually be params, just start 0,0 chunk for testing.
    chunk_x = 0;
    chunk_y = 0;

    world = []
    # C
    for chunk_y in range( y_chunks ):
        for chunk_x in range( x_chunks ):
            start_x = chunk_x * args.chunk_size
            start_y = chunk_y * args.chunk_size
            chunk_data = []
            print(f'start x: {start_x} y: {start_y}') 
            for y in range(args.chunk_size):
                row = []
                for x in range(args.chunk_size):
                    # Absolute world coordinates passed to the generator
                    world_x = (start_x + x) / args.scale
                    world_y = (start_y + y) / args.scale
  
                    value = get_tile_id(noise_generator([world_x, world_y]))
                    row.append(value)
                chunk_data.append(row)
            #print('---------------------------------------')
            #print_world(chunk_data)
            world.append( [ start_x, start_y, chunk_data] )

    if len(args.base_filename) > 0:
        save_world( world, args.base_filename );
    





   
if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description = "Create worlds for A8 and MegaDrive",
        fromfile_prefix_chars = '@' )
    parser.add_argument( "-w",
        "--width",
        default = 64,
        type=int,
        help = "world map width in tiles ",
        metavar = "ARG")

    parser.add_argument( "-H",
        "--height",
        default = 64,
        type=int,
        help = "world map height in tiles ",
        metavar = "ARG") 


    parser.add_argument( "-c",
        "--chunk_size",
        default = 32,
        type=int,
        help = "world chunk size",
        metavar = "ARG")


    parser.add_argument( "-s",
        "--scale",
        default = 15,
        type=int,
        help = "Zoom level of terrain (lower noisy, higher smoother)",
        metavar = "ARG")





    parser.add_argument( "-S",
        "--seed",
        default = 1250,
        type=int,
        help = "Zoom level of terrain (lower noisy, higher smoother)",
        metavar = "ARG")

    parser.add_argument( "-b",
        "--base_filename",
        default = "outfile_",
        help = "base name for output files (will auto add A8, C64, MD as needed)",
        metavar = "ARG")




    args = parser.parse_args()
    main(args)
