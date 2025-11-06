#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Bluetooth.mk)" "nbproject/Makefile-local-Bluetooth.mk"
include nbproject/Makefile-local-Bluetooth.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Bluetooth
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../main.c ../serial.c ../sp_driver.S ../rtos.c ../wdt_driver.c ../clksys_driver.c ../TC_driver.c ../iq.c ../usart_driver.c ../adc_driver.c ../eeprom_driver.c ../spi_driver.c ../lowpower.c ../rtc_driver.c ../sleepmgr.c ../ccpwrite.c ../seg7.c ../buffers.c ../ulaw.c ../eeprom.c ../TaskWatchDog.c ../crc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/sp_driver.o ${OBJECTDIR}/_ext/1472/rtos.o ${OBJECTDIR}/_ext/1472/wdt_driver.o ${OBJECTDIR}/_ext/1472/clksys_driver.o ${OBJECTDIR}/_ext/1472/TC_driver.o ${OBJECTDIR}/_ext/1472/iq.o ${OBJECTDIR}/_ext/1472/usart_driver.o ${OBJECTDIR}/_ext/1472/adc_driver.o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ${OBJECTDIR}/_ext/1472/spi_driver.o ${OBJECTDIR}/_ext/1472/lowpower.o ${OBJECTDIR}/_ext/1472/rtc_driver.o ${OBJECTDIR}/_ext/1472/sleepmgr.o ${OBJECTDIR}/_ext/1472/ccpwrite.o ${OBJECTDIR}/_ext/1472/seg7.o ${OBJECTDIR}/_ext/1472/buffers.o ${OBJECTDIR}/_ext/1472/ulaw.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ${OBJECTDIR}/_ext/1472/crc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/serial.o.d ${OBJECTDIR}/_ext/1472/sp_driver.o.d ${OBJECTDIR}/_ext/1472/rtos.o.d ${OBJECTDIR}/_ext/1472/wdt_driver.o.d ${OBJECTDIR}/_ext/1472/clksys_driver.o.d ${OBJECTDIR}/_ext/1472/TC_driver.o.d ${OBJECTDIR}/_ext/1472/iq.o.d ${OBJECTDIR}/_ext/1472/usart_driver.o.d ${OBJECTDIR}/_ext/1472/adc_driver.o.d ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d ${OBJECTDIR}/_ext/1472/spi_driver.o.d ${OBJECTDIR}/_ext/1472/lowpower.o.d ${OBJECTDIR}/_ext/1472/rtc_driver.o.d ${OBJECTDIR}/_ext/1472/sleepmgr.o.d ${OBJECTDIR}/_ext/1472/ccpwrite.o.d ${OBJECTDIR}/_ext/1472/seg7.o.d ${OBJECTDIR}/_ext/1472/buffers.o.d ${OBJECTDIR}/_ext/1472/ulaw.o.d ${OBJECTDIR}/_ext/1472/eeprom.o.d ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d ${OBJECTDIR}/_ext/1472/crc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/sp_driver.o ${OBJECTDIR}/_ext/1472/rtos.o ${OBJECTDIR}/_ext/1472/wdt_driver.o ${OBJECTDIR}/_ext/1472/clksys_driver.o ${OBJECTDIR}/_ext/1472/TC_driver.o ${OBJECTDIR}/_ext/1472/iq.o ${OBJECTDIR}/_ext/1472/usart_driver.o ${OBJECTDIR}/_ext/1472/adc_driver.o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ${OBJECTDIR}/_ext/1472/spi_driver.o ${OBJECTDIR}/_ext/1472/lowpower.o ${OBJECTDIR}/_ext/1472/rtc_driver.o ${OBJECTDIR}/_ext/1472/sleepmgr.o ${OBJECTDIR}/_ext/1472/ccpwrite.o ${OBJECTDIR}/_ext/1472/seg7.o ${OBJECTDIR}/_ext/1472/buffers.o ${OBJECTDIR}/_ext/1472/ulaw.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ${OBJECTDIR}/_ext/1472/crc.o

# Source Files
SOURCEFILES=../main.c ../serial.c ../sp_driver.S ../rtos.c ../wdt_driver.c ../clksys_driver.c ../TC_driver.c ../iq.c ../usart_driver.c ../adc_driver.c ../eeprom_driver.c ../spi_driver.c ../lowpower.c ../rtc_driver.c ../sleepmgr.c ../ccpwrite.c ../seg7.c ../buffers.c ../ulaw.c ../eeprom.c ../TaskWatchDog.c ../crc.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/atxmega64a3"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Bluetooth.mk ${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATxmega64A3
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/sp_driver.o: ../sp_driver.S  .generated_files/flags/Bluetooth/ed80a5c5224f4cfe59d98e1e7eababb3b3f78cce .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.ok ${OBJECTDIR}/_ext/1472/sp_driver.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=atxmega64a3    -DDEBUG  -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/sp_driver.o -o ${OBJECTDIR}/_ext/1472/sp_driver.o ../sp_driver.S  -DXPRJ_Bluetooth=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/sp_driver.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1 -Wall -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
	
else
${OBJECTDIR}/_ext/1472/sp_driver.o: ../sp_driver.S  .generated_files/flags/Bluetooth/d9fff199884e1f6966c6d94bfbd081063b49402 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.ok ${OBJECTDIR}/_ext/1472/sp_driver.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=atxmega64a3    -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/sp_driver.o -o ${OBJECTDIR}/_ext/1472/sp_driver.o ../sp_driver.S  -DXPRJ_Bluetooth=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/sp_driver.o.asm.d" -Wall -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Bluetooth/85dde917d09d18ebfa57d195911e0bef38c57f6c .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  .generated_files/flags/Bluetooth/1335f0c42d85f3ba4a0af06b889566a4f41c6ce6 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -MT "${OBJECTDIR}/_ext/1472/serial.o.d" -MT ${OBJECTDIR}/_ext/1472/serial.o  -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/rtos.o: ../rtos.c  .generated_files/flags/Bluetooth/bd82c7ffea4fe3809ff4dd14c981bf7bc4a6bfdf .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT ${OBJECTDIR}/_ext/1472/rtos.o  -o ${OBJECTDIR}/_ext/1472/rtos.o ../rtos.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/wdt_driver.o: ../wdt_driver.c  .generated_files/flags/Bluetooth/bf577df989821df50981634af4fe82a2736196b1 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt_driver.o  -o ${OBJECTDIR}/_ext/1472/wdt_driver.o ../wdt_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/clksys_driver.o: ../clksys_driver.c  .generated_files/flags/Bluetooth/45ddb1de7fa75b231db58442709799f1f05f9b23 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/clksys_driver.o  -o ${OBJECTDIR}/_ext/1472/clksys_driver.o ../clksys_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/TC_driver.o: ../TC_driver.c  .generated_files/flags/Bluetooth/a2fdee5dd93082c988a63d4b12b6e656aa8446ab .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/TC_driver.o  -o ${OBJECTDIR}/_ext/1472/TC_driver.o ../TC_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/iq.o: ../iq.c  .generated_files/flags/Bluetooth/7408558aa1b2b7d39badfa6ec84c605607da2763 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/iq.o.d" -MT "${OBJECTDIR}/_ext/1472/iq.o.d" -MT ${OBJECTDIR}/_ext/1472/iq.o  -o ${OBJECTDIR}/_ext/1472/iq.o ../iq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/usart_driver.o: ../usart_driver.c  .generated_files/flags/Bluetooth/d8d3d54df0b342ba5105c380e595b4d6aa3d5221 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/usart_driver.o  -o ${OBJECTDIR}/_ext/1472/usart_driver.o ../usart_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/adc_driver.o: ../adc_driver.c  .generated_files/flags/Bluetooth/dc39da2e2cbf59d255299ed99ca964463847c4ec .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/adc_driver.o  -o ${OBJECTDIR}/_ext/1472/adc_driver.o ../adc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/eeprom_driver.o: ../eeprom_driver.c  .generated_files/flags/Bluetooth/b3215ae88599b48a75f466c7170dabe03612efe8 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom_driver.o  -o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ../eeprom_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/spi_driver.o: ../spi_driver.c  .generated_files/flags/Bluetooth/267a0a2ed43521898a0e1dfb8221ce6fb79be24e .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/spi_driver.o  -o ${OBJECTDIR}/_ext/1472/spi_driver.o ../spi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/lowpower.o: ../lowpower.c  .generated_files/flags/Bluetooth/2c0ff0a0d0823134e25f123c91bf512dd00a5375 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT ${OBJECTDIR}/_ext/1472/lowpower.o  -o ${OBJECTDIR}/_ext/1472/lowpower.o ../lowpower.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/rtc_driver.o: ../rtc_driver.c  .generated_files/flags/Bluetooth/90ee3d09b0ae2e8d5a9420a7ca36de7794a368aa .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/rtc_driver.o  -o ${OBJECTDIR}/_ext/1472/rtc_driver.o ../rtc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/sleepmgr.o: ../sleepmgr.c  .generated_files/flags/Bluetooth/57ccb548f97d2a52d1e3a9d2dca4bf78b1523b96 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT ${OBJECTDIR}/_ext/1472/sleepmgr.o  -o ${OBJECTDIR}/_ext/1472/sleepmgr.o ../sleepmgr.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/ccpwrite.o: ../ccpwrite.c  .generated_files/flags/Bluetooth/32a908577a3fdf27f8dc2c7e93319bcb7f3f0f3 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT ${OBJECTDIR}/_ext/1472/ccpwrite.o  -o ${OBJECTDIR}/_ext/1472/ccpwrite.o ../ccpwrite.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/seg7.o: ../seg7.c  .generated_files/flags/Bluetooth/59a80ef12d31a98d3f78fae9b020e73cc4766d55 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT ${OBJECTDIR}/_ext/1472/seg7.o  -o ${OBJECTDIR}/_ext/1472/seg7.o ../seg7.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/buffers.o: ../buffers.c  .generated_files/flags/Bluetooth/3941d4b1010cd103b405636e37be1ee40cd72f44 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT ${OBJECTDIR}/_ext/1472/buffers.o  -o ${OBJECTDIR}/_ext/1472/buffers.o ../buffers.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/ulaw.o: ../ulaw.c  .generated_files/flags/Bluetooth/23077f6cb995b3e94291b806d5bcd3d352737c6 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT ${OBJECTDIR}/_ext/1472/ulaw.o  -o ${OBJECTDIR}/_ext/1472/ulaw.o ../ulaw.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Bluetooth/daa5188957095dbabbfe827a750d4141f32d823b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/TaskWatchDog.o: ../TaskWatchDog.c  .generated_files/flags/Bluetooth/37e4b0c9f669ec13840decea00b60631b0f804dd .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskWatchDog.o  -o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ../TaskWatchDog.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/crc.o: ../crc.c  .generated_files/flags/Bluetooth/a98aaa1777acf65885d529154b12516e1458bf88 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/crc.o.d" -MT "${OBJECTDIR}/_ext/1472/crc.o.d" -MT ${OBJECTDIR}/_ext/1472/crc.o  -o ${OBJECTDIR}/_ext/1472/crc.o ../crc.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
else
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Bluetooth/c36c3073f8b5ff255dd06af1e438cf4583a22092 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  .generated_files/flags/Bluetooth/5c9654e070a3d70e6545f423825a2664ba6b11de .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -MT "${OBJECTDIR}/_ext/1472/serial.o.d" -MT ${OBJECTDIR}/_ext/1472/serial.o  -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/rtos.o: ../rtos.c  .generated_files/flags/Bluetooth/7c29a8f400e7ca7c59753e106d5127abecf9c897 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT ${OBJECTDIR}/_ext/1472/rtos.o  -o ${OBJECTDIR}/_ext/1472/rtos.o ../rtos.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/wdt_driver.o: ../wdt_driver.c  .generated_files/flags/Bluetooth/f6078a718f2cdecb041184d4105d2fdb0d169b69 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt_driver.o  -o ${OBJECTDIR}/_ext/1472/wdt_driver.o ../wdt_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/clksys_driver.o: ../clksys_driver.c  .generated_files/flags/Bluetooth/3e2a8a7976109eca2bbac4e2acfa6f3aa2249bbc .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/clksys_driver.o  -o ${OBJECTDIR}/_ext/1472/clksys_driver.o ../clksys_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/TC_driver.o: ../TC_driver.c  .generated_files/flags/Bluetooth/46acb7706aafd2128cb1edd1b37ba54fef322e40 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/TC_driver.o  -o ${OBJECTDIR}/_ext/1472/TC_driver.o ../TC_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/iq.o: ../iq.c  .generated_files/flags/Bluetooth/5b6148978b10eb62675a971fa130e45016d98dc4 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/iq.o.d" -MT "${OBJECTDIR}/_ext/1472/iq.o.d" -MT ${OBJECTDIR}/_ext/1472/iq.o  -o ${OBJECTDIR}/_ext/1472/iq.o ../iq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/usart_driver.o: ../usart_driver.c  .generated_files/flags/Bluetooth/2db9b5a69c9458746ed3cc0ac769193d3afb5447 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/usart_driver.o  -o ${OBJECTDIR}/_ext/1472/usart_driver.o ../usart_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/adc_driver.o: ../adc_driver.c  .generated_files/flags/Bluetooth/9723ed70a629c507204915c1fa225ca43e43f47 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/adc_driver.o  -o ${OBJECTDIR}/_ext/1472/adc_driver.o ../adc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/eeprom_driver.o: ../eeprom_driver.c  .generated_files/flags/Bluetooth/b43e9a3f56d6fe1d885e15f286c51734ec1d7716 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom_driver.o  -o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ../eeprom_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/spi_driver.o: ../spi_driver.c  .generated_files/flags/Bluetooth/4685dde947033593813885395887a833e863d0bc .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/spi_driver.o  -o ${OBJECTDIR}/_ext/1472/spi_driver.o ../spi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/lowpower.o: ../lowpower.c  .generated_files/flags/Bluetooth/bc876fc2a959f00369bc358198c26c853e8feeac .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT ${OBJECTDIR}/_ext/1472/lowpower.o  -o ${OBJECTDIR}/_ext/1472/lowpower.o ../lowpower.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/rtc_driver.o: ../rtc_driver.c  .generated_files/flags/Bluetooth/af05d297029077dc935f4e0a6a54b0ea086c2f5 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/rtc_driver.o  -o ${OBJECTDIR}/_ext/1472/rtc_driver.o ../rtc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/sleepmgr.o: ../sleepmgr.c  .generated_files/flags/Bluetooth/6480381a350277ecdec31addead203a0b97bfc0a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT ${OBJECTDIR}/_ext/1472/sleepmgr.o  -o ${OBJECTDIR}/_ext/1472/sleepmgr.o ../sleepmgr.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/ccpwrite.o: ../ccpwrite.c  .generated_files/flags/Bluetooth/efc67876bbf48100ecf9deac8fa402f0d1fe815c .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT ${OBJECTDIR}/_ext/1472/ccpwrite.o  -o ${OBJECTDIR}/_ext/1472/ccpwrite.o ../ccpwrite.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/seg7.o: ../seg7.c  .generated_files/flags/Bluetooth/b26d0c0b0493676168c502f035ffdf4e71e2280a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT ${OBJECTDIR}/_ext/1472/seg7.o  -o ${OBJECTDIR}/_ext/1472/seg7.o ../seg7.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/buffers.o: ../buffers.c  .generated_files/flags/Bluetooth/ec197faf38ac56e0ff23b439380cb4cf2258aec4 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT ${OBJECTDIR}/_ext/1472/buffers.o  -o ${OBJECTDIR}/_ext/1472/buffers.o ../buffers.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/ulaw.o: ../ulaw.c  .generated_files/flags/Bluetooth/1dc80368ecbab807a3a7db4f4d16df7871fbdee6 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT ${OBJECTDIR}/_ext/1472/ulaw.o  -o ${OBJECTDIR}/_ext/1472/ulaw.o ../ulaw.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Bluetooth/12367ae258fed2dcf9277014e0c9fa7bba1fee0c .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
${OBJECTDIR}/_ext/1472/TaskWatchDog.o: ../TaskWatchDog.c  .generated_files/flags/Bluetooth/f853c834dcccb083abbc766b9c9e1d3253ddbf .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskWatchDog.o  -o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ../TaskWatchDog.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/crc.o: ../crc.c  .generated_files/flags/Bluetooth/1b229b2f2c4bca6362534b2c526d9c9a00aaa007 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/crc.o.d" -MT "${OBJECTDIR}/_ext/1472/crc.o.d" -MT ${OBJECTDIR}/_ext/1472/crc.o  -o ${OBJECTDIR}/_ext/1472/crc.o ../crc.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)         
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atxmega64a3    -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lprintf_flt -Wl,-lscanf_flt -Wl,-lm -Wl,--end-group  -Wl,-u,vfscanf -Wl,-u,vfprintf
	
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}/avr-objdump -h -S "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.srec"
	
	
else
${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atxmega64a3   -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lprintf_flt -Wl,-lscanf_flt -Wl,-lm -Wl,--end-group  -Wl,-u,vfscanf -Wl,-u,vfprintf
	${MP_CC_DIR}/avr-objcopy -O ihex "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.hex"
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}/avr-objdump -h -S "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/TestBluetooth.X.${IMAGE_TYPE}.srec"
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
