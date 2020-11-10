NAME	= olampki
MCU	= attiny13
FREQ	= 9600000
FUSEH	= fb
FUSEL	= 32
CFLAGS	= -std=c99

SCONS		= scons
AVRDUDE		= avrdude
PROG		= usbasp
PORT		= /dev/ttyUSB0

.PHONY: all
all: build

.PHONY: build
build:
	$(SCONS) -f avr.scons NAME=$(NAME) MCU=$(MCU) FREQ=$(FREQ) GAL=$(GALPATH) ADDCFLAGS="$(CFLAGS)" ADDCXXFLAGS="$(CXXFLAGS)" ADDLDFLAGS="$(LDFLAGS)"

.PHONY: poke
poke: build
	$(AVRDUDE) -V -i 5 -p $(MCU) -c $(PROG) -P $(PORT) -e -U lfuse:w:0x$(FUSEL):m -U hfuse:w:0x$(FUSEH):m -U flash:w:build/$(NAME).hex

.PHONY: clean
clean:
	$(SCONS) -c -f avr.scons NAME=$(NAME) MCU=$(MCU) FREQ=$(FREQ) GAL=$(GALPATH) ADDCFLAGS="$(CFLAGS)" ADDCXXFLAGS="$(CXXFLAGS)" ADDLDFLAGS="$(LDFLAGS)"
