#!/bin/bash

# Build and Run Script for BT122 Monitor

set -e  # Exit on error

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

echo "========================================="
echo "Building BT122 Monitor..."
echo "========================================="

# Clean and build
make clean
make

echo ""
echo "========================================="
echo "Build successful!"
echo "========================================="
echo ""

# Check if serial port exists
if [ ! -e /dev/ttyUSB0 ]; then
    echo "WARNING: /dev/ttyUSB0 not found!"
    echo "Available serial ports:"
    ls -l /dev/ttyUSB* 2>/dev/null || echo "  No /dev/ttyUSB* devices found"
    ls -l /dev/ttyACM* 2>/dev/null || echo "  No /dev/ttyACM* devices found"
    echo ""
    echo "Please specify the correct port as an argument:"
    echo "  $0 /dev/ttyUSB1"
    echo ""
    read -p "Press Enter to exit or Ctrl+C to abort..."
    exit 1
fi

# Check permissions
if [ ! -r /dev/ttyUSB0 ] || [ ! -w /dev/ttyUSB0 ]; then
    echo "WARNING: No read/write permission for /dev/ttyUSB0"
    echo ""
    echo "You have two options:"
    echo "  1. Add your user to dialout group (recommended):"
    echo "     sudo usermod -a -G dialout \$USER"
    echo "     Then log out and log back in"
    echo ""
    echo "  2. Run with sudo:"
    echo "     sudo $0"
    echo ""
    read -p "Do you want to run with sudo now? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        sudo ./bt122_monitor "$@"
        exit 0
    else
        exit 1
    fi
fi

echo "Running BT122 Monitor..."
echo "Press Ctrl+C to stop"
echo ""

# Run the program
./bt122_monitor "$@"
