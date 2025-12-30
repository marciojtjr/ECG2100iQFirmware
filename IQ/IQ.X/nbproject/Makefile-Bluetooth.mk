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
SOURCEFILES_QUOTED_IF_SPACED=../BGAPI/bgapi_commands.c ../BGAPI/bgapi_parser.c ../BGAPI/BT122_handler.c ../BGAPI/legacybt_handler.c ../apple.c ../ffft.S ../main.c ../serial.c ../sp_driver.s ../rtos.c ../wdt_driver.c ../clksys_driver.c ../TC_driver.c ../iq.c ../TaskAcq.c ../usart_driver.c ../adc_driver.c ../eeprom_driver.c ../TaskUSB.c ../spi_driver.c ../lowpower.c ../rtc_driver.c ../sleepmgr.c ../ccpwrite.c ../seg7.c ../buffers.c ../ulaw.c ../eeprom.c ../TaskBT.c ../TaskWatchDog.c ../crc.c ../TaskCC.c ../twi_driver.c ../ltc2959.c ../src/ASF/xmega/drivers/twi/twim.c ../src/ASF/xmega/drivers/twi/twis.c ../src/ASF/xmega/drivers/nvm/nvm.c ../src/ASF/common/services/clock/xmega/sysclk.c ../update_handler.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ${OBJECTDIR}/_ext/2144063110/BT122_handler.o ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o ${OBJECTDIR}/_ext/1472/apple.o ${OBJECTDIR}/_ext/1472/ffft.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/sp_driver.o ${OBJECTDIR}/_ext/1472/rtos.o ${OBJECTDIR}/_ext/1472/wdt_driver.o ${OBJECTDIR}/_ext/1472/clksys_driver.o ${OBJECTDIR}/_ext/1472/TC_driver.o ${OBJECTDIR}/_ext/1472/iq.o ${OBJECTDIR}/_ext/1472/TaskAcq.o ${OBJECTDIR}/_ext/1472/usart_driver.o ${OBJECTDIR}/_ext/1472/adc_driver.o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ${OBJECTDIR}/_ext/1472/TaskUSB.o ${OBJECTDIR}/_ext/1472/spi_driver.o ${OBJECTDIR}/_ext/1472/lowpower.o ${OBJECTDIR}/_ext/1472/rtc_driver.o ${OBJECTDIR}/_ext/1472/sleepmgr.o ${OBJECTDIR}/_ext/1472/ccpwrite.o ${OBJECTDIR}/_ext/1472/seg7.o ${OBJECTDIR}/_ext/1472/buffers.o ${OBJECTDIR}/_ext/1472/ulaw.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/TaskBT.o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ${OBJECTDIR}/_ext/1472/crc.o ${OBJECTDIR}/_ext/1472/TaskCC.o ${OBJECTDIR}/_ext/1472/twi_driver.o ${OBJECTDIR}/_ext/1472/ltc2959.o ${OBJECTDIR}/_ext/900742808/twim.o ${OBJECTDIR}/_ext/900742808/twis.o ${OBJECTDIR}/_ext/900748601/nvm.o ${OBJECTDIR}/_ext/1210819865/sysclk.o ${OBJECTDIR}/_ext/1472/update_handler.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d ${OBJECTDIR}/_ext/2144063110/BT122_handler.o.d ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o.d ${OBJECTDIR}/_ext/1472/apple.o.d ${OBJECTDIR}/_ext/1472/ffft.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/serial.o.d ${OBJECTDIR}/_ext/1472/sp_driver.o.d ${OBJECTDIR}/_ext/1472/rtos.o.d ${OBJECTDIR}/_ext/1472/wdt_driver.o.d ${OBJECTDIR}/_ext/1472/clksys_driver.o.d ${OBJECTDIR}/_ext/1472/TC_driver.o.d ${OBJECTDIR}/_ext/1472/iq.o.d ${OBJECTDIR}/_ext/1472/TaskAcq.o.d ${OBJECTDIR}/_ext/1472/usart_driver.o.d ${OBJECTDIR}/_ext/1472/adc_driver.o.d ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d ${OBJECTDIR}/_ext/1472/TaskUSB.o.d ${OBJECTDIR}/_ext/1472/spi_driver.o.d ${OBJECTDIR}/_ext/1472/lowpower.o.d ${OBJECTDIR}/_ext/1472/rtc_driver.o.d ${OBJECTDIR}/_ext/1472/sleepmgr.o.d ${OBJECTDIR}/_ext/1472/ccpwrite.o.d ${OBJECTDIR}/_ext/1472/seg7.o.d ${OBJECTDIR}/_ext/1472/buffers.o.d ${OBJECTDIR}/_ext/1472/ulaw.o.d ${OBJECTDIR}/_ext/1472/eeprom.o.d ${OBJECTDIR}/_ext/1472/TaskBT.o.d ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d ${OBJECTDIR}/_ext/1472/crc.o.d ${OBJECTDIR}/_ext/1472/TaskCC.o.d ${OBJECTDIR}/_ext/1472/twi_driver.o.d ${OBJECTDIR}/_ext/1472/ltc2959.o.d ${OBJECTDIR}/_ext/900742808/twim.o.d ${OBJECTDIR}/_ext/900742808/twis.o.d ${OBJECTDIR}/_ext/900748601/nvm.o.d ${OBJECTDIR}/_ext/1210819865/sysclk.o.d ${OBJECTDIR}/_ext/1472/update_handler.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ${OBJECTDIR}/_ext/2144063110/BT122_handler.o ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o ${OBJECTDIR}/_ext/1472/apple.o ${OBJECTDIR}/_ext/1472/ffft.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/sp_driver.o ${OBJECTDIR}/_ext/1472/rtos.o ${OBJECTDIR}/_ext/1472/wdt_driver.o ${OBJECTDIR}/_ext/1472/clksys_driver.o ${OBJECTDIR}/_ext/1472/TC_driver.o ${OBJECTDIR}/_ext/1472/iq.o ${OBJECTDIR}/_ext/1472/TaskAcq.o ${OBJECTDIR}/_ext/1472/usart_driver.o ${OBJECTDIR}/_ext/1472/adc_driver.o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ${OBJECTDIR}/_ext/1472/TaskUSB.o ${OBJECTDIR}/_ext/1472/spi_driver.o ${OBJECTDIR}/_ext/1472/lowpower.o ${OBJECTDIR}/_ext/1472/rtc_driver.o ${OBJECTDIR}/_ext/1472/sleepmgr.o ${OBJECTDIR}/_ext/1472/ccpwrite.o ${OBJECTDIR}/_ext/1472/seg7.o ${OBJECTDIR}/_ext/1472/buffers.o ${OBJECTDIR}/_ext/1472/ulaw.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/TaskBT.o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ${OBJECTDIR}/_ext/1472/crc.o ${OBJECTDIR}/_ext/1472/TaskCC.o ${OBJECTDIR}/_ext/1472/twi_driver.o ${OBJECTDIR}/_ext/1472/ltc2959.o ${OBJECTDIR}/_ext/900742808/twim.o ${OBJECTDIR}/_ext/900742808/twis.o ${OBJECTDIR}/_ext/900748601/nvm.o ${OBJECTDIR}/_ext/1210819865/sysclk.o ${OBJECTDIR}/_ext/1472/update_handler.o

# Source Files
SOURCEFILES=../BGAPI/bgapi_commands.c ../BGAPI/bgapi_parser.c ../BGAPI/BT122_handler.c ../BGAPI/legacybt_handler.c ../apple.c ../ffft.S ../main.c ../serial.c ../sp_driver.s ../rtos.c ../wdt_driver.c ../clksys_driver.c ../TC_driver.c ../iq.c ../TaskAcq.c ../usart_driver.c ../adc_driver.c ../eeprom_driver.c ../TaskUSB.c ../spi_driver.c ../lowpower.c ../rtc_driver.c ../sleepmgr.c ../ccpwrite.c ../seg7.c ../buffers.c ../ulaw.c ../eeprom.c ../TaskBT.c ../TaskWatchDog.c ../crc.c ../TaskCC.c ../twi_driver.c ../ltc2959.c ../src/ASF/xmega/drivers/twi/twim.c ../src/ASF/xmega/drivers/twi/twis.c ../src/ASF/xmega/drivers/nvm/nvm.c ../src/ASF/common/services/clock/xmega/sysclk.c ../update_handler.c

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
${OBJECTDIR}/_ext/2144063110/bgapi_commands.o: ../BGAPI/bgapi_commands.c  .generated_files/flags/Bluetooth/85ca626a1628088a00fbad0a17a5b9a2fa583b5b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ../BGAPI/bgapi_commands.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/bgapi_parser.o: ../BGAPI/bgapi_parser.c  .generated_files/flags/Bluetooth/19ea9860e3ad8fafd1020ff98e38e43c8983878b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ../BGAPI/bgapi_parser.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/BT122_handler.o: ../BGAPI/BT122_handler.c  .generated_files/flags/Bluetooth/6b939dc3fdf91374bac28327ace1f670bc07d280 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/BT122_handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/BT122_handler.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/BT122_handler.o.d" -MT "${OBJECTDIR}/_ext/2144063110/BT122_handler.o.d" -MT ${OBJECTDIR}/_ext/2144063110/BT122_handler.o  -o ${OBJECTDIR}/_ext/2144063110/BT122_handler.o ../BGAPI/BT122_handler.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/legacybt_handler.o: ../BGAPI/legacybt_handler.c  .generated_files/flags/Bluetooth/64e74792d9ddbdf0f39f7de1cb35656f73c7b96 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/legacybt_handler.o.d" -MT "${OBJECTDIR}/_ext/2144063110/legacybt_handler.o.d" -MT ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o  -o ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o ../BGAPI/legacybt_handler.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/apple.o: ../apple.c  .generated_files/flags/Bluetooth/6a3780d2211985b3887c71609fdbf0aa44c83424 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/apple.o.d" -MT "${OBJECTDIR}/_ext/1472/apple.o.d" -MT ${OBJECTDIR}/_ext/1472/apple.o  -o ${OBJECTDIR}/_ext/1472/apple.o ../apple.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Bluetooth/82a554941b3a1af846c84ff62cd5babb52ffe7fa .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  .generated_files/flags/Bluetooth/21a079c275cdf53d3c9eb206a4c3d319e640bf48 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -MT "${OBJECTDIR}/_ext/1472/serial.o.d" -MT ${OBJECTDIR}/_ext/1472/serial.o  -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/rtos.o: ../rtos.c  .generated_files/flags/Bluetooth/18279dbeb27cae7a284f63fed373cc1fa565f610 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT ${OBJECTDIR}/_ext/1472/rtos.o  -o ${OBJECTDIR}/_ext/1472/rtos.o ../rtos.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/wdt_driver.o: ../wdt_driver.c  .generated_files/flags/Bluetooth/38ea8d3c68aaee8147d49948ece007d75d14d6f9 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt_driver.o  -o ${OBJECTDIR}/_ext/1472/wdt_driver.o ../wdt_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/clksys_driver.o: ../clksys_driver.c  .generated_files/flags/Bluetooth/c70bc1ee22228344e61c2419f773e04da7f4f1b7 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/clksys_driver.o  -o ${OBJECTDIR}/_ext/1472/clksys_driver.o ../clksys_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TC_driver.o: ../TC_driver.c  .generated_files/flags/Bluetooth/c034bd1fe05c21dd3959d67ee9d9c92097ee8a00 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/TC_driver.o  -o ${OBJECTDIR}/_ext/1472/TC_driver.o ../TC_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/iq.o: ../iq.c  .generated_files/flags/Bluetooth/8c1897665d09a1c5a53c53cb5ad632a059dac32a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/iq.o.d" -MT "${OBJECTDIR}/_ext/1472/iq.o.d" -MT ${OBJECTDIR}/_ext/1472/iq.o  -o ${OBJECTDIR}/_ext/1472/iq.o ../iq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskAcq.o: ../TaskAcq.c  .generated_files/flags/Bluetooth/2276d8e173b0a7d95d959d06f25ccd07a1dbba95 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskAcq.o  -o ${OBJECTDIR}/_ext/1472/TaskAcq.o ../TaskAcq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/usart_driver.o: ../usart_driver.c  .generated_files/flags/Bluetooth/e2d85f6583ce449222d6f9adda2b03028e70c8e5 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/usart_driver.o  -o ${OBJECTDIR}/_ext/1472/usart_driver.o ../usart_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/adc_driver.o: ../adc_driver.c  .generated_files/flags/Bluetooth/fc9d6aba045ed9a3763c5e3d2b43a8ddd8072cf1 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/adc_driver.o  -o ${OBJECTDIR}/_ext/1472/adc_driver.o ../adc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom_driver.o: ../eeprom_driver.c  .generated_files/flags/Bluetooth/f259d5bfa58e9d02e21b68f0a2ab90fea5fc6688 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom_driver.o  -o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ../eeprom_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskUSB.o: ../TaskUSB.c  .generated_files/flags/Bluetooth/a6206e385c88b28148f3ccc46f98dfb320c58cd .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskUSB.o  -o ${OBJECTDIR}/_ext/1472/TaskUSB.o ../TaskUSB.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/spi_driver.o: ../spi_driver.c  .generated_files/flags/Bluetooth/214b860b36f3d11565a850629ce56142cf282a37 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/spi_driver.o  -o ${OBJECTDIR}/_ext/1472/spi_driver.o ../spi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/lowpower.o: ../lowpower.c  .generated_files/flags/Bluetooth/9efbcabd5226562fb402c846da919f0d0d4e6900 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT ${OBJECTDIR}/_ext/1472/lowpower.o  -o ${OBJECTDIR}/_ext/1472/lowpower.o ../lowpower.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/rtc_driver.o: ../rtc_driver.c  .generated_files/flags/Bluetooth/1aa2256411e8084f17284fd10729d717147edfb9 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/rtc_driver.o  -o ${OBJECTDIR}/_ext/1472/rtc_driver.o ../rtc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/sleepmgr.o: ../sleepmgr.c  .generated_files/flags/Bluetooth/15a8729753ec511c4dbee572591c9429bda52931 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT ${OBJECTDIR}/_ext/1472/sleepmgr.o  -o ${OBJECTDIR}/_ext/1472/sleepmgr.o ../sleepmgr.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ccpwrite.o: ../ccpwrite.c  .generated_files/flags/Bluetooth/e82837ee01cf0d367c7290ffe3fbd5044c3793fc .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT ${OBJECTDIR}/_ext/1472/ccpwrite.o  -o ${OBJECTDIR}/_ext/1472/ccpwrite.o ../ccpwrite.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/seg7.o: ../seg7.c  .generated_files/flags/Bluetooth/b19727cf49afe577facf4b7529c0a48cd6e69b4f .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT ${OBJECTDIR}/_ext/1472/seg7.o  -o ${OBJECTDIR}/_ext/1472/seg7.o ../seg7.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/buffers.o: ../buffers.c  .generated_files/flags/Bluetooth/c9c0cbaeeca56d302d6e49eeab47a9e42a295181 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT ${OBJECTDIR}/_ext/1472/buffers.o  -o ${OBJECTDIR}/_ext/1472/buffers.o ../buffers.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ulaw.o: ../ulaw.c  .generated_files/flags/Bluetooth/a9d7bd0a5b395c51572fea5341577b4203e11a44 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT ${OBJECTDIR}/_ext/1472/ulaw.o  -o ${OBJECTDIR}/_ext/1472/ulaw.o ../ulaw.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Bluetooth/d408cb0f407ba38dbe1c3b4851bfc34600073260 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskBT.o: ../TaskBT.c  .generated_files/flags/Bluetooth/9129a304ac60bc9800856cba39ebf2f33e1703d3 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskBT.o  -o ${OBJECTDIR}/_ext/1472/TaskBT.o ../TaskBT.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskWatchDog.o: ../TaskWatchDog.c  .generated_files/flags/Bluetooth/5d82dba37d5e7e11322a832260ef937a454e1ae7 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskWatchDog.o  -o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ../TaskWatchDog.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/crc.o: ../crc.c  .generated_files/flags/Bluetooth/8c3ae6398f2361cfb93c92b54edb9056aa7e5f47 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/crc.o.d" -MT "${OBJECTDIR}/_ext/1472/crc.o.d" -MT ${OBJECTDIR}/_ext/1472/crc.o  -o ${OBJECTDIR}/_ext/1472/crc.o ../crc.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskCC.o: ../TaskCC.c  .generated_files/flags/Bluetooth/340737e9a4b72cff2eef35a99fc934a2807e1541 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskCC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskCC.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskCC.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskCC.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskCC.o  -o ${OBJECTDIR}/_ext/1472/TaskCC.o ../TaskCC.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/twi_driver.o: ../twi_driver.c  .generated_files/flags/Bluetooth/5290bb75e10d84530a621a8879456703ba242051 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/twi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/twi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/twi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/twi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/twi_driver.o  -o ${OBJECTDIR}/_ext/1472/twi_driver.o ../twi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/ltc2959.o: ../ltc2959.c  .generated_files/flags/Bluetooth/fdafd8eac2a273ac5ffd078c9583d3e9349aea4d .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ltc2959.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ltc2959.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ltc2959.o.d" -MT "${OBJECTDIR}/_ext/1472/ltc2959.o.d" -MT ${OBJECTDIR}/_ext/1472/ltc2959.o  -o ${OBJECTDIR}/_ext/1472/ltc2959.o ../ltc2959.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/900742808/twim.o: ../src/ASF/xmega/drivers/twi/twim.c  .generated_files/flags/Bluetooth/5aa28b6cc2553bed4eec696071713948003fbd40 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/900742808" 
	@${RM} ${OBJECTDIR}/_ext/900742808/twim.o.d 
	@${RM} ${OBJECTDIR}/_ext/900742808/twim.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/900742808/twim.o.d" -MT "${OBJECTDIR}/_ext/900742808/twim.o.d" -MT ${OBJECTDIR}/_ext/900742808/twim.o  -o ${OBJECTDIR}/_ext/900742808/twim.o ../src/ASF/xmega/drivers/twi/twim.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/900742808/twis.o: ../src/ASF/xmega/drivers/twi/twis.c  .generated_files/flags/Bluetooth/cb0a21a593da3045c924ae908a2bbdec8b7505b3 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/900742808" 
	@${RM} ${OBJECTDIR}/_ext/900742808/twis.o.d 
	@${RM} ${OBJECTDIR}/_ext/900742808/twis.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/900742808/twis.o.d" -MT "${OBJECTDIR}/_ext/900742808/twis.o.d" -MT ${OBJECTDIR}/_ext/900742808/twis.o  -o ${OBJECTDIR}/_ext/900742808/twis.o ../src/ASF/xmega/drivers/twi/twis.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/900748601/nvm.o: ../src/ASF/xmega/drivers/nvm/nvm.c  .generated_files/flags/Bluetooth/4b19c8410bfa6b4c6d0335a14fd4e163372f91b4 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/900748601" 
	@${RM} ${OBJECTDIR}/_ext/900748601/nvm.o.d 
	@${RM} ${OBJECTDIR}/_ext/900748601/nvm.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/900748601/nvm.o.d" -MT "${OBJECTDIR}/_ext/900748601/nvm.o.d" -MT ${OBJECTDIR}/_ext/900748601/nvm.o  -o ${OBJECTDIR}/_ext/900748601/nvm.o ../src/ASF/xmega/drivers/nvm/nvm.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1210819865/sysclk.o: ../src/ASF/common/services/clock/xmega/sysclk.c  .generated_files/flags/Bluetooth/c9e57583e4ca3768c93de7661ef90e7d598219cb .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1210819865" 
	@${RM} ${OBJECTDIR}/_ext/1210819865/sysclk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1210819865/sysclk.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1210819865/sysclk.o.d" -MT "${OBJECTDIR}/_ext/1210819865/sysclk.o.d" -MT ${OBJECTDIR}/_ext/1210819865/sysclk.o  -o ${OBJECTDIR}/_ext/1210819865/sysclk.o ../src/ASF/common/services/clock/xmega/sysclk.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/update_handler.o: ../update_handler.c  .generated_files/flags/Bluetooth/fa281d81ca4519acca46603ed5526fcbe41723b0 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/update_handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/update_handler.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u   -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/update_handler.o.d" -MT "${OBJECTDIR}/_ext/1472/update_handler.o.d" -MT ${OBJECTDIR}/_ext/1472/update_handler.o  -o ${OBJECTDIR}/_ext/1472/update_handler.o ../update_handler.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
else
${OBJECTDIR}/_ext/2144063110/bgapi_commands.o: ../BGAPI/bgapi_commands.c  .generated_files/flags/Bluetooth/4627cfa2525c6a887e19e872b33ced1e4ef8c09c .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_commands.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_commands.o ../BGAPI/bgapi_commands.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/bgapi_parser.o: ../BGAPI/bgapi_parser.c  .generated_files/flags/Bluetooth/5040e6e6b3db5951b4a79305507daedcca4b39f6 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT "${OBJECTDIR}/_ext/2144063110/bgapi_parser.o.d" -MT ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o  -o ${OBJECTDIR}/_ext/2144063110/bgapi_parser.o ../BGAPI/bgapi_parser.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/BT122_handler.o: ../BGAPI/BT122_handler.c  .generated_files/flags/Bluetooth/ddd0c41da668fcd034c2a9e1cfbbe155b838f524 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/BT122_handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/BT122_handler.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/BT122_handler.o.d" -MT "${OBJECTDIR}/_ext/2144063110/BT122_handler.o.d" -MT ${OBJECTDIR}/_ext/2144063110/BT122_handler.o  -o ${OBJECTDIR}/_ext/2144063110/BT122_handler.o ../BGAPI/BT122_handler.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/2144063110/legacybt_handler.o: ../BGAPI/legacybt_handler.c  .generated_files/flags/Bluetooth/1ea77aeb3da2760643478b9f4858c39eeb777c50 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2144063110" 
	@${RM} ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/2144063110/legacybt_handler.o.d" -MT "${OBJECTDIR}/_ext/2144063110/legacybt_handler.o.d" -MT ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o  -o ${OBJECTDIR}/_ext/2144063110/legacybt_handler.o ../BGAPI/legacybt_handler.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/apple.o: ../apple.c  .generated_files/flags/Bluetooth/de6d9051ea91f06cc70af28bbb29c254ec49c213 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/apple.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/apple.o.d" -MT "${OBJECTDIR}/_ext/1472/apple.o.d" -MT ${OBJECTDIR}/_ext/1472/apple.o  -o ${OBJECTDIR}/_ext/1472/apple.o ../apple.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/Bluetooth/1cf0aa9c608c0ab45d082fe7543b09d6bb4522ec .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  .generated_files/flags/Bluetooth/33ef26e41f3ef5369c0d8addd6d426e784a952fe .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -MT "${OBJECTDIR}/_ext/1472/serial.o.d" -MT ${OBJECTDIR}/_ext/1472/serial.o  -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/rtos.o: ../rtos.c  .generated_files/flags/Bluetooth/5b640e97afc5c0e34f14274d008298733d709a98 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtos.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT "${OBJECTDIR}/_ext/1472/rtos.o.d" -MT ${OBJECTDIR}/_ext/1472/rtos.o  -o ${OBJECTDIR}/_ext/1472/rtos.o ../rtos.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/wdt_driver.o: ../wdt_driver.c  .generated_files/flags/Bluetooth/ed999cf6a18300f0ec2045cff0f4449b89f8176e .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wdt_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/wdt_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/wdt_driver.o  -o ${OBJECTDIR}/_ext/1472/wdt_driver.o ../wdt_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/clksys_driver.o: ../clksys_driver.c  .generated_files/flags/Bluetooth/6ee95c52ed89abf98f00d18ead9552998c48b356 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/clksys_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/clksys_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/clksys_driver.o  -o ${OBJECTDIR}/_ext/1472/clksys_driver.o ../clksys_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TC_driver.o: ../TC_driver.c  .generated_files/flags/Bluetooth/7f1810eb7772372bd3a13af18a1141683e3b1ee7 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TC_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/TC_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/TC_driver.o  -o ${OBJECTDIR}/_ext/1472/TC_driver.o ../TC_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/iq.o: ../iq.c  .generated_files/flags/Bluetooth/beb8faacbd20bc7c4e91124493a1e7c52b6f9d97 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/iq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/iq.o.d" -MT "${OBJECTDIR}/_ext/1472/iq.o.d" -MT ${OBJECTDIR}/_ext/1472/iq.o  -o ${OBJECTDIR}/_ext/1472/iq.o ../iq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskAcq.o: ../TaskAcq.c  .generated_files/flags/Bluetooth/1c3b53aba60b496e821b75bcab672c15101fa179 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskAcq.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskAcq.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskAcq.o  -o ${OBJECTDIR}/_ext/1472/TaskAcq.o ../TaskAcq.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/usart_driver.o: ../usart_driver.c  .generated_files/flags/Bluetooth/458eb3651f40bf56e0db263a0f9b4bd702d50f1a .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usart_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/usart_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/usart_driver.o  -o ${OBJECTDIR}/_ext/1472/usart_driver.o ../usart_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/adc_driver.o: ../adc_driver.c  .generated_files/flags/Bluetooth/a2705c5512ec7abdd7e7c213869bdfb65bfe53f5 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/adc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/adc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/adc_driver.o  -o ${OBJECTDIR}/_ext/1472/adc_driver.o ../adc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom_driver.o: ../eeprom_driver.c  .generated_files/flags/Bluetooth/78b83ef6836138a1db95d2baf8069d79b98a6c2b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom_driver.o  -o ${OBJECTDIR}/_ext/1472/eeprom_driver.o ../eeprom_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskUSB.o: ../TaskUSB.c  .generated_files/flags/Bluetooth/31b434fef85ee27aae16fb530e4bb39dd4a0cc71 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskUSB.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskUSB.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskUSB.o  -o ${OBJECTDIR}/_ext/1472/TaskUSB.o ../TaskUSB.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/spi_driver.o: ../spi_driver.c  .generated_files/flags/Bluetooth/a38dc598909dc4a87494b87d529a102d5767fa25 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/spi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/spi_driver.o  -o ${OBJECTDIR}/_ext/1472/spi_driver.o ../spi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/lowpower.o: ../lowpower.c  .generated_files/flags/Bluetooth/d8164cc5e04ecbcca15204467c55989399f9044b .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lowpower.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT "${OBJECTDIR}/_ext/1472/lowpower.o.d" -MT ${OBJECTDIR}/_ext/1472/lowpower.o  -o ${OBJECTDIR}/_ext/1472/lowpower.o ../lowpower.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/rtc_driver.o: ../rtc_driver.c  .generated_files/flags/Bluetooth/dfb6f4c22708111805af8c676d797f9d58a89a50 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtc_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/rtc_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/rtc_driver.o  -o ${OBJECTDIR}/_ext/1472/rtc_driver.o ../rtc_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/sleepmgr.o: ../sleepmgr.c  .generated_files/flags/Bluetooth/355627c3e7a7ae04bf8350df59cc4d5c8c9e3f4 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/sleepmgr.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT "${OBJECTDIR}/_ext/1472/sleepmgr.o.d" -MT ${OBJECTDIR}/_ext/1472/sleepmgr.o  -o ${OBJECTDIR}/_ext/1472/sleepmgr.o ../sleepmgr.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ccpwrite.o: ../ccpwrite.c  .generated_files/flags/Bluetooth/eaaabcb851c7d0b7da2aa543ac16106a29a51390 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ccpwrite.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT "${OBJECTDIR}/_ext/1472/ccpwrite.o.d" -MT ${OBJECTDIR}/_ext/1472/ccpwrite.o  -o ${OBJECTDIR}/_ext/1472/ccpwrite.o ../ccpwrite.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/seg7.o: ../seg7.c  .generated_files/flags/Bluetooth/ffd04a2105870cd07a50c7fa4cf039d226a1d567 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/seg7.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT "${OBJECTDIR}/_ext/1472/seg7.o.d" -MT ${OBJECTDIR}/_ext/1472/seg7.o  -o ${OBJECTDIR}/_ext/1472/seg7.o ../seg7.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/buffers.o: ../buffers.c  .generated_files/flags/Bluetooth/5e8684440532c9b90c0ffb538e41902019c4164f .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/buffers.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT "${OBJECTDIR}/_ext/1472/buffers.o.d" -MT ${OBJECTDIR}/_ext/1472/buffers.o  -o ${OBJECTDIR}/_ext/1472/buffers.o ../buffers.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/ulaw.o: ../ulaw.c  .generated_files/flags/Bluetooth/267128836ee9377b159c92c4bc7f25816bb516ec .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ulaw.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT "${OBJECTDIR}/_ext/1472/ulaw.o.d" -MT ${OBJECTDIR}/_ext/1472/ulaw.o  -o ${OBJECTDIR}/_ext/1472/ulaw.o ../ulaw.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/flags/Bluetooth/c05445946a749259cad42751194898ef938443 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT "${OBJECTDIR}/_ext/1472/eeprom.o.d" -MT ${OBJECTDIR}/_ext/1472/eeprom.o  -o ${OBJECTDIR}/_ext/1472/eeprom.o ../eeprom.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskBT.o: ../TaskBT.c  .generated_files/flags/Bluetooth/b490c1c1aba296e9d94816a9cc12398d11d742f5 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskBT.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskBT.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskBT.o  -o ${OBJECTDIR}/_ext/1472/TaskBT.o ../TaskBT.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskWatchDog.o: ../TaskWatchDog.c  .generated_files/flags/Bluetooth/7193ec8c799a97395a3bf4d3cc67ef72b54d7f80 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskWatchDog.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskWatchDog.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskWatchDog.o  -o ${OBJECTDIR}/_ext/1472/TaskWatchDog.o ../TaskWatchDog.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/crc.o: ../crc.c  .generated_files/flags/Bluetooth/6c6f091c941371d71b1439b27529f63a7e1559a7 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/crc.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/crc.o.d" -MT "${OBJECTDIR}/_ext/1472/crc.o.d" -MT ${OBJECTDIR}/_ext/1472/crc.o  -o ${OBJECTDIR}/_ext/1472/crc.o ../crc.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TaskCC.o: ../TaskCC.c  .generated_files/flags/Bluetooth/ec9d9ce8e34e6dfc54c5af7b4c08e9a68cd81cd8 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskCC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TaskCC.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/TaskCC.o.d" -MT "${OBJECTDIR}/_ext/1472/TaskCC.o.d" -MT ${OBJECTDIR}/_ext/1472/TaskCC.o  -o ${OBJECTDIR}/_ext/1472/TaskCC.o ../TaskCC.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/twi_driver.o: ../twi_driver.c  .generated_files/flags/Bluetooth/5d0078f1c6ccaa4baf2b7f1ab0acbcadf7442ca3 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/twi_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/twi_driver.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/twi_driver.o.d" -MT "${OBJECTDIR}/_ext/1472/twi_driver.o.d" -MT ${OBJECTDIR}/_ext/1472/twi_driver.o  -o ${OBJECTDIR}/_ext/1472/twi_driver.o ../twi_driver.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/ltc2959.o: ../ltc2959.c  .generated_files/flags/Bluetooth/ec80bdd03fd01475ed7552cc04ed74a9c6db4f02 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ltc2959.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ltc2959.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/ltc2959.o.d" -MT "${OBJECTDIR}/_ext/1472/ltc2959.o.d" -MT ${OBJECTDIR}/_ext/1472/ltc2959.o  -o ${OBJECTDIR}/_ext/1472/ltc2959.o ../ltc2959.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/900742808/twim.o: ../src/ASF/xmega/drivers/twi/twim.c  .generated_files/flags/Bluetooth/5630df3719f068d77fa687c00d2cb196053ca206 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/900742808" 
	@${RM} ${OBJECTDIR}/_ext/900742808/twim.o.d 
	@${RM} ${OBJECTDIR}/_ext/900742808/twim.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/900742808/twim.o.d" -MT "${OBJECTDIR}/_ext/900742808/twim.o.d" -MT ${OBJECTDIR}/_ext/900742808/twim.o  -o ${OBJECTDIR}/_ext/900742808/twim.o ../src/ASF/xmega/drivers/twi/twim.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/900742808/twis.o: ../src/ASF/xmega/drivers/twi/twis.c  .generated_files/flags/Bluetooth/1777127b3fddae669d7e1678fe5a14673ba0ba40 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/900742808" 
	@${RM} ${OBJECTDIR}/_ext/900742808/twis.o.d 
	@${RM} ${OBJECTDIR}/_ext/900742808/twis.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/900742808/twis.o.d" -MT "${OBJECTDIR}/_ext/900742808/twis.o.d" -MT ${OBJECTDIR}/_ext/900742808/twis.o  -o ${OBJECTDIR}/_ext/900742808/twis.o ../src/ASF/xmega/drivers/twi/twis.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/900748601/nvm.o: ../src/ASF/xmega/drivers/nvm/nvm.c  .generated_files/flags/Bluetooth/bbe3a4cb5abf9d1a86188ffdb74cb47197d0464c .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/900748601" 
	@${RM} ${OBJECTDIR}/_ext/900748601/nvm.o.d 
	@${RM} ${OBJECTDIR}/_ext/900748601/nvm.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/900748601/nvm.o.d" -MT "${OBJECTDIR}/_ext/900748601/nvm.o.d" -MT ${OBJECTDIR}/_ext/900748601/nvm.o  -o ${OBJECTDIR}/_ext/900748601/nvm.o ../src/ASF/xmega/drivers/nvm/nvm.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1210819865/sysclk.o: ../src/ASF/common/services/clock/xmega/sysclk.c  .generated_files/flags/Bluetooth/28e83bd8c5d14b5081cf0335e86bb0d89a657d28 .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1210819865" 
	@${RM} ${OBJECTDIR}/_ext/1210819865/sysclk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1210819865/sysclk.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1210819865/sysclk.o.d" -MT "${OBJECTDIR}/_ext/1210819865/sysclk.o.d" -MT ${OBJECTDIR}/_ext/1210819865/sysclk.o  -o ${OBJECTDIR}/_ext/1210819865/sysclk.o ../src/ASF/common/services/clock/xmega/sysclk.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
${OBJECTDIR}/_ext/1472/update_handler.o: ../update_handler.c  .generated_files/flags/Bluetooth/c6c16e1b9337896a9f0a2cc687f7f7a0ac89905f .generated_files/flags/Bluetooth/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/update_handler.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/update_handler.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atxmega64a3u    -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DBLUETOOTH=1 -DBOARD=USER_BOARD -DF_CPU=32000000UL -D__AVR_ATxmega64A3U__ -D__AVR_XMEGA__ -DWDE=3 -DSLEEP_MODE_IDLE=0x00 -DSLEEP_MODE_STANDBY=0x06 -DSLEEP_MODE_PWR_DOWN=0x04 -DSLEEP_MODE_EXT_STANDBY=0x07 -DSLEEP_MODE_SAVE=0x05  -I "../src/ASF/common/utils" -I "../src/ASF/xmega/utils" -I "../src/ASF/xmega/utils/preprocessor" -I "../src/ASF/common/utils/interrupt" -I "../src/config" -I "../src" -I "../src/ASF/common/services/clock" -I "../src/ASF/common/boards" -I "../src/ASF/xmega/drivers/cpu" -I "../src/ASF/xmega/drivers/twi" -I "../src/ASF/xmega/drivers/nvm" -I "../src/ASF/common/boards/user_board" -I "../src/Config" -I "../src/ASF/common/services/twi" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/update_handler.o.d" -MT "${OBJECTDIR}/_ext/1472/update_handler.o.d" -MT ${OBJECTDIR}/_ext/1472/update_handler.o  -o ${OBJECTDIR}/_ext/1472/update_handler.o ../update_handler.c  -DXPRJ_Bluetooth=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-2 -std=gnu99
	
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
