import os, argparse, logging
from PIL import Image


def main(args, loglevel):
    logging.basicConfig( format="%(levelname)s: %(message)s", level=loglevel )

    with open( args.output_filename, "w") as outfile, Image.open( args.input_filename) as img:
        pal = img.getpalette()
        logging.info( 'Palette' )
        logging.info( pal )

       
        width, height = img.size

        logging.info( f'Image w:{width} h:{height}' )
       
        px = img.load()
        outfile.write( f'const unsigned char {args.data_name}[] = {{\n' )
        for y in range(0, height ):
            for x in range(0, width ):
                outfile.write(f' {px[x,y]},' )
            outfile.write("\n")
        outfile.write("};\n")
        


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
            description = "Create comma separated list of byte values from 4-color paletteized png files"
            )

    # Parameter list
    parser.add_argument( "-v",
                        "--verbose",
                        help="Increase output verbositly",
                        action="store_true")


    parser.add_argument( "-i",
                        "--input_filename",
                        default = 'atari_fuji_160x176.png',
                        help = "input image filename",
                        metavar = "ARG")

    parser.add_argument( "-o",
                        "--output_filename",
                        default = 'img_data.c',
                        help = "Output filename",
                        metavar = "ARG")

    parser.add_argument( "-n",
                        "--data_name",
                        default = 'img_data',
                        help = "Name of C array",
                        metavar = "ARG")

    args = parser.parse_args()

    if args.verbose:
        loglevel = logging.DEBUG
    else:
        loglevel = logging.WARNING

    main(args, loglevel)
