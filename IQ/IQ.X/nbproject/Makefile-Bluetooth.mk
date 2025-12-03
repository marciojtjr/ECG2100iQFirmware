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
FINAL_IMAGE=${DISTDIR}/IQ.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/IQ.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../BGAPI/bgapi_commands.c ../BGAPI/bgapi_parser.c ../apple.c ../ffft.S ../main.c ../serial.c ../sp_driver.s ../taskPaceSimu.c ../rtos.c ../wdt_driver.c ../clksys_driver.c ../TC_driver.c ../iq.c ../TaskAcq.c ../usart_driver.c ../adc_driver.c ../eeprom_driver.c ../TaskUSB.c ../TaskFlash.c ../spi_driver.c ../lowpower.c ../rtc_driver.c ../sleepmgr.c ../ccpwrite.c ../seg7.c ../buffers.c ../ulaw.c ../eeprom.c ../TaskBT.c ../TaskWatchDog.c ../crc.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ${OBJECTDIR}/_ext/1472/apple.o ${OBJECTDIR}/_ext/1472/ffft.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/sp_driver.o ${OBJECTDIR}/_ext/1472/taskPaceSimu.o ${OBJECTDIR}/_ext/1472/rtos.o ${OBJECTDIR}/_ext/1472/wdt_driver.o ${OBJECTDIR}/_ext/1472/clksys_driver.o ${OBJECTDIR}/_ext/1472/TC_driver.o ${OBJECTDIR}/_ext/1472/iq.o ${OBJECTDIR}/_ext/1472/TaskAcq.o ${OBJECTDIR}/_ext/1472/usart_driver.o ${OBJECTDIR}/_ext/1472/adc_driver.o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ${OBJECTDIR}/_ext/1472/TaskUSB.o ${OBJECTDIR}/_ext/1472/TaskFlash.o ${OBJECTDIR}/_ext/1472/spi_driver.o ${OBJECTDIR}/_ext/1472/lowpower.o ${OBJECTDIR}/_ext/1472/rtc_driver.o ${OBJECTDIR}/_ext/1472/sleepmgr.o ${OBJECTDIR}/_ext/1472/ccpwrite.o ${OBJECTDIR}/_ext/1472/seg7.o ${OBJECTDIR}/_ext/1472/buffers.o ${OBJECTDIR}/_ext/1472/ulaw.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/TaskBT.o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ${OBJECTDIR}/_ext/1472/crc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d ${OBJECTDIR}/_ext/1472/apple.o.d ${OBJECTDIR}/_ext/1472/ffft.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/serial.o.d ${OBJECTDIR}/_ext/1472/sp_driver.o.d ${OBJECTDIR}/_ext/1472/taskPaceSimu.o.d ${OBJECTDIR}/_ext/1472/rtos.o.d ${OBJECTDIR}/_ext/1472/wdt_driver.o.d ${OBJECTDIR}/_ext/1472/clksys_driver.o.d ${OBJECTDIR}/_ext/1472/TC_driver.o.d ${OBJECTDIR}/_ext/1472/iq.o.d ${OBJECTDIR}/_ext/1472/TaskAcq.o.d ${OBJECTDIR}/_ext/1472/usart_driver.o.d ${OBJECTDIR}/_ext/1472/adc_driver.o.d ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d ${OBJECTDIR}/_ext/1472/TaskUSB.o.d ${OBJECTDIR}/_ext/1472/TaskFlash.o.d ${OBJECTDIR}/_ext/1472/spi_driver.o.d ${OBJECTDIR}/_ext/1472/lowpower.o.d ${OBJECTDIR}/_ext/1472/rtc_driver.o.d ${OBJECTDIR}/_ext/1472/sleepmgr.o.d ${OBJECTDIR}/_ext/1472/ccpwrite.o.d ${OBJECTDIR}/_ext/1472/seg7.o.d ${OBJECTDIR}/_ext/1472/buffers.o.d ${OBJECTDIR}/_ext/1472/ulaw.o.d ${OBJECTDIR}/_ext/1472/eeprom.o.d ${OBJECTDIR}/_ext/1472/TaskBT.o.d ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d ${OBJECTDIR}/_ext/1472/crc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ${OBJECTDIR}/_ext/1472/apple.o ${OBJECTDIR}/_ext/1472/ffft.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/sp_driver.o ${OBJECTDIR}/_ext/1472/taskPaceSimu.o ${OBJECTDIR}/_ext/1472/rtos.o ${OBJECTDIR}/_ext/1472/wdt_driver.o ${OBJECTDIR}/_ext/1472/clksys_driver.o ${OBJECTDIR}/_ext/1472/TC_driver.o ${OBJECTDIR}/_ext/1472/iq.o ${OBJECTDIR}/_ext/1472/TaskAcq.o ${OBJECTDIR}/_ext/1472/usart_driver.o ${OBJECTDIR}/_ext/1472/adc_driver.o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ${OBJECTDIR}/_ext/1472/TaskUSB.o ${OBJECTDIR}/_ext/1472/TaskFlash.o ${OBJECTDIR}/_ext/1472/spi_driver.o ${OBJECTDIR}/_ext/1472/lowpower.o ${OBJECTDIR}/_ext/1472/rtc_driver.o ${OBJECTDIR}/_ext/1472/sleepmgr.o ${OBJECTDIR}/_ext/1472/ccpwrite.o ${OBJECTDIR}/_ext/1472/seg7.o ${OBJECTDIR}/_ext/1472/buffers.o ${OBJECTDIR}/_ext/1472/ulaw.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/TaskBT.o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ${OBJECTDIR}/_ext/1472/crc.o

# Source Files
SOURCEFILES=../BGAPI/bgapi_commands.c ../BGAPI/bgapi_parser.c ../apple.c ../ffft.S ../main.c ../serial.c ../sp_driver.s ../taskPaceSimu.c ../rtos.c ../wdt_driver.c ../clksys_driver.c ../TC_driver.c ../iq.c ../TaskAcq.c ../usart_driver.c ../adc_driver.c ../eeprom_driver.c ../TaskUSB.c ../TaskFlash.c ../spi_driver.c ../lowpower.c ../rtc_driver.c ../sleepmgr.c ../ccpwrite.c ../seg7.c ../buffers.c ../ulaw.c ../eeprom.c ../TaskBT.c ../TaskWatchDog.c ../crc.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/atxmega64a3u"



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
	${MAKE}  -f nbproject/Makefile-Bluetooth.mk ${DISTDIR}/IQ.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATxmega64A3U
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/sp_driver.o: ../sp_driver.s  .generated_files/flags/Bluetooth/b11c914cc52345dddda45a99503e324777695805 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.ok ${OBJECTDIR}/_ext/1472/sp_driver.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=atxmega64a3u    -DDEBUG   -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/sp_driver.o  -o ${OBJECTDIR}/_ext/1472/sp_driver.o ../sp_driver.s  -DXPRJ_Bluetooth=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/sp_driver.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--gdwarf-2 -Wall -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
	
else
${OBJECTDIR}/_ext/1472/sp_driver.o: ../sp_driver.s  .generated_files/flags/Bluetooth/4198128b4e6d738650316b7ff2dea1db02d1f9f8 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o 
	@${RM} ${OBJECTDIR}/_ext/1472/sp_driver.o.ok ${OBJECTDIR}/_ext/1472/sp_driver.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=atxmega64a3u    -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/sp_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/sp_driver.o  -o ${OBJECTDIR}/_ext/1472/sp_driver.o ../sp_driver.s  -DXPRJ_Bluetooth=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/sp_driver.o.asm.d" -Wall -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/ffft.o: ../ffft.S  .generated_files/flags/Bluetooth/cd5e0d295f7389dc2854ec208156104fb051c6a2 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ffft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ffft.o 
	@${RM} ${OBJECTDIR}/_ext/1472/ffft.o.ok ${OBJECTDIR}/_ext/1472/ffft.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=atxmega64a3u    -DDEBUG  -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/_ext/1472/ffft.o.d" -MT "${OBJECTDIR}/_ext/1472/ffft.o.d" -MT ${OBJECTDIR}/_ext/1472/ffft.o -o ${OBJECTDIR}/_ext/1472/ffft.o ../ffft.S  -DXPRJ_Bluetooth=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/ffft.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1 -Wall -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
	
else
${OBJECTDIR}/_ext/1472/ffft.o: ../ffft.S  .generated_files/flags/Bluetooth/b00126e5a09b8d65855d20641128e1b585ae2a99 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ffft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ffft.o 
	@${RM} ${OBJECTDIR}/_ext/1472/ffft.o.ok ${OBJECTDIR}/_ext/1472/ffft.o.err 
	 ${MP_CC} $(MP_EXTRA_AS_PRE) -mmcu=atxmega64a3u    -x assembler-with-cpp -c -D__$(MP_PROCESSOR_OPTION)__  -MD -MP -MF "${OBJECTDIR}/_ext/1472/ffft.o.d" -MT "${OBJECTDIR}/_ext/1472/ffft.o.d" -MT ${OBJECTDIR}/_ext/1472/ffft.o -o ${OBJECTDIR}/_ext/1472/ffft.o ../ffft.S  -DXPRJ_Bluetooth=$(CND_CONF)  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/ffft.o.asm.d" -Wall -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -gdwarf-2 -std=gnu99 -DBLUETOOTH=1        -DF_CPU=32000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/2144063110/bgapi_commands.o: ../BGAPI/bgapi_commands.c  .generated_files/flags/Bluetooth/40b497058d12d24857336cb52ba955dca0acc129 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ../BGAPI/bgapi_commands.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/bgapi_parser.o: ../BGAPI/bgapi_parser.c  .generated_files/flags/Bluetooth/96e9543216fbecc985ee26ea3da9d25bec18b07d .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ../BGAPI/bgapi_parser.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/apple.o: ../apple.c  .generated_files/flags/Bluetooth/929fa071283176991b9f42005f7714a5e12b4e7a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/apple.o.d" -MT "${OBJECTDIR}/_ext/1472/apple.o.d" -MT ${OBJECTDIR}/_ext/1472/apple.o  -o ${OBJECTDIR}/_ext/1472/apple.o ../apple.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Bluetooth/28673c9def2536708d1df0d3f1aae2e63e2b58e9 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  .generated_files/flags/Bluetooth/967f8d72a6b80de08f760fe67b3db6acab055bb5 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -MT "${OBJECTDIR}/_ext/1472/serial.o.d" -MT ${OBJECTDIR}/_ext/1472/serial.o  -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/taskPaceSimu.o: ../taskPaceSimu.c  .generated_files/flags/Bluetooth/1e7425ac93c6e635fa3e0d0a46bd8f11064599dd .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/taskPaceSimu.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/taskPaceSimu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/taskPaceSimu.o.d" -MT "${OBJECTDIR}/_ext/1472/taskPaceSimu.o.d" -MT ${OBJECTDIR}/_ext/1472/taskPaceSimu.o  -o ${OBJECTDIR}/_ext/1472/taskPaceSimu.o ../taskPaceSimu.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/rtos.o: ../rtos.c  .generated_files/flags/Bluetooth/f9d2ab5f698472ed0c7b6131e497687d9ec5382a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT ${OBJECTDIR}/_ext/1472/rtos.o  -o ${OBJECTDIR}/_ext/1472/rtos.o ../rtos.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/wdt_driver.o: ../wdt_driver.c  .generated_files/flags/Bluetooth/6cb53e45fdb0331b862cdb3bc1561e347658276c .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt_driver.o  -o ${OBJECTDIR}/_ext/1472/wdt_driver.o ../wdt_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/clksys_driver.o: ../clksys_driver.c  .generated_files/flags/Bluetooth/d94a913346333377107dc5677e971ad8eed14fe9 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/clksys_driver.o  -o ${OBJECTDIR}/_ext/1472/clksys_driver.o ../clksys_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TC_driver.o: ../TC_driver.c  .generated_files/flags/Bluetooth/c5a6ecddf8424a84a5a6fdc7929e77105cc522c8 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/TC_driver.o  -o ${OBJECTDIR}/_ext/1472/TC_driver.o ../TC_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/iq.o: ../iq.c  .generated_files/flags/Bluetooth/f963de4cec4a7d5a2304e3177021ab139c0db85a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/iq.o.d" -MT "${OBJECTDIR}/_ext/1472/iq.o.d" -MT ${OBJECTDIR}/_ext/1472/iq.o  -o ${OBJECTDIR}/_ext/1472/iq.o ../iq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskAcq.o: ../TaskAcq.c  .generated_files/flags/Bluetooth/f5b1599d3326b44a23d9d9d266b8ce6a21a70d9f .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskAcq.o  -o ${OBJECTDIR}/_ext/1472/TaskAcq.o ../TaskAcq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/usart_driver.o: ../usart_driver.c  .generated_files/flags/Bluetooth/ede2caa9fb38a6533d6d133615769d883833d7c9 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/usart_driver.o  -o ${OBJECTDIR}/_ext/1472/usart_driver.o ../usart_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/adc_driver.o: ../adc_driver.c  .generated_files/flags/Bluetooth/b1b5eb85ee79191c032a5d61b8474a76f33ffb63 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/adc_driver.o  -o ${OBJECTDIR}/_ext/1472/adc_driver.o ../adc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom_driver.o: ../eeprom_driver.c  .generated_files/flags/Bluetooth/4e2356d29123bc77fe3ac1992a5d551d5bb2b70a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom_driver.o  -o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ../eeprom_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskUSB.o: ../TaskUSB.c  .generated_files/flags/Bluetooth/d8eb05c33a6e639840f4a70bcdc90877a622c840 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskUSB.o  -o ${OBJECTDIR}/_ext/1472/TaskUSB.o ../TaskUSB.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskFlash.o: ../TaskFlash.c  .generated_files/flags/Bluetooth/f9dc89ae9dbc044962c880aec4956bde9a251dbf .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskFlash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskFlash.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskFlash.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskFlash.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskFlash.o  -o ${OBJECTDIR}/_ext/1472/TaskFlash.o ../TaskFlash.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/spi_driver.o: ../spi_driver.c  .generated_files/flags/Bluetooth/b03f17e7dfa83938c3207ea8210bd596fcbf863d .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/spi_driver.o  -o ${OBJECTDIR}/_ext/1472/spi_driver.o ../spi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/lowpower.o: ../lowpower.c  .generated_files/flags/Bluetooth/5186608b59c2989bf7c9f16d36cc9802ec193d3b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT ${OBJECTDIR}/_ext/1472/lowpower.o  -o ${OBJECTDIR}/_ext/1472/lowpower.o ../lowpower.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/rtc_driver.o: ../rtc_driver.c  .generated_files/flags/Bluetooth/9a411af9ac646efd120630c17a776d8e9d8a7256 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/rtc_driver.o  -o ${OBJECTDIR}/_ext/1472/rtc_driver.o ../rtc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/sleepmgr.o: ../sleepmgr.c  .generated_files/flags/Bluetooth/e0630c40fd976b777fe446aef099f4f68ee6316b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT ${OBJECTDIR}/_ext/1472/sleepmgr.o  -o ${OBJECTDIR}/_ext/1472/sleepmgr.o ../sleepmgr.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ccpwrite.o: ../ccpwrite.c  .generated_files/flags/Bluetooth/9c7cb6fe09055b36a13c1171feeca13ab1d9b4bc .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT ${OBJECTDIR}/_ext/1472/ccpwrite.o  -o ${OBJECTDIR}/_ext/1472/ccpwrite.o ../ccpwrite.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/seg7.o: ../seg7.c  .generated_files/flags/Bluetooth/174989b343a6b66e520dc6bc42d329358bec0c8d .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT ${OBJECTDIR}/_ext/1472/seg7.o  -o ${OBJECTDIR}/_ext/1472/seg7.o ../seg7.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/buffers.o: ../buffers.c  .generated_files/flags/Bluetooth/9105ae730c44eca410855960b63b4a61033b541a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT ${OBJECTDIR}/_ext/1472/buffers.o  -o ${OBJECTDIR}/_ext/1472/buffers.o ../buffers.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ulaw.o: ../ulaw.c  .generated_files/flags/Bluetooth/738be26e0d6561e92ffe850a3c475ff8d7ee4cc4 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT ${OBJECTDIR}/_ext/1472/ulaw.o  -o ${OBJECTDIR}/_ext/1472/ulaw.o ../ulaw.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Bluetooth/40e83ec18b07fbc7514f626d18ebdd748718f5b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskBT.o: ../TaskBT.c  .generated_files/flags/Bluetooth/8802617e2255945d94fe0f9aafdd36f6840c2006 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskBT.o  -o ${OBJECTDIR}/_ext/1472/TaskBT.o ../TaskBT.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskWatchDog.o: ../TaskWatchDog.c  .generated_files/flags/Bluetooth/3d176c9bf151b691fdc7e9952aef91201f85c08e .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskWatchDog.o  -o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ../TaskWatchDog.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/crc.o: ../crc.c  .generated_files/flags/Bluetooth/766e4df8757ec871e2df332d722ca67a22c9a4c6 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/crc.o.d" -MT "${OBJECTDIR}/_ext/1472/crc.o.d" -MT ${OBJECTDIR}/_ext/1472/crc.o  -o ${OBJECTDIR}/_ext/1472/crc.o ../crc.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/2144063110/bgapi_commands.o: ../BGAPI/bgapi_commands.c  .generated_files/flags/Bluetooth/ea777cf73fdaefc310270def9aa00789e107fdac .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ../BGAPI/bgapi_commands.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/bgapi_parser.o: ../BGAPI/bgapi_parser.c  .generated_files/flags/Bluetooth/1f44066475d888f10b12f69defb29bff5b05472f .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ../BGAPI/bgapi_parser.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/apple.o: ../apple.c  .generated_files/flags/Bluetooth/9453845b2ae46923ad701436a96aa9a65ab0d203 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/apple.o.d" -MT "${OBJECTDIR}/_ext/1472/apple.o.d" -MT ${OBJECTDIR}/_ext/1472/apple.o  -o ${OBJECTDIR}/_ext/1472/apple.o ../apple.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Bluetooth/df056fff26be783ad1f442966b51c058f7f0a496 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  .generated_files/flags/Bluetooth/7138bf03a6503b10615027c9e1fdc0a155c9c0e1 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -MT "${OBJECTDIR}/_ext/1472/serial.o.d" -MT ${OBJECTDIR}/_ext/1472/serial.o  -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/taskPaceSimu.o: ../taskPaceSimu.c  .generated_files/flags/Bluetooth/d7501382ce58c9491128f60f277411b67cd2add9 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/taskPaceSimu.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/taskPaceSimu.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/taskPaceSimu.o.d" -MT "${OBJECTDIR}/_ext/1472/taskPaceSimu.o.d" -MT ${OBJECTDIR}/_ext/1472/taskPaceSimu.o  -o ${OBJECTDIR}/_ext/1472/taskPaceSimu.o ../taskPaceSimu.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/rtos.o: ../rtos.c  .generated_files/flags/Bluetooth/32f0b80a82d2707576c8cf419554896d06e7ac11 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT ${OBJECTDIR}/_ext/1472/rtos.o  -o ${OBJECTDIR}/_ext/1472/rtos.o ../rtos.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/wdt_driver.o: ../wdt_driver.c  .generated_files/flags/Bluetooth/fabb23deffb93ebbaca51ae496687363d9011257 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt_driver.o  -o ${OBJECTDIR}/_ext/1472/wdt_driver.o ../wdt_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/clksys_driver.o: ../clksys_driver.c  .generated_files/flags/Bluetooth/5b2533badb206b51c9f7a424c3c487540fa37d1f .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/clksys_driver.o  -o ${OBJECTDIR}/_ext/1472/clksys_driver.o ../clksys_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TC_driver.o: ../TC_driver.c  .generated_files/flags/Bluetooth/18c284f80b791521879333507f5cfda2498d0cf6 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/TC_driver.o  -o ${OBJECTDIR}/_ext/1472/TC_driver.o ../TC_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/iq.o: ../iq.c  .generated_files/flags/Bluetooth/7cdfa3403e030b5f7b5e464ab143f6f86d918391 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/iq.o.d" -MT "${OBJECTDIR}/_ext/1472/iq.o.d" -MT ${OBJECTDIR}/_ext/1472/iq.o  -o ${OBJECTDIR}/_ext/1472/iq.o ../iq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskAcq.o: ../TaskAcq.c  .generated_files/flags/Bluetooth/7d41cd6860047a8cf7ae6eef2474b313a7abaa64 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskAcq.o  -o ${OBJECTDIR}/_ext/1472/TaskAcq.o ../TaskAcq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/usart_driver.o: ../usart_driver.c  .generated_files/flags/Bluetooth/187152f14ede5dd9cf7b0d57bd1050c61b7bb6c5 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/usart_driver.o  -o ${OBJECTDIR}/_ext/1472/usart_driver.o ../usart_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/adc_driver.o: ../adc_driver.c  .generated_files/flags/Bluetooth/ec6f03ab09cfb226f0b98566f9c20d1854264ee .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/adc_driver.o  -o ${OBJECTDIR}/_ext/1472/adc_driver.o ../adc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom_driver.o: ../eeprom_driver.c  .generated_files/flags/Bluetooth/c1fb58281a8a61e2b12fa7e9104614f81f32920f .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom_driver.o  -o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ../eeprom_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskUSB.o: ../TaskUSB.c  .generated_files/flags/Bluetooth/11ad722ceca29627163f24712617da468bd92363 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskUSB.o  -o ${OBJECTDIR}/_ext/1472/TaskUSB.o ../TaskUSB.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskFlash.o: ../TaskFlash.c  .generated_files/flags/Bluetooth/772fa9088026238ddc6e6af7ae8762f23d615f74 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskFlash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskFlash.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskFlash.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskFlash.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskFlash.o  -o ${OBJECTDIR}/_ext/1472/TaskFlash.o ../TaskFlash.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/spi_driver.o: ../spi_driver.c  .generated_files/flags/Bluetooth/2c07c0a827db4555eb26f30e0ac567beab455f2b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/spi_driver.o  -o ${OBJECTDIR}/_ext/1472/spi_driver.o ../spi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/lowpower.o: ../lowpower.c  .generated_files/flags/Bluetooth/538c2d72c0369572e1babed4976f4df8bef93b93 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT ${OBJECTDIR}/_ext/1472/lowpower.o  -o ${OBJECTDIR}/_ext/1472/lowpower.o ../lowpower.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/rtc_driver.o: ../rtc_driver.c  .generated_files/flags/Bluetooth/df04deaa4f902f740fcc4964a74c0a4cac2e974a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/rtc_driver.o  -o ${OBJECTDIR}/_ext/1472/rtc_driver.o ../rtc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/sleepmgr.o: ../sleepmgr.c  .generated_files/flags/Bluetooth/700c3cabc8ed9c03f282b73f6728baec682ee5c9 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT ${OBJECTDIR}/_ext/1472/sleepmgr.o  -o ${OBJECTDIR}/_ext/1472/sleepmgr.o ../sleepmgr.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ccpwrite.o: ../ccpwrite.c  .generated_files/flags/Bluetooth/e94747754c14ead37d8deef8b33cd493086f1fea .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT ${OBJECTDIR}/_ext/1472/ccpwrite.o  -o ${OBJECTDIR}/_ext/1472/ccpwrite.o ../ccpwrite.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/seg7.o: ../seg7.c  .generated_files/flags/Bluetooth/742147aa6003c930e94188766d7d5e7f9009d64d .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT ${OBJECTDIR}/_ext/1472/seg7.o  -o ${OBJECTDIR}/_ext/1472/seg7.o ../seg7.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/buffers.o: ../buffers.c  .generated_files/flags/Bluetooth/2f8a1fde2b66de31b8f3b0e8be511b142a469efe .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT ${OBJECTDIR}/_ext/1472/buffers.o  -o ${OBJECTDIR}/_ext/1472/buffers.o ../buffers.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ulaw.o: ../ulaw.c  .generated_files/flags/Bluetooth/c84fc65a27fb5f2130c29c959e48e628fda803b2 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT ${OBJECTDIR}/_ext/1472/ulaw.o  -o ${OBJECTDIR}/_ext/1472/ulaw.o ../ulaw.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Bluetooth/9075cc48a75a208583174c923db3ee5312fe5a7b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskBT.o: ../TaskBT.c  .generated_files/flags/Bluetooth/5b8f43c1d5c756dd5c987a8f7e88f56592a94b70 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskBT.o  -o ${OBJECTDIR}/_ext/1472/TaskBT.o ../TaskBT.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskWatchDog.o: ../TaskWatchDog.c  .generated_files/flags/Bluetooth/b7301e5707d3c524a266a74093f00762395fda99 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskWatchDog.o  -o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ../TaskWatchDog.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/crc.o: ../crc.c  .generated_files/flags/Bluetooth/d1c46cfa9afa41a5492974a62c7ad797e20c390c .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DF_CPU=32000000UL -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/crc.o.d" -MT "${OBJECTDIR}/_ext/1472/crc.o.d" -MT ${OBJECTDIR}/_ext/1472/crc.o  -o ${OBJECTDIR}/_ext/1472/crc.o ../crc.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/IQ.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atxmega64a3u    -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/IQ.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/IQ.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lprintf_flt -Wl,-lscanf_flt -Wl,-lm -Wl,--end-group  -Wl,-u,vfscanf -Wl,-u,vfprintf
	
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/IQ.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}/avr-objdump -h -S "${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "${DISTDIR}/IQ.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/IQ.X.${IMAGE_TYPE}.srec"
	
	
else
${DISTDIR}/IQ.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atxmega64a3u   -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="${DISTDIR}/IQ.X.${IMAGE_TYPE}.map"    -o ${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lprintf_flt -Wl,-lscanf_flt -Wl,-lm -Wl,--end-group  -Wl,-u,vfscanf -Wl,-u,vfprintf
	${MP_CC_DIR}/avr-objcopy -O ihex "${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/IQ.X.${IMAGE_TYPE}.hex"
	${MP_CC_DIR}/avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/IQ.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}/avr-objdump -h -S "${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "${DISTDIR}/IQ.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}/avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/IQ.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/IQ.X.${IMAGE_TYPE}.srec"
	
	
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
