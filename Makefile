# makefile for mazout_tiny

tiny1:
# Compile
	avr-gcc -Os -DF_CPU=1000000UL -mmcu=attiny84 -c main.c
	avr-gcc -Os -DF_CPU=1000000UL -mmcu=attiny84 -c lcd.c
	avr-gcc -Os -DF_CPU=1000000UL -mmcu=attiny84 -c adc.c
	avr-gcc -Os -DF_CPU=1000000UL -mmcu=attiny84 -c srf04.c

#linking
	avr-gcc -Os -DF_CPU=1000000UL -mmcu=attiny84 main.o lcd.o srf04.o adc.o -o main
#	avr-gcc -Os -DF_CPU=1000000UL -mmcu=attiny84 main.o LCD-AVR-4d.o srf04.o -o main

# convert to AVR-hex
	avr-objcopy -O ihex -R .eeprom main main.hex

# flash to the device
#	avrdude -c USBasp -p attiny84 -P /dev/USBasp -b 115200 -U flash:w:main.hex

tiny4:
# Compile
	avr-gcc -Os -DF_CPU=4000000UL -mmcu=attiny84 -c main.c
	avr-gcc -Os -DF_CPU=4000000UL -mmcu=attiny84 -c LCD-AVR-4d.c

#linking
	avr-gcc -Os -DF_CPU=4000000UL -mmcu=attiny84 main.o LCD-AVR-4d.o -o main

# convert to AVR-hex
	avr-objcopy -O ihex -R .eeprom main main.hex

# flash to the device
#	avrdude -c USBasp -p attiny84 -P /dev/USBasp -b 115200 -U flash:w:main.hex

mega:
# Compile
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c main.c
#	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c LCD-AVR-4d.c
	
#linking LCD-AVR-4d.o 
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p main.o -o main

# convert to AVR-hex
	avr-objcopy -O ihex -R .eeprom main main.hex
	
# flash to the device
#	avrdude -c USBasp -p ATMEGA328P -P /dev/USBasp -b 115200 -U flash:w:main.hex
