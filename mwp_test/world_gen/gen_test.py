from perlin_noise import PerlinNoise
import os, argparse
import random
from PIL import Image

chunk_data = []

WORLD_SEED = 84384892817394821

chunk_size =32


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


def print_world(world):
    for row in world:
        print(''.join(row))

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


def main( args ):
    print(f"world seed: {args.seed}")

    noise_generator = PerlinNoise(octaves=4, seed=args.seed)
    chunk_data = []

    # these should eventually be params, just start 0,0 chunk for testing.
    chunk_x = 0;
    chunk_y = 0;

    # C
    start_x = chunk_x * chunk_size
    start_y = chunk_y * chunk_size

    for y in range(chunk_size):
        row = []
        for x in range(chunk_size):
            # Absolute world coordinates passed to the generator
            world_x = (start_x + x) / args.scale
            world_y = (start_y + y) / args.scale

            value = get_biome(noise_generator([world_x, world_y]))
            row.append(value)
        chunk_data.append(row)
        print(row)






   
if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description = "Create worlds for A8 and MegaDrive",
        fromfile_prefix_chars = '@' )

    parser.add_argument( "-w",
        "--width",
        default = 40,
        type=int,
        help = "map width in tiles ",
        metavar = "ARG")

    parser.add_argument( "-H",
        "--height",
        default = 20,
        type=int,
        help = "map height in tiles ",
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
