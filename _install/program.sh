#!/bin/bash
# Script to program IQ device with Atmel programmer
# Programs both bootloader and application firmware

# Configuration
TOOL="atmelice"
INTERFACE="pdi"
DEVICE="atxmega64a3u"

# Paths (Windows paths for Git Bash)
ATPROGRAM="/c/Program Files (x86)/Atmel/Studio/7.0/atbackend/atprogram.exe"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Hex files
BOOTLOADER_HEX="$SCRIPT_DIR/IQ/Bootloader/iq_bootloader.hex"
APPLICATION_HEX="$SCRIPT_DIR/IQ/Bluetooth/iq_bluetooth.hex"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}============================================${NC}"
echo -e "${CYAN}IQ Device Programmer${NC}"
echo -e "${CYAN}============================================${NC}"

# Check atprogram exists
if [ ! -f "$ATPROGRAM" ]; then
    echo -e "${RED}ERROR: atprogram.exe not found at: $ATPROGRAM${NC}"
    exit 1
fi

# Check hex files
if [ ! -f "$BOOTLOADER_HEX" ]; then
    echo -e "${RED}ERROR: Bootloader hex not found: $BOOTLOADER_HEX${NC}"
    exit 1
fi

if [ ! -f "$APPLICATION_HEX" ]; then
    echo -e "${RED}ERROR: Application hex not found: $APPLICATION_HEX${NC}"
    exit 1
fi

echo "Tool:        $TOOL"
echo "Interface:   $INTERFACE"
echo "Device:      $DEVICE"
echo "Bootloader:  $BOOTLOADER_HEX"
echo "Application: $APPLICATION_HEX"
echo -e "${CYAN}============================================${NC}"

# Erase chip
if [ "$1" == "--erase" ] || [ "$1" == "-e" ]; then
    echo -e "\n${CYAN}Erasing chip...${NC}"
    "$ATPROGRAM" -t $TOOL -i $INTERFACE -d $DEVICE chiperase
    if [ $? -ne 0 ]; then
        echo -e "${RED}ERROR: Chip erase failed${NC}"
        exit 1
    fi
    echo -e "${GREEN}Chip erased successfully${NC}"
fi

# Program bootloader
echo -e "\n${CYAN}Programming bootloader...${NC}"
"$ATPROGRAM" -t $TOOL -i $INTERFACE -d $DEVICE program -fl -f "$BOOTLOADER_HEX" --verify
if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: Bootloader programming failed${NC}"
    exit 1
fi
echo -e "${GREEN}Bootloader programmed successfully${NC}"

# Program application
echo -e "\n${CYAN}Programming application...${NC}"
"$ATPROGRAM" -t $TOOL -i $INTERFACE -d $DEVICE program -fl -f "$APPLICATION_HEX" --verify
if [ $? -ne 0 ]; then
    echo -e "${RED}ERROR: Application programming failed${NC}"
    exit 1
fi
echo -e "${GREEN}Application programmed successfully${NC}"

echo -e "\n${CYAN}============================================${NC}"
echo -e "${GREEN}Programming complete!${NC}"
echo -e "${CYAN}============================================${NC}"
