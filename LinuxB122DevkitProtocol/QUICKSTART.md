# Quick Start Guide

## 1. Build and Run (Simplest Method)

```bash
./build_and_run.sh
```

That's it! The script handles everything.

## 2. Open in VSCode

```bash
code .
```

Or from VSCode: File → Open Folder → Select this directory

## 3. Debug in VSCode

### First Time Setup:
1. Install C/C++ extension in VSCode (Extension ID: `ms-vscode.cpptools`)
2. Install GDB: `sudo apt-get install gdb`

### Start Debugging:
1. Open `main.c` in VSCode
2. Click in the left margin to set a breakpoint (try line with `printf("Serial port opened...")`)
3. Press **F5** to start debugging
4. Select **"Debug BT122 Monitor"** from the dropdown

### If You Need Root Permissions:
Select **"Debug BT122 Monitor (with sudo)"** instead

## 4. Manual Build

```bash
# Build release version
make

# Build debug version (for GDB)
make debug

# Clean
make clean

# Run
./bt122_monitor
```

## 5. Common Serial Port Issues

### Port doesn't exist?
Check available ports:
```bash
ls -l /dev/ttyUSB* /dev/ttyACM*
```

### Permission denied?
Add yourself to dialout group (one time):
```bash
sudo usermod -a -G dialout $USER
```
Then log out and log back in.

### Temporary fix:
```bash
sudo ./bt122_monitor
# or
sudo ./build_and_run.sh
```

## 6. Testing Without Hardware

To test the program without actual hardware:

### Create a virtual serial port pair:
```bash
# Install socat
sudo apt-get install socat

# Create virtual ports
socat -d -d pty,raw,echo=0 pty,raw,echo=0
# Note the device names (e.g., /dev/pts/3 and /dev/pts/4)

# In another terminal, run the monitor
./bt122_monitor /dev/pts/3

# In third terminal, send test data
echo "Hello BT122" > /dev/pts/4
```

## VSCode Keyboard Shortcuts

- **F5** - Start/Continue debugging
- **Shift+F5** - Stop debugging
- **F9** - Toggle breakpoint
- **F10** - Step over
- **F11** - Step into
- **Shift+F11** - Step out
- **Ctrl+Shift+B** - Build

## Project Files Overview

```
LinuxB122DevkitProtocol/
├── main.c                      # Source code
├── Makefile                    # Build system
├── build_and_run.sh           # Automated build & run script
├── README.md                   # Full documentation
├── QUICKSTART.md              # This file
└── .vscode/                    # VSCode configuration
    ├── launch.json             # Debug configurations
    ├── tasks.json              # Build tasks
    ├── c_cpp_properties.json   # IntelliSense settings
    └── settings.json           # Editor settings
```
