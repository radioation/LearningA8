import struct, sys

rom = open(sys.argv[1], "rb").read()
cart_type = 16 if len(rom) == 16384 else 4  # 16 = 16KB assuming 1-chip, 4 = 32KB

checksum = sum(rom) & 0xFFFFFFFF
header = b"CART" + struct.pack(">II4x", cart_type, checksum)

open(sys.argv[2], "wb").write(header + rom)

