# hardware
MCU = atmega168pb
PROGRAMMER = stk500v2
PORT = /dev/ttyUSB0

# dirs
AVR_DIR = /usr/lib/gcc/avr/14.1.0

CC = avr-g++

%.o: %.c
	${CC} -mmcu=${MCU} -B ${AVR_DIR} -c $< -Os

%.elf: %.o
	${CC} -mmcu=${MCU} -o $@ $<

%.hex: %.elf
	avr-objcopy -O ihex $< $@

%.flash: %.hex
	sudo avrdude -p ${MCU} -e -c ${PROGRAMMER} -P ${PORT} -Uflash:w:$<

.PHONY: all
clean:
	rm -rf *.hex *.elf *.o
