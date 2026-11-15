# Controller input
* We don't need to call POTGO apparently. System VBI handler starts it?

* keypad is a bit more complicated than I'd hoped
https://forums.atariage.com/topic/13387-5200-keyboard-reading-magic-kbcode-e809/
https://forums.atariage.com/topic/354064-code-for-reading-5200-joysticks/


If your cartridge runs normally with the Atari logo, then the OS vectors and some of the hardware should already be set up. If your cartridge runs as a diagnostic cart, then basically you'll be starting from nothing but cleared HW registers.

 
https://forums.atariage.com/topic/224616-5200-interrupt-handling/
>> Things to check:
>> 
>> * IRQs must be unmasked on the CPU.

`asm("cli")`;

>> * The keyboard interrupt must be enabled on IRQEN and POKMSK.

IRQEN	=	$E80E
POKMSK	=	$00  
```c
    POKE(POKMSK, PEEK(POKMSK) | 0x40);
    POKEY_WRITE.irqen = PEEK(POKMSK);
```

>> * SKCTL must be set to $02... not $03 as on the 400/800/XL. Computer requires
>>   debounce enabled, 5200 requires it to be disabled. Some emulators will let
>>   you get away with $03, but real hardware won't.

```c
 POKEY_WRITE.skctl = 0x02;
```

>> * SKSTAT bit 2 won't work as you expect. Due to the way the keypad is wired,
>>   each key will only be detected as down for a couple of scanlines at a time.
>>   This also is not necessarily emulated correctly.




https://www.atarimax.com/jindroush.atari.org/data/a5200/5200os.asm
```s
;keypad equates
KEY52_0 = $00
KEY52_1 = $01
KEY52_2 = $02
KEY52_3 = $03
KEY52_4 = $04
KEY52_5 = $05
KEY52_6 = $06
KEY52_7 = $07
KEY52_8 = $08
KEY52_9 = $09
KEY52_ASTERISK = $0A
KEY52_HASH = $0B
KEY52_START = $0C
KEY52_PAUSE = $0D
KEY52_RESET = $0E
KEY52_NONE = $FF

VKEYBD  =	$208 ;Keyboard IRQ vector
VKPD    =	$20A ;Keypad routine continuation vector

```




