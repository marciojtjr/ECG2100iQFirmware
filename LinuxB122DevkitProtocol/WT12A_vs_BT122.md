# WT12-A vs BT122 Comparison

## Module Identification

Your firmware supports BOTH modules and auto-detects which one is connected.

### WT12-A (Bluetooth Classic)
- **Protocol**: iWRAP (text commands like `"SET BT NAME\r"`)
- **Detection**: Lines 397-478 in TaskBT.c
- **Baud Rate**: 921600 bps
- **Response**: Text strings like `"READY\r\n"`

### BT122 (Bluetooth Low Energy)
- **Protocol**: BGAPI (binary like `0xA0 0x06 0x01 0x01...`)
- **Detection**: Lines 390-396 in TaskBT.c (checks for BGAPI `0x20` response)
- **Baud Rate**: 115200 bps
- **Response**: Binary packets

## Feature Comparison

| Feature | WT12-A (Classic BT) | BT122 (BLE) | Status |
|---------|---------------------|-------------|--------|
| **Apple iAP** | ✅ Supported | ❌ Not available | BLE doesn't support iAP |
| **SPP/RFCOMM** | ✅ Supported | ❌ Not available | Use GATT instead |
| **Device Name** | `SET BT NAME` | BGAPI PS Store | ✅ Implemented |
| **Pairing** | PIN/SSP | BLE pairing | Different mechanisms |
| **Data Transfer** | SPP stream | GATT notifications | Different APIs |
| **iOS Support** | iAP2 protocol | BLE GATT | Both work, different methods |

## WT12-A Initialization (Your Firmware)

```c
// Text-based iWRAP commands
print_bt("SET CONTROL INIT SET CONTROL AUTOCALL\r");
print_bt("SET CONTROL CD 20 0\r");
print_bt("SET BT NAME SpauCliQ%02d%02d%04d\r", serial_num...);
print_bt("SET BT PAIRCOUNT 1\r");
print_bt("SET CONTROL CONFIG 0000 2000 2000 1980\r");
print_bt("SET PROFILE IAP ...\r");  // Apple iAP2
print_bt("SDP ADD 72243468... SpauldingECGRFCOMM\r");
```

## BT122 Equivalent (This Program)

```c
// Binary BGAPI commands
bgapi_cmd_system_get_info(&cmd);               // Get module info
bt122_set_device_name("SpauCliQ_BLE");         // Set name (PS store)
bgapi_cmd_gap_set_mode(&cmd, 0x02, 0x02);     // Discoverable/connectable
// Note: No iAP, no SPP - use GATT services instead
```

## What CANNOT Be Ported to BT122

### 1. Apple iAP Profile
```c
// WT12-A: SUPPORTED
print_bt("SET PROFILE IAP ...\r");

// BT122: NOT POSSIBLE
// BLE doesn't have iAP. Use standard BLE GATT for iOS communication.
```

### 2. SPP/RFCOMM
```c
// WT12-A: SUPPORTED
print_bt("SDP ADD 72243468-25ee-4025-8fe7-1edd2cf13cd2 SpauldingECGRFCOMM\r");

// BT122: NOT POSSIBLE
// BLE doesn't have SPP. Use GATT characteristics for data transfer.
```

### 3. CD Pin Control
```c
// WT12-A: SUPPORTED
print_bt("SET CONTROL CD 20 0\r");  // Set carrier detect pin

// BT122: NOT APPLICABLE
// BLE connection model is different
```

## Data Transfer Methods

### WT12-A (Classic BT)
```
Phone → SPP/RFCOMM → UART → Your MCU
(Stream-based, like serial port)
```

### BT122 (BLE)
```
Phone → GATT Characteristic → Notifications → UART → Your MCU
(Packet-based, 20 bytes max per packet)
```

## Recommendations

### If You Need Apple iAP:
- **Use WT12-A module** (not BT122)
- iAP is only available on Bluetooth Classic

### If You Can Use Standard BLE:
- **Use BT122** (lower power, works with all modern phones)
- Implement custom GATT service for ECG data
- iOS and Android both support BLE GATT

### For Your Application (ECG Device):
The firmware already detects the module type:
1. **If BT122 detected** → Uses BGAPI, no iAP (THIS PROGRAM)
2. **If WT12-A detected** → Uses iWRAP, has iAP support (lines 397-478)

## Testing Your Setup

Run the program and look for:
```
╔══════════════════════════════════════════════════════════════╗
║ *** BT122 Module Ready! Starting Initialization... ***       ║
║                                                              ║
║ NOTE: BT122 is BLE (Bluetooth Low Energy), not Classic BT   ║
║ - No iAP support (use WT12-A for Apple iAP)                 ║
║ - Uses GATT instead of SPP/RFCOMM                           ║
╚══════════════════════════════════════════════════════════════╝
```

This confirms you have BT122, not WT12-A.
