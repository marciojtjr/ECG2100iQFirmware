#!/bin/bash
# Serial Number Programming Script
# Waits for device connection and programs serial number

set -e

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Default configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BAUDRATE=115200
PORT=""
SERIAL_NUMBER=""

# Usage
usage() {
    echo "Usage: $0 [OPTIONS] [serial_number]"
    echo ""
    echo "Options:"
    echo "  -p, --port PORT       Serial port (e.g., /dev/ttyUSB0)"
    echo "  -b, --baud RATE       Baud rate (default: 115200)"
    echo "  -h, --help            Show this help"
    echo ""
    echo "Examples:"
    echo "  $0                    # Prompt for serial number, auto-detect port"
    echo "  $0 12345678           # Use provided serial number"
    echo "  $0 -p /dev/ttyUSB0 12345678"
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
        -h|--help)
            usage
            ;;
        -*)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            ;;
        *)
            SERIAL_NUMBER="$1"
            shift
            ;;
    esac
done

echo -e "${CYAN}============================================${NC}"
echo -e "${CYAN}IQ Serial Number Programming${NC}"
echo -e "${CYAN}============================================${NC}"
echo ""

# Get serial number if not provided
if [ -z "$SERIAL_NUMBER" ]; then
    echo -ne "${BOLD}Enter serial number:${NC} "
    read SERIAL_NUMBER
fi

# Validate serial number is numeric
if ! [[ "$SERIAL_NUMBER" =~ ^[0-9]+$ ]]; then
    echo -e "${RED}ERROR: Serial number must be numeric${NC}"
    exit 1
fi

echo "Serial number: $SERIAL_NUMBER"
echo ""

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
    stty -F "$port" "$baud" cs8 -cstopb -parenb raw -echo ixon ixoff || {
        echo -e "${RED}ERROR: Failed to configure serial port${NC}"
        return 1
    }

    # Set timeout
    stty -F "$port" min 0 time 1
}

# Wait for device connection
wait_for_device() {
    echo -e "${YELLOW}Waiting for device connection...${NC}"
    echo "Please connect the device or press Ctrl+C to cancel"
    echo ""

    local retry_count=0
    local max_retries=60  # 60 seconds timeout

    while [ $retry_count -lt $max_retries ]; do
        # Try to find serial port
        if [ -z "$PORT" ]; then
            local found_port=$(find_serial_port 2>/dev/null)
            if [ -n "$found_port" ]; then
                PORT="$found_port"
                echo -e "${GREEN}Device connected on: $PORT${NC}"
                return 0
            fi
        else
            # Check if specified port exists
            if [ -c "$PORT" ]; then
                echo -e "${GREEN}Device found on: $PORT${NC}"
                return 0
            fi
        fi

        # Show progress dots
        echo -n "."
        sleep 1
        retry_count=$((retry_count + 1))
    done

    echo ""
    echo -e "${RED}ERROR: Device connection timeout${NC}"
    return 1
}

# Wait for device if not already connected
if [ -z "$PORT" ]; then
    if ! wait_for_device; then
        exit 1
    fi
else
    echo "Using port: $PORT"
    # Check port exists
    if [ ! -c "$PORT" ]; then
        echo -e "${YELLOW}Port not found, waiting for device...${NC}"
        if ! wait_for_device; then
            exit 1
        fi
    fi
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
echo ""
echo "Configuring port: $PORT @ $BAUDRATE baud"
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
sleep 0.5
kill $cat_pid 2>/dev/null || true

# Send empty line to get device attention and wait for Hello response
echo ""
echo "Initializing communication..."
echo "" >&3
sleep 0.5

# Read and display the complete Hello response (including Battery status)
echo "Device response:"
hello_response=""
start_time=$(date +%s)
last_read_time=$start_time

while [ $(($(date +%s) - start_time)) -lt 3 ]; do
    if read -t 0.2 -u 3 line 2>/dev/null; then
        hello_response+="$line"
        echo "$line"
        last_read_time=$(date +%s)
    else
        # If no data for 0.5 seconds after last read, assume response is complete
        if [ $(($(date +%s) - last_read_time)) -ge 1 ]; then
            break
        fi
    fi
done

if [ -z "$hello_response" ]; then
    echo -e "${YELLOW}Warning: No response from device to initialization${NC}"
    echo "Device may not be in application mode. Try power cycling the device."
    exit 1
fi

# Small delay before next command
sleep 0.3

# Send serial number command with explicit CR
echo ""
echo "Sending serial number: sn${SERIAL_NUMBER}"
printf "sn%s\r\n" "$SERIAL_NUMBER" >&3

# Give device time to process
sleep 0.3

# Wait for response (character-by-character since device doesn't send newline)
echo "Waiting for device response..."
response=""
start_time=$(date +%s)
timeout=3
no_data_count=0

while [ $(($(date +%s) - start_time)) -lt $timeout ]; do
    # Read one character at a time
    if read -t 0.05 -u 3 -n 1 char 2>/dev/null; then
        response+="$char"
        echo -n "$char"
        no_data_count=0

        # Check if we got the complete SN response (ends with "101")
        if echo "$response" | grep -q "SN:9-.*-101"; then
            echo ""  # Add newline after response
            break
        fi
    else
        no_data_count=$((no_data_count + 1))
        # If no data for 10 consecutive reads (0.5s), consider response complete
        if [ $no_data_count -ge 10 ] && [ -n "$response" ]; then
            echo ""  # Add newline after response
            break
        fi
    fi
done

echo ""

# Check if we got a valid response
if echo "$response" | grep -q "SN:9-"; then
    echo -e "${CYAN}============================================${NC}"
    echo -e "${GREEN}Serial number programmed successfully!${NC}"
    echo -e "${CYAN}============================================${NC}"

    # Query version information to verify
    echo ""
    echo "Querying device information..."
    sleep 0.5

    # Clear input buffer
    while read -t 0.1 -u 3 line 2>/dev/null; do :; done

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

    echo ""
    exit 0
elif echo "$response" | grep -qi "SN:"; then
    echo -e "${YELLOW}Device responded with serial number${NC}"
    echo "Response: $response"
    exit 0
else
    echo -e "${RED}No valid response from device${NC}"
    echo "Response received: $response"
    exit 1
fi
