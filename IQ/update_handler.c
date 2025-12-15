#ifdef BOOTLOADER 
#include <avr/io.h>
#include <string.h>
#include "update_handler.h"
#include "sp_driver.h"
#include "serial.h"

// Global firmware update state
FirmwareUpdate_t fwUpdate;

// Simple hex byte parser (replaces strtol - saves ~200 bytes)
static uint8_t hexToByte(char h, char l) {
    uint8_t val = 0;
    if(h >= '0' && h <= '9') val = (h - '0') << 4;
    else if(h >= 'A' && h <= 'F') val = (h - 'A' + 10) << 4;
    else if(h >= 'a' && h <= 'f') val = (h - 'a' + 10) << 4;
    if(l >= '0' && l <= '9') val |= (l - '0');
    else if(l >= 'A' && l <= 'F') val |= (l - 'A' + 10);
    else if(l >= 'a' && l <= 'f') val |= (l - 'a' + 10);
    return val;
}

void FW_InitUpdate(void) {
    fwUpdate.active = 1;
    fwUpdate.headerVerified = 0;
    fwUpdate.size = 0;
    fwUpdate.lastAddr = 0;
    memset(fwUpdate.pageBuffer, 0xFF, FW_UPDATE_PAGE_SIZE);
}

void FW_CancelUpdate(void) {
    fwUpdate.active = 0;
}

uint8_t FW_IsUpdateActive(void) {
    return fwUpdate.active;
}

// Print error with record
static void parseError(const char *msg, const char *line) {
    sendStr("[ERR] ");
    sendStr(msg);
    sendStr(": ");
    sendStr(line);
    sendStr("\r\n");
}

// Parse Intel HEX record - Returns: 0 = success, 1 = error
uint8_t FW_ParseHexRecord(const char *line) {
    uint8_t count, type, checksum = 0;
    uint16_t addr;
    uint8_t i;

    if(line[0] != ':') {
        parseError("No colon", line);
        return 1;
    }

    // Parse header: :LLAAAATT
    count = hexToByte(line[1], line[2]);
    checksum += count;

    addr = ((uint16_t)hexToByte(line[3], line[4]) << 8) | hexToByte(line[5], line[6]);
    checksum += (addr >> 8) + (addr & 0xFF);

    type = hexToByte(line[7], line[8]);
    checksum += type;

    // Verify first record
    if(!fwUpdate.headerVerified) {
        if(type == 0x00) fwUpdate.headerVerified = 1;
        else {
            parseError("Bad header", line);
            return 1;
        }
    }

    // Parse and checksum data bytes, store directly to page buffer for type 0x00
    for(i = 0; i < count; i++) {
        uint8_t byte = hexToByte(line[9 + i*2], line[10 + i*2]);
        checksum += byte;

        if(type == 0x00) {
            uint16_t absolute_addr = addr + i;
            uint8_t buffer_offset = absolute_addr & (FW_UPDATE_PAGE_SIZE - 1);
            fwUpdate.pageBuffer[buffer_offset] = byte;
            fwUpdate.lastAddr = absolute_addr;
            fwUpdate.size++;

            // Page complete?
            if(buffer_offset == (FW_UPDATE_PAGE_SIZE - 1)) {
                uint16_t page_addr = absolute_addr & ~(FW_UPDATE_PAGE_SIZE - 1);
                FW_ProgramFlashPage(page_addr);
                memset(fwUpdate.pageBuffer, 0xFF, FW_UPDATE_PAGE_SIZE);
            }
        }
    }

    // Verify checksum
    checksum = (~checksum + 1) & 0xFF;
    if(checksum != hexToByte(line[9 + count*2], line[10 + count*2])) {
        parseError("Checksum", line);
        return 1;
    }

    // End of file record
    if(type == 0x01) {
        // Write final partial page if needed
        if((fwUpdate.lastAddr & (FW_UPDATE_PAGE_SIZE - 1)) != (FW_UPDATE_PAGE_SIZE - 1)) {
            uint16_t page_addr = fwUpdate.lastAddr & ~(FW_UPDATE_PAGE_SIZE - 1);
            FW_ProgramFlashPage(page_addr);
        }
        fwUpdate.active = 0;
    }

    return 0;
}

// Flash page programming using sp_driver
void FW_ProgramFlashPage(uint16_t page_addr) {
    uint16_t i;

    SP_WaitForSPM();
    SP_EraseFlashBuffer();
    SP_WaitForSPM();

    for(i = 0; i < FW_UPDATE_PAGE_SIZE; i += 2) {
        uint16_t word = fwUpdate.pageBuffer[i] | ((uint16_t)fwUpdate.pageBuffer[i + 1] << 8);
        SP_LoadFlashWord(i, word);
    }

    SP_EraseWriteApplicationPage(page_addr);
    SP_WaitForSPM();
}
#endif