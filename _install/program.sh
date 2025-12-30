#!/bin/bash
# Script to program IQ device with avrdude
# Programs both bootloader and application firmware

set -e  # Exit on error

# Configuration
PROGRAMMER="atmelice_pdi"
DEVICE="atxmega64a3u"

# Paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AVRDUDE="${SCRIPT_DIR}/avrdude-8.1/avrdude"

# Hex files
BOOTLOADER_HEX="${SCRIPT_DIR}/iq_bootloader.hex"
APPLICATION_HEX="${SCRIPT_DIR}/iq_bluetooth.hex"

echo "============================================"
echo "IQ Device Programmer (avrdude)"
echo "============================================"
echo "Programmer:  ${PROGRAMMER}"
echo "Device:      ${DEVICE}"
echo "Bootloader:  ${BOOTLOADER_HEX}"
echo "Application: ${APPLICATION_HEX}"
echo "============================================"

# Check avrdude exists
if [ ! -f "${AVRDUDE}" ]; then
    echo "ERROR: avrdude not found at ${AVRDUDE}"
    exit 1
fi

# Make sure avrdude is executable
chmod +x "${AVRDUDE}"

# Check hex files
if [ ! -f "${BOOTLOADER_HEX}" ]; then
    echo "ERROR: Bootloader hex not found: ${BOOTLOADER_HEX}"
    exit 1
fi

if [ ! -f "${APPLICATION_HEX}" ]; then
    echo "ERROR: Application hex not found: ${APPLICATION_HEX}"
    exit 1
fi

# Erase if requested
if [ "$1" = "--erase" ]; then
    echo ""
    echo "Erasing chip..."
    if ! "${AVRDUDE}" -c "${PROGRAMMER}" -p "${DEVICE}" -e; then
        echo "ERROR: Chip erase failed"
        exit 1
    fi
    echo "Chip erased successfully"
fi

# Program bootloader
echo ""
echo "Programming bootloader..."
if ! "${AVRDUDE}" -c "${PROGRAMMER}" -p "${DEVICE}" -U "flash:w:${BOOTLOADER_HEX}:i"; then
    echo "ERROR: Bootloader programming failed"
    exit 1
fi
echo "Bootloader programmed successfully"

# Program application
echo ""
echo "Programming application..."
if ! "${AVRDUDE}" -c "${PROGRAMMER}" -p "${DEVICE}" -U "flash:w:${APPLICATION_HEX}:i"; then
    echo "ERROR: Application programming failed"
    exit 1
fi
echo "Application programmed successfully"

echo ""
echo "============================================"
echo "Programming complete!"
echo "============================================"
