# Module Detection Guide

## How to Identify Your Module

Your firmware supports TWO different Bluetooth modules. Here's how to tell which one you have:

### Method 1: Visual Inspection

**WT12-A Module:**
- Labeled "WT12-A" or "WT12" on the chip
- Larger module (approx 27mm x 13mm)
- Often has external antenna connector

**BT122 Module:**
- Labeled "BT122" or "BLE122" on the chip
- Smaller module (approx 15mm x 18mm)
- Integrated PCB antenna

### Method 2: Run Detection (this program does it automatically)

```bash
./bt122_monitor
```

**If you see BGAPI packets** (hex starting with `A0`, `20`):
```
Received 7 bytes:
Hex: A0 03 0B 00 85 01 00
```
→ **You have BT122** (BLE only)

**If you see text responses**:
```
Received:
READY
```
→ **You have WT12-A** (Classic Bluetooth)

### Method 3: Check Baud Rate

- **115200 bps** → BT122 (BLE)
- **921600 bps** → WT12-A (Classic BT)

## Module Capabilities

| Feature | WT12-A | BT122 | Your Need |
|---------|--------|-------|-----------|
| **Bluetooth Classic** | ✅ | ❌ | ❓ |
| **Bluetooth Low Energy** | ❌ | ✅ | ❓ |
| **Apple iAP/iAP2** | ✅ | ❌ | ❓ |
| **SPP (Serial Port)** | ✅ | ❌ | ❓ |
| **GATT Services** | ❌ | ✅ | ❓ |
| **iOS Support** | iAP2 | BLE GATT | Both work |
| **Android Support** | SPP | BLE GATT | Both work |
| **Power Consumption** | Higher | Lower | - |
| **Range** | Good | Good | - |

## What This Means For Your Application

### If You NEED Classic Bluetooth:

**Required Features:**
- Apple iAP/iAP2 protocol
- SPP/RFCOMM communication
- Classic Bluetooth profiles

**You MUST use:** WT12-A module

**BT122 CANNOT do this!**

### If You Can Use BLE:

**Features Available:**
- GATT services for data transfer
- Lower power consumption
- Works with all modern iOS/Android
- Custom GATT characteristics

**You CAN use:** BT122 module

## Current Situation

Based on your test output (`A0 03 0B 00...`), **you currently have BT122 connected**.

### If You Need Classic Bluetooth:

**Option 1: Replace Module**
- Order WT12-A module instead of BT122
- Connect to same UART pins
- Firmware will auto-detect and use iWRAP protocol

**Option 2: Modify Approach**
- Keep BT122 (BLE)
- Redesign iOS app to use BLE GATT instead of iAP
- Android app uses BLE instead of SPP
- Lower power consumption!

## Testing Different Module

To test WT12-A initialization (when you get the module):

1. Replace BT122 with WT12-A physically
2. Power cycle
3. Run this program:
   ```bash
   ./bt122_monitor
   ```

The program will:
1. Send BGAPI hello at 115200 bps
2. Get no response (WT12-A doesn't speak BGAPI)
3. Switch to 921600 bps
4. Wait for "READY" text response
5. Send iWRAP commands

**Note:** Current version only implements BGAPI (BT122). For full WT12-A support with iWRAP commands, we need to add text command handling.

## Recommendation

**Do you actually need Classic Bluetooth?**

- If YES (need iAP) → Get WT12-A module
- If NO (BLE is fine) → Keep BT122

Modern approach: **Most new devices use BLE** because:
- Lower power
- Better iOS/Android support
- Simpler security
- No iAP licensing needed

Only use Classic BT if you specifically need iAP or legacy SPP support.
