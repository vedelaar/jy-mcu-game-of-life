avr-gcc -O2 -mmcu=atmega8 -o main.elf *.cpp
avr-objcopy -O ihex main.elf main.hex
avrdude -c usbasp -p m8 -v -V -B8 -U flash:w:main.hex:i
rm *.elf
rm *.hex
