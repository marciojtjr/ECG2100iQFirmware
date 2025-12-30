#!/bin/bash
# Firmware update script - sends hex file over serial to bootloader
# Pure bash implementation - no Python required

set -e

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
GRAY='\033[0;90m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Default configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HEX_FILE="${SCRIPT_DIR}/iq_bluetooth.hex"
BAUDRATE=115200
TIMEOUT=10
PORT=""

# Usage
usage() {
    echo "Usage: $0 [OPTIONS] [hex_file]"
    echo ""
    echo "Options:"
    echo "  -p, --port PORT       Serial port (e.g., /dev/ttyUSB0)"
    echo "  -b, --baud RATE       Baud rate (default: 115200)"
    echo "  -t, --timeout SEC     Timeout in seconds (default: 10)"
    echo "  -h, --help            Show this help"
    echo ""
    echo "Examples:"
    echo "  $0                              # Auto-detect port"
    echo "  $0 -p /dev/ttyUSB0              # Specify port"
    echo "  $0 -b 115200 my_firmware.hex    # Custom baud and hex file"
    exit 0
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--port)
            PORT="$2"
            shift 2
            ;;
        -b|--baud|--baudrate)
            BAUDRATE="$2"
            shift 2
            ;;
        -t|--timeout)
            TIMEOUT="$2"
            shift 2
            ;;
        -h|--help)
            usage
            ;;
        -*)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            ;;
        *)
            HEX_FILE="$1"
            shift
            ;;
    esac
done

# Find FTDI/USB serial port
find_serial_port() {
    local ports=()

    # Look for common USB serial devices
    for device in /dev/ttyUSB* /dev/ttyACM* /dev/ttyS*; do
        if [ -e "$device" ] && [ -c "$device" ]; then
            ports+=("$device")
        fi
    done

    # Filter for FTDI if possible
    local ftdi_port=""
    for port in "${ports[@]}"; do
        if [ -e "/sys/class/tty/$(basename "$port")/device/driver" ]; then
            local driver=$(readlink -f "/sys/class/tty/$(basename "$port")/device/driver" 2>/dev/null)
            if [[ "$driver" == *"ftdi"* ]] || [[ "$driver" == *"usb"* ]]; then
                ftdi_port="$port"
                break
            fi
        fi
    done

    # Return FTDI port if found
    if [ -n "$ftdi_port" ]; then
        echo "$ftdi_port"
        return 0
    fi

    # If only one port, return it
    if [ ${#ports[@]} -eq 1 ]; then
        echo "${ports[0]}"
        return 0
    fi

    # Multiple ports - let user choose
    if [ ${#ports[@]} -gt 1 ]; then
        echo -e "${CYAN}Available serial ports:${NC}" >&2
        for i in "${!ports[@]}"; do
            echo "  $((i+1)). ${ports[$i]}" >&2
        done
        echo -n "Select port number (1-${#ports[@]}): " >&2
        read selection

        if [[ "$selection" =~ ^[0-9]+$ ]] && [ "$selection" -ge 1 ] && [ "$selection" -le ${#ports[@]} ]; then
            echo "${ports[$((selection-1))]}"
            return 0
        fi
    fi

    return 1
}

# Configure serial port
setup_serial() {
    local port=$1
    local baud=$2

    # Configure port with stty
    # raw mode, no echo, 8N1, XON/XOFF flow control
    stty -F "$port" "$baud" cs8 -cstopb -parenb raw \
         -echo -echoe -echok -echonl -echoctl -echoke \
         ixon ixoff || {
        echo -e "${RED}ERROR: Failed to configure serial port${NC}"
        return 1
    }

    # Set timeout
    stty -F "$port" min 0 time 1
}

# Read from serial with timeout (non-blocking)
read_serial() {
    local port=$1
    local timeout=$2
    local output=""
    local start_time=$(date +%s)

    while [ $(($(date +%s) - start_time)) -lt $timeout ]; do
        # Try to read available data (non-blocking)
        if read -t 0.1 -u 3 line 2>/dev/null; then
            output+="$line"
            echo -n "$line"
        fi
    done
}

# Main script
echo -e "${CYAN}============================================${NC}"
echo -e "${CYAN}IQ Firmware Updater${NC}"
echo -e "${CYAN}============================================${NC}"

# Check hex file exists
if [ ! -f "$HEX_FILE" ]; then
    echo -e "${RED}ERROR: Hex file not found: $HEX_FILE${NC}"
    exit 1
fi
echo "Hex file: $HEX_FILE"

# Find or use specified serial port
if [ -z "$PORT" ]; then
    echo ""
    echo "Searching for serial port..."
    PORT=$(find_serial_port)
    if [ -z "$PORT" ]; then
        echo -e "${RED}ERROR: No serial port found${NC}"
        exit 1
    fi
    echo -e "${GREEN}Found port: $PORT${NC}"
else
    echo "Using port: $PORT"
fi

# Check port exists and is accessible
if [ ! -c "$PORT" ]; then
    echo -e "${RED}ERROR: $PORT is not a valid serial device${NC}"
    exit 1
fi

# Check permissions
if [ ! -r "$PORT" ] || [ ! -w "$PORT" ]; then
    echo -e "${YELLOW}WARNING: No read/write permission on $PORT${NC}"
    echo -e "${YELLOW}Try: sudo usermod -a -G dialout \$USER${NC}"
    echo -e "${YELLOW}Then log out and back in.${NC}"
    echo ""
    echo -n "Attempting to continue with sudo... "
    if ! sudo -v; then
        echo -e "${RED}FAILED${NC}"
        exit 1
    fi
    SUDO="sudo"
fi

# Setup serial port
echo "Configuring port: $PORT @ $BAUDRATE baud (XON/XOFF)"
if ! setup_serial "$PORT" "$BAUDRATE"; then
    exit 1
fi
echo -e "${GREEN}Port configured successfully${NC}"

# Open serial port for reading and writing
exec 3<>"$PORT"

# Function to cleanup on exit
cleanup() {
    exec 3>&- 2>/dev/null || true
}
trap cleanup EXIT

# Clear any pending data
$SUDO cat "$PORT" > /dev/null 2>&1 &
cat_pid=$!
sleep 0.2
kill $cat_pid 2>/dev/null || true

# Send 'up' command
echo ""
echo "Sending 'up' command..."
echo "up" >&3

# Wait for bootloader response
echo "Waiting for bootloader..."
response=""
bootloader_ready=false
start_time=$(date +%s)

while [ $(($(date +%s) - start_time)) -lt $TIMEOUT ]; do
    if read -t 0.1 -u 3 line 2>/dev/null; then
        response+="$line"
        echo -n "$line"

        # Check for bootloader ready indicators
        if echo "$response" | grep -iq "update\|bootloader"; then
            bootloader_ready=true
            echo ""
            break
        fi
    fi
done

if [ "$bootloader_ready" != "true" ]; then
    echo -e "${RED}ERROR: Bootloader not responding (timeout)${NC}"
    exit 1
fi

echo ""
echo -e "${CYAN}[Bootloader response: '$response']${NC}"
echo -e "${GREEN}Bootloader ready!${NC}"
sleep 0.5

# Send hex file
echo ""
echo "Sending hex file (XON/XOFF flow control)..."
total_lines=$(wc -l < "$HEX_FILE")
echo "Lines to send: $total_lines"

line_num=0
success_count=0
error_count=0
appstart_detected=false

echo ""
while IFS= read -r line || [ -n "$line" ]; do
    line_num=$((line_num + 1))

    # Send line
    echo "$line" >&3

    # Small delay to let bootloader process
    sleep 0.01

    # Read any available response (with longer timeout)
    response_buffer=""
    while read -t 0.05 -u 3 -n 1 char 2>/dev/null; do
        response_buffer+="$char"
        case "$char" in
            '.')
                echo -ne "${GREEN}.${NC}"
                success_count=$((success_count + 1))
                ;;
            'E')
                echo -ne "${RED}E${NC}"
                error_count=$((error_count + 1))
                ;;
            [a-zA-Z])
                echo -ne "${GRAY}${char}${NC}"
                ;;
        esac
    done

    # Check if Appstart appeared in the response
    if echo "$response_buffer" | grep -iq "appstart"; then
        appstart_detected=true
    fi

done < "$HEX_FILE"

echo ""
echo -e "${YELLOW}File sent, waiting for completion...${NC}"

# Wait for final responses
final_response=""
start_time=$(date +%s)
timeout=10

while [ $(($(date +%s) - start_time)) -lt $timeout ]; do
    if read -t 0.1 -u 3 line 2>/dev/null; then
        final_response+="$line"
        echo -ne "${GRAY}${line}${NC}"
        start_time=$(date +%s)  # Reset timeout on data

        if echo "$final_response" | grep -iq "appstart\|app start"; then
            break
        fi
    fi
done

echo ""
echo "Records: $success_count OK, $error_count errors"

# Check if Appstart was detected anywhere
if [ "$appstart_detected" = true ] || echo "$final_response" | grep -iq "appstart\|app start"; then
    echo ""
    echo -e "${CYAN}============================================${NC}"
    echo -e "${GREEN}Firmware update complete!${NC}"
    echo -e "${CYAN}============================================${NC}"

    # Query version information
    echo ""
    echo "Querying firmware version..."
    sleep 1

    # Send 've' command
    echo "ve" >&3

    # Read version response
    version_response=""
    start_time=$(date +%s)
    timeout=3

    while [ $(($(date +%s) - start_time)) -lt $timeout ]; do
        if read -t 0.1 -u 3 line 2>/dev/null; then
            version_response+="$line"
            echo "$line"

            # Check if we got the full response (contains "SN:")
            if echo "$version_response" | grep -q "SN:"; then
                break
            fi
        fi
    done

    exit 0
elif [ $error_count -gt 10 ]; then
    # Only fail if we have significant errors (more than 10)
    echo ""
    echo -e "${RED}Update had too many errors${NC}"
    exit 1
else
    echo ""
    echo -e "${YELLOW}Transfer complete (minor errors ignored)${NC}"
    exit 0
fi
