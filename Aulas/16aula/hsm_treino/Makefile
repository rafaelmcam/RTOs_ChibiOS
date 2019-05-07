# Hey Emacs, this is a -*- makefile -*-
# Makefile
#
# Taken from "Veronica – Macro Assembly for AVR" - http://quinndunki.com/blondihacks/?p=1106
#
# Original - Parts of this are borrowed from http://electrons.psychogenic.com
#


#############################
# Source files (use .S for assembly)

MAIN_FILE=hsm_ex1
SOURCES=$(MAIN_FILE).c event.c sm.c bsp_linux.c


#############################

CFLAGS=-I. -g -O3 -fshort-enums         \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes -pthread

ASMFLAGS =-I. -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) \
	-x assembler-with-cpp            \
	-Wa,-gstabs,-ahlms=$(firstword   \
		$(<:.S=.lst))

LDFLAGS=-pthread

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

all: $(MAIN_FILE)

%.o: %.S
	gcc -c $(ASMFLAGS) -o $@ $<

%.o: %.c
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(MAIN_FILE).out $(MAIN_FILE).out.map $(MAIN_FILE)
	rm -f $(OBJDEPS)
	rm -f $(LST)
	rm -f *~
	rm -f $(MAIN_FILE).hex $(MAIN_FILE).ee.hex $(MAIN_FILE).elf

fuse:
	avrdude -c $(PROGRAMMER) -p $(DEVICE) $(FUSES)

$(MAIN_FILE): $(OBJDEPS)
	gcc $(LDFLAGS) -o $(MAIN_FILE) $(OBJDEPS)


#############################
# Generating object files

.S.o :
	gcc $(ASMFLAGS) -c $< -o $@
