
# `__fastcall__`

from the [docs](https://cc65.github.io/doc/cc65-intern.html)
>  fastcall - passes the rightmost parameter in registers A/X/sreg and all others on the C-stack.

and the [wiki](https://github.com/cc65/wiki/wiki/Parameter-passing-and-calling-conventions)

> If a function is declared as `__fastcall__` (or fastcall), the last (rightmost)
> parameter is not passed on the stack, but passed in the primary register to the
> called function. That is A in the case of an eight-bit value, A/X in the case
> of a 16-bit value, and A/X/sreg in the case of a 32-bit value.

*IMP* The function being called is responsible for cleaning up the stack.


This function prototype:
```c
 void cdecl foo(unsigned bar, unsigned char baz);
```
Gets layed out as 

```txt
            +------------------+
            | High byte of bar |
 Offset 2 ->+------------------+
            | Low byte of bar  |
 Offset 1 ->+------------------+
            | baz              |
 Offset 0 ->+------------------+
```
and read by 

```s
         ldy     #2      ; Offset of high byte of bar
         lda     (sp),y  ; High byte now in A
         tax             ; High byte now in X
         dey             ; Offset of low byte of bar
         lda     (sp),y  ; Low byte now in A
```


# ZeroPage
can get around using the stack  with zeropage variables.

create in assemgly

```s
.segment "ZEROPAGE"
_zval:    .res 1
_zcount:    .res 1
_zptr: .res 2
.exportzp _zval, _zcount, _zptr

```
define inC

```c

extern char zval;
#pragma zpsym("zval");
extern char zcount;
#pragma zpsym("zcount");
extern uint8_t* zptr;
#pragma zpsym("zptr");

```



# Inline
[docs](https://www.cc65.org/doc/cc65-9.html)

```c
        asm (<string literal>[, optional parameters]) ;
```



