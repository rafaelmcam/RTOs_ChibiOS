# Hey Emacs, this is a -*- makefile -*-
# Makefile
#
# Taken from "Veronica – Macro Assembly for AVR" - http://quinndunki.com/blondihacks/?p=1106
#
# Original - Parts of this are borrowed from http://electrons.psychogenic.com
#

CLOCK=16000000UL

ifeq ($(BOARD),leonardo)
DEVICE=atmega32u4
PROGRAMMER=avr109
USB_PORT=/dev/ttyACM0
LED_DEFS = -DLED_DDR=DDRC -DLED_PORT=PORTC -DLED_PIN=PINC -DLED_PINNBR=DDC7
BAUD_RATE=57600
else ifeq ($(BOARD),nano)
DEVICE=atmega328p
PROGRAMMER=arduino
USB_PORT=/dev/ttyUSB0
LED_DEFS = -DLED_DDR=DDRB -DLED_PORT=PORTB -DLED_PIN=PINB -DLED_PINNBR=DDB5
BAUD_RATE=57600
else ifeq ($(BOARD),uno)
DEVICE=atmega328p
PROGRAMMER=arduino
USB_PORT=/dev/ttyUSB0
LED_DEFS = -DLED_DDR=DDRB -DLED_PORT=PORTB -DLED_PIN=PINB -DLED_PINNBR=DDB5
BAUD_RATE=115200
else ifeq ($(BOARD),unov2)
DEVICE=atmega328p
PROGRAMMER=arduino
USB_PORT=/dev/ttyACM0
LED_DEFS = -DLED_DDR=DDRB -DLED_PORT=PORTB -DLED_PIN=PINB -DLED_PINNBR=DDB5
BAUD_RATE=57600
else ifeq ($(BOARD),promicro)
DEVICE=atmega32u4
PROGRAMMER=avr109
USB_PORT=/dev/ttyACM0
LED_DEFS = -DLED_DDR=DDRB -DLED_PORT=PORTB -DLED_PIN=PINB -DLED_PINNBR=DDB0
BAUD_RATE=57600
else ifeq ($(MAKECMDGOALS), )
$(error Você deve definir a placa que está programando através da opção BOARD=<placa>, onde placa=leonardo|nano|uno|promicro)
endif

#############################
# Source files (use .S for assembly)

MAIN_FILE=potenciometro
SOURCES=$(MAIN_FILE).c


#############################

CFLAGS=-I. -g -mmcu=$(DEVICE) -Os -DF_CPU=$(CLOCK) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	$(LED_DEFS)                             \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

ASMFLAGS =-I. -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) \
	-x assembler-with-cpp            \
	-Wa,-gstabs,-ahlms=$(firstword   \
		$(<:.S=.lst))

LDFLAGS=-Wl,-Map,$(MAIN_FILE).out.map -mmcu=$(DEVICE)

CFILES=$(filter %.c, $(SOURCES))
ASMFILES=$(filter %.S, $(SOURCES))

OBJDEPS=$(CFILES:.c=.o)    \
	$(ASMFILES:.S=.o)

M4DEPS=

LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

.SUFFIXES : .c .o .out .S .hex .ee.hex .h

.PHONY: writeflash clean stats gdbinit stats fuse

#############################
# Make targets:
# all, disasm, stats, hex, writeflash/install, clean, fuse

all: $(MAIN_FILE).out

%.o: %.S
	avr-gcc -c $(ASMFLAGS) -o $@ $<

%.o: %.c
	avr-gcc -c $(CFLAGS) -o $@ $<

disasm: $(MAIN_FILE).out
	avr-objdump -d $(MAIN_FILE).out

stats: $(MAIN_FILE).out
	avr-objdump -h $(MAIN_FILE).out
	avr-size $(MAIN_FILE).out

hex: $(MAIN_FILE).hex $(MAIN_FILE).ee.hex

writeflash: hex
ifeq ($(BOARD),promicro)
	python3 -c "import serial; serial.Serial(\"$(USB_PORT)\", 1200).close()"
endif
	avrdude -P $(USB_PORT) -c $(PROGRAMMER) -p $(DEVICE) -b$(BAUD_RATE) -v -v -v -v -D -U flash:w:$(MAIN_FILE).hex:i

install: writeflash

clean:
	rm -f $(MAIN_FILE).out $(MAIN_FILE).out.map
	rm -f $(OBJDEPS)
	rm -f $(LST)
	rm -f *~
	rm -f $(MAIN_FILE).hex $(MAIN_FILE).ee.hex $(MAIN_FILE).elf

fuse:
	avrdude -c $(PROGRAMMER) -p $(DEVICE) $(FUSES)

$(MAIN_FILE).out: $(OBJDEPS)
	avr-gcc $(LDFLAGS) -o $(MAIN_FILE).out $(OBJDEPS)


#############################
# Generating object files

.S.o :
	avr-gcc $(ASMFLAGS) -c $< -o $@


#############################
# Generating hex files

# hex files from elf
.out.hex:
	avr-objcopy -j .text -j .data -O ihex $< $@

.out.ee.hex:
	avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@
