###############################################################################
# Makefile for the project peanut
###############################################################################

## General Flags
PROJECT = peanut
MCU = atxmega64a3
TARGET = peanut.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99   -DMANUFACTURING=1     -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-u,vfscanf  -Wl,-u,vfprintf  -Wl,-Map=peanut.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Libraries
LIBS = -lm -lprintf_flt -lscanf_flt 

## Objects that must be built in order to link
OBJECTS = rtos.o wdt_driver.o clksys_driver.o TC_driver.o peanut.o TaskAcq.o usart_driver.o adc_driver.o eeprom_driver.o TaskUSB.o TaskFlash.o spi_driver.o lowpower.o rtc_driver.o sleepmgr.o ccpwrite.o seg7.o buffers.o ulaw.o eeprom.o crc.o TaskBT.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) peanut.hex peanut.eep peanut.lss size

## Compile
rtos.o: ../rtos.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wdt_driver.o: ../wdt_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

clksys_driver.o: ../clksys_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TC_driver.o: ../TC_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

peanut.o: ../peanut.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TaskAcq.o: ../TaskAcq.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usart_driver.o: ../usart_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc_driver.o: ../adc_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

eeprom_driver.o: ../eeprom_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TaskUSB.o: ../TaskUSB.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TaskFlash.o: ../TaskFlash.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi_driver.o: ../spi_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lowpower.o: ../lowpower.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtc_driver.o: ../rtc_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sleepmgr.o: ../sleepmgr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ccpwrite.o: ../ccpwrite.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

seg7.o: ../seg7.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

buffers.o: ../buffers.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ulaw.o: ../ulaw.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

eeprom.o: ../eeprom.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

crc.o: ../crc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TaskBT.o: ../TaskBT.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) peanut.elf dep/* peanut.hex peanut.eep peanut.lss peanut.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

