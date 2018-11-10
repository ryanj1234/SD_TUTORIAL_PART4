# SD Tutorial Part 4

This is the source code for the tutorial located here:
http://rjhcoding.com/avrc-sd-interface-4.php

Edit the makefile and change MCU to the microcontroller you are using, and AVRDUDE_PROGRAMMER to your programmer.

Type 'make' to build:
```
Compiling: main.c
avr-gcc -c -mmcu=atmega328p -I. -DF_CPU=16000000UL  -Os 
-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
-Wall -Wstrict-prototypes -Wa,-adhlns=main.lst  -std=gnu99 main.c -o main.o

Linking: main.elf
avr-gcc -mmcu=atmega328p -I. -DF_CPU=16000000UL  -Os 
-funsigned-char -funsigned-bitfields -fpack-struct 
-fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=main.o  
-std=gnu99 main.o --output main.elf -Wl,-Map=main.map,--cref

Creating load file for Flash: main.hex
avr-objcopy -O ihex -R .eeprom main.elf main.hex

Creating load file for EEPROM: main.eep
avr-objcopy -j .eeprom --set-section-flags .eeprom=alloc,load \
--change-section-lma .eeprom=0 -O ihex main.elf main.eep
avr-objcopy: --change-section-lma .eeprom=0x0000000000000000 never used
```

Type make program to write to device:
```
avrdude -p atmega328p -B 10 -c usbtiny -U flash:w:main.hex

avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.00s

avrdude: Device signature = 0x1e950f (probably m328p)
avrdude: NOTE: "flash" memory has been specified, an erase cycle will be performed
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: reading input file "main.hex"
avrdude: input file main.hex auto detected as Intel Hex
avrdude: writing flash (2036 bytes):

Writing | ################################################## | 100% 3.14s

avrdude: 2036 bytes of flash written
avrdude: verifying flash memory against main.hex:
avrdude: load data flash data from input file main.hex:
avrdude: input file main.hex auto detected as Intel Hex
avrdude: input file main.hex contains 2036 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 1.93s

avrdude: verifying ...
avrdude: 2036 bytes of flash verified

avrdude: safemode: Fuses OK (E:FF, H:D9, L:FF)

avrdude done.  Thank you.
```
