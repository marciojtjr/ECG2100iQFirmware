#!/bin/bash
# Copy hex files to _install directory

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INSTALL_DIR="${SCRIPT_DIR}/_install"

# Create _install directory if it doesn't exist
if [ ! -d "$INSTALL_DIR" ]; then
    mkdir -p "$INSTALL_DIR"
    echo "Created _install directory"
fi

# Copy hex files
echo "Copying hex files to _install..."
echo ""

# Copy bootloader hex
BOOTLOADER_HEX="${SCRIPT_DIR}/IQ/IQ.X/dist/Bootloader/production/IQ.X.production.hex"
if [ -f "$BOOTLOADER_HEX" ]; then
    if cp "$BOOTLOADER_HEX" "$INSTALL_DIR/iq_bootloader.hex"; then
        echo -e "${GREEN}✓${NC} Copied iq_bootloader.hex"
    else
        echo -e "${RED}✗${NC} ERROR: Failed to copy iq_bootloader.hex"
        exit 1
    fi
else
    echo -e "${RED}✗${NC} ERROR: IQ.X.production.hex not found at $BOOTLOADER_HEX"
    exit 1
fi

# Copy bluetooth hex
BLUETOOTH_HEX="${SCRIPT_DIR}/IQ/IQ.X/dist/Bluetooth/production/IQ.X.production.hex"
if [ -f "$BLUETOOTH_HEX" ]; then
    if cp "$BLUETOOTH_HEX" "$INSTALL_DIR/iq_bluetooth.hex"; then
        echo -e "${GREEN}✓${NC} Copied iq_bluetooth.hex"
    else
        echo -e "${RED}✗${NC} ERROR: Failed to copy iq_bluetooth.hex"
        exit 1
    fi
else
    echo -e "${RED}✗${NC} ERROR: IQ.X.production.hex not found at $BLUETOOTH_HEX"
    exit 1
fi

echo ""
echo -e "${CYAN}Done!${NC}"
echo ""
echo "Hex files are now in: $INSTALL_DIR"
