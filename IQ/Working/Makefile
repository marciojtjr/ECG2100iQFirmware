################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../rtos.c \
../wdt_driver.c \
../clksys_driver.c \
../TC_driver.c \
../iq.c \
../TaskAcq.c \
../usart_driver.c \
../adc_driver.c \
../eeprom_driver.c \
../TaskUSB.c \
../TaskFlash.c \
../spi_driver.c \
../lowpower.c \
../rtc_driver.c \
../sleepmgr.c \
../ccpwrite.c \
../seg7.c \
../buffers.c \
../ulaw.c \
../eeprom.c \
../TaskBT.c \
../TaskWatchDog.c \
../crc.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
rtos.o \
wdt_driver.o \
clksys_driver.o \
TC_driver.o \
iq.o \
TaskAcq.o \
usart_driver.o \
adc_driver.o \
eeprom_driver.o \
TaskUSB.o \
TaskFlash.o \
spi_driver.o \
lowpower.o \
rtc_driver.o \
sleepmgr.o \
ccpwrite.o \
seg7.o \
buffers.o \
ulaw.o \
eeprom.o \
TaskBT.o \
TaskWatchDog.o \
crc.o


OBJS_AS_ARGS +=  \
rtos.o \
wdt_driver.o \
clksys_driver.o \
TC_driver.o \
iq.o \
TaskAcq.o \
usart_driver.o \
adc_driver.o \
eeprom_driver.o \
TaskUSB.o \
TaskFlash.o \
spi_driver.o \
lowpower.o \
rtc_driver.o \
sleepmgr.o \
ccpwrite.o \
seg7.o \
buffers.o \
ulaw.o \
eeprom.o \
TaskBT.o \
TaskWatchDog.o \
crc.o


C_DEPS +=  \
rtos.d \
wdt_driver.d \
clksys_driver.d \
TC_driver.d \
iq.d \
TaskAcq.d \
usart_driver.d \
adc_driver.d \
eeprom_driver.d \
TaskUSB.d \
TaskFlash.d \
spi_driver.d \
lowpower.d \
rtc_driver.d \
sleepmgr.d \
ccpwrite.d \
seg7.d \
buffers.d \
ulaw.d \
eeprom.d \
TaskBT.d \
TaskWatchDog.d \
crc.d


C_DEPS_AS_ARGS +=  \
rtos.d \
wdt_driver.d \
clksys_driver.d \
TC_driver.d \
iq.d \
TaskAcq.d \
usart_driver.d \
adc_driver.d \
eeprom_driver.d \
TaskUSB.d \
TaskFlash.d \
spi_driver.d \
lowpower.d \
rtc_driver.d \
sleepmgr.d \
ccpwrite.d \
seg7.d \
buffers.d \
ulaw.d \
eeprom.d \
TaskBT.d \
TaskWatchDog.d \
crc.d


OUTPUT_FILE_PATH +=iq_bluetooth.elf

OUTPUT_FILE_PATH_AS_ARGS +=iq_bluetooth.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR32/GNU C Compiler















































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : (WinAVR 20100110) 4.3.3
	$(QUOTE)C:\apps\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DBLUETOOTH=1 -DF_CPU=32000000UL -DWORKING  -Os -fpack-struct -fshort-enums -g3 -Wall -pedantic -c -gdwarf-2 -std=gnu99  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -mmcu=atxmega64a3   -o"$@" "$<" 
	@echo Finished building: $<



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : (WinAVR 20100110) 4.3.3
	$(QUOTE)C:\apps\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="iq_bluetooth.map" -Wl,--start-group -Wl,-lm -Wl,-lprintf_flt -Wl,-lscanf_flt  -Wl,--end-group -Wl,-u,vfscanf -Wl,-u,vfprintf   -mmcu=atxmega64a3  
	@echo Finished building target: $@
	"C:\apps\WinAVR-20100110\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "iq_bluetooth.elf" "iq_bluetooth.hex"
	"C:\apps\WinAVR-20100110\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "iq_bluetooth.elf" "iq_bluetooth.eep" || exit 0
	"C:\apps\WinAVR-20100110\bin\avr-objdump.exe" -h -S "iq_bluetooth.elf" > "iq_bluetooth.lss"
	"C:\apps\WinAVR-20100110\bin\avr-size.exe" "iq_bluetooth.elf"
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) 
	rm -rf "iq_bluetooth.elf" "iq_bluetooth.a" "iq_bluetooth.hex" "iq_bluetooth.lss" "iq_bluetooth.eep" "iq_bluetooth.map"
	