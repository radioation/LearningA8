# Command Line Arguments Example
cc65 supports command line arguments for `atari` targets. This does 
[require](https://cc65.github.io/doc/atari.html#ss11.2) a DOS that 
supports arguments.


To make a disk image with [mkatr](https://github.com/dmsc/mkatr)

```bash
 mkatr 01_hello.atr 01_helloarg.xex
```

Mount this disk image to D2: and boot from a compatible DOS (SpartDOS,
RealDOS, BW-DOS, etc) in D1:


```cmd
D2:
01_HELLO.XEX FUII SAMA
```
