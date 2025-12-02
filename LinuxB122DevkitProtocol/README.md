# Linux BT122 DevKit Protocol Monitor

A simple C program to monitor serial communication from BT122 Bluetooth module on Linux.

## Features

- Connects to serial port (default: `/dev/ttyUSB0`)
- Configures port: 115200 baud, 8N1, hardware flow control (RTS/CTS)
- Displays received data in both HEX and ASCII format
- Graceful shutdown with Ctrl+C
- VSCode debugging support with GDB

## Quick Start - Build and Run Script

The easiest way to build and run:

```bash
./build_and_run.sh
```

This script will:
1. Clean and build the project
2. Check if the serial port exists
3. Check permissions and offer to run with sudo if needed
4. Run the program

### Run with custom port
```bash
./build_and_run.sh /dev/ttyUSB1
```

## Building Manually

### Release build
```bash
make
```

### Debug build (with GDB symbols)
```bash
make debug
```

### Clean build artifacts
```bash
make clean
```

## Usage

### Run with default port (/dev/ttyUSB0)
```bash
./bt122_monitor
```

### Run with custom port
```bash
./bt122_monitor /dev/ttyUSB1
```

## VSCode Debugging

Yes! You can debug this program in VSCode with full GDB support.

### Setup

1. **Install required VSCode extensions:**
   - C/C++ (by Microsoft) - `ms-vscode.cpptools`
   - C/C++ Extension Pack (optional but recommended)

2. **Install GDB debugger:**
   ```bash
   sudo apt-get install gdb
   ```

### How to Debug

1. **Open the project in VSCode:**
   ```bash
   code /home/slavakupreev/_projects/spaulding/ECG2100iQFirmware/LinuxB122DevkitProtocol
   ```

2. **Set breakpoints:**
   - Click in the left margin of `main.c` to set breakpoints
   - Common places: `main()`, `configure_serial_port()`, read loop

3. **Start debugging:**
   - Press `F5` or go to Run → Start Debugging
   - Select one of the debug configurations:
     - **Debug BT122 Monitor** - Normal debugging
     - **Debug BT122 Monitor (with sudo)** - If you need elevated permissions
     - **Debug BT122 Monitor (custom port)** - Use /dev/ttyUSB1

4. **Debug controls:**
   - `F5` - Continue
   - `F10` - Step over
   - `F11` - Step into
   - `Shift+F11` - Step out
   - `Ctrl+Shift+F5` - Restart
   - `Shift+F5` - Stop

### Available Debug Configurations

The `.vscode/launch.json` includes three configurations:

1. **Debug BT122 Monitor** - Standard debugging on `/dev/ttyUSB0`
2. **Debug BT122 Monitor (with sudo)** - Run with elevated permissions
3. **Debug BT122 Monitor (custom port)** - Use `/dev/ttyUSB1` (edit args in launch.json)

### VSCode Build Tasks

Press `Ctrl+Shift+B` to access build tasks:
- **build** (default) - Build release version
- **build-debug** - Build with debug symbols
- **clean** - Remove build artifacts
- **rebuild** - Clean and rebuild
- **run** - Build and run

## Serial Port Configuration

The program configures the serial port with the following parameters:
- **Baud rate**: 115200
- **Data bits**: 8
- **Stop bits**: 1
- **Parity**: None
- **Flow control**: Hardware (RTS/CTS)
- **Mode**: Raw (no processing)

This is equivalent to the `stty` command:
```bash
stty -F /dev/ttyUSB0 115200 raw cs8 -cstopb -parenb crtscts -echo
```

## Permissions

If you get a "Permission denied" error, you need to add your user to the `dialout` group:

```bash
sudo usermod -a -G dialout $USER
```

Then log out and log back in for the changes to take effect.

Alternatively, run with sudo (not recommended for regular use):
```bash
sudo ./bt122_monitor
```

## Cleaning

```bash
make clean
```

## Installation

To install to `/usr/local/bin`:
```bash
sudo make install
```

## Troubleshooting

### Port doesn't exist
Check available USB serial devices:
```bash
ls -l /dev/ttyUSB*
```

### Check current port settings
```bash
stty -F /dev/ttyUSB0 -a
```

### Test port connectivity
```bash
# In one terminal
echo "test" > /dev/ttyUSB0

# In another terminal
cat /dev/ttyUSB0
```
