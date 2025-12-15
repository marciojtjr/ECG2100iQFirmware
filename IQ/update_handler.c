#ifndef BOOTLOADER
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "update_handler.h"
#include "sp_driver.h"
#include "wdt_driver.h"
#include "usart_driver.h"

// Global firmware update state
FirmwareUpdate_t fwUpdate = {0};

// External references for USB output
extern FILE *usb;
#define print_usb(fmt,...)  fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)

//==========================================================================================
// Initialize firmware update
void FW_InitUpdate(void) {
    fwUpdate.active = 1;
    fwUpdate.headerVerified = 0;  // Reset verification flag
    fwUpdate.address = 0;
    fwUpdate.size = 0;
    fwUpdate.checksum = 0;
    fwUpdate.lineBufferPos = 0;
    memset(fwUpdate.pageBuffer, 0xFF, FW_UPDATE_PAGE_SIZE);
    memset(fwUpdate.lineBuffer, 0, FW_HEX_LINE_BUFFER_SIZE);
    WDT_Disable();
}

//==========================================================================================
// Cancel firmware update
void FW_CancelUpdate(void) {
    if(fwUpdate.active) {
        fwUpdate.active = 0;
        print_usb("Firmware update cancelled\r\n");
        // Re-enable watchdog
        WDT_EnableAndSetTimeout(WDT_PER_8KCLK_gc);
    }
    else {
        print_usb("No firmware update in progress\r\n");
    }
}

//==========================================================================================
// Check if update is active
uint8_t FW_IsUpdateActive(void) {
    return fwUpdate.active;
}

//==========================================================================================
// Print update status
void FW_PrintStatus(void) {
    if(fwUpdate.active) {
        print_usb("Update in progress - Size: %lu bytes\r\n", fwUpdate.size);
    }
    else {
        print_usb("No update in progress\r\n");
    }
}

//==========================================================================================
// Parse Intel HEX record
// Returns: 0 = success, 1 = error
uint8_t FW_ParseHexRecord(const char *line) {
    uint8_t count, type, checksum = 0;
    uint16_t addr;
    uint8_t data[32];
    uint8_t i;
    char hex[3] = {0, 0, 0};
    
    // Intel HEX format: :LLAAAATT[DD...]CC
    // LL = byte count, AAAA = address, TT = type, DD = data, CC = checksum
    
    if(line[0] != ':') return 1;  // Not a valid Intel HEX line
    
    // Parse byte count
    hex[0] = line[1]; hex[1] = line[2];
    count = (uint8_t)strtol(hex, NULL, 16);
    checksum += count;
    
    // Parse address
    hex[0] = line[3]; hex[1] = line[4];
    addr = (uint16_t)strtol(hex, NULL, 16) << 8;
    checksum += (addr >> 8);
    hex[0] = line[5]; hex[1] = line[6];
    addr |= (uint16_t)strtol(hex, NULL, 16);
    checksum += (addr & 0xFF);
    
    // Parse record type
    hex[0] = line[7]; hex[1] = line[8];
    type = (uint8_t)strtol(hex, NULL, 16);
    checksum += type;
    
    // If this is the first record, verify it's a valid firmware file
    if(!fwUpdate.headerVerified) {
        // For ATxmega64A3U, application starts at 0x0000
        if(type == 0x00 && addr < 0x10000) {
            fwUpdate.headerVerified = 1;
            print_usb("Firmware verified - Type:%02X Addr:%04X\r\n", type, addr);
        } else {
            print_usb("ERROR: Invalid firmware - Type:%02X Addr:%04X\r\n", type, addr);
            return 1;
        }
    }
    
    // Parse data bytes
    for(i = 0; i < count; i++) {
        hex[0] = line[9 + i*2]; hex[1] = line[10 + i*2];
        data[i] = (uint8_t)strtol(hex, NULL, 16);
        checksum += data[i];
    }
    
    // Parse and verify checksum
    hex[0] = line[9 + count*2]; hex[1] = line[10 + count*2];
    uint8_t file_checksum = (uint8_t)strtol(hex, NULL, 16);
    checksum = (~checksum + 1) & 0xFF;
    
    if(checksum != file_checksum) {
        print_usb("Checksum error\r\n");
        return 1;
    }
    
    // Process based on record type
    switch(type) {
        case 0x00:  // Data record
            // Verify address is within application space
            if(addr >= 0x10000) {
                print_usb("ERROR: Address 0x%04X outside application space!\r\n", addr);
                return 1;
            }
            
            // Check if firmware size exceeds maximum
            if(fwUpdate.size + count > FW_UPDATE_MAX_SIZE) {
                print_usb("ERROR: Firmware too large! Max size is %u bytes\r\n", FW_UPDATE_MAX_SIZE);
                return 1;
            }
            
            // Add data to page buffer and write when page is complete
            for(i = 0; i < count; i++) {
                uint32_t absolute_addr = addr + i;  // Current byte address
                uint16_t buffer_offset = absolute_addr % FW_UPDATE_PAGE_SIZE;
                
                // Store byte in buffer
                fwUpdate.pageBuffer[buffer_offset] = data[i];
                fwUpdate.checksum += data[i];
                
                // Check if we've filled a page (reached the last byte of a page)
                if(buffer_offset == (FW_UPDATE_PAGE_SIZE - 1)) {
                    // Calculate the page start address
                    uint32_t page_addr = (absolute_addr / FW_UPDATE_PAGE_SIZE) * FW_UPDATE_PAGE_SIZE;
                    
                    // Verify we're not overwriting bootloader area
                    if(page_addr < 0x10000) {
                        FW_ProgramFlashPage(page_addr);
                        
                        // Clear page buffer for next page
                        memset(fwUpdate.pageBuffer, 0xFF, FW_UPDATE_PAGE_SIZE);
                        
                        // Show progress every 16 pages (4KB) with a simple dot
                        static uint8_t page_count = 0;
                        page_count++;
                        if((page_count % 16) == 0) {
                            //print_usb(".");
                        }
                    } else {
                        print_usb("ERROR: Attempted to write to bootloader area at 0x%08lX!\r\n", page_addr);
                        return 1;
                    }
                }
            }
            fwUpdate.size += count;
            break;
            
        case 0x01:  // End of file
            // Write any remaining data in page buffer (partial page)
            if(fwUpdate.size % FW_UPDATE_PAGE_SIZE != 0) {
                // Calculate the last page address that needs writing
                uint32_t last_page_addr = (fwUpdate.size / FW_UPDATE_PAGE_SIZE) * FW_UPDATE_PAGE_SIZE;
                
                if(last_page_addr < 0x10000) {
                    FW_ProgramFlashPage(last_page_addr);
                    print_usb("\r\nWriting final page at 0x%04lX\r\n", last_page_addr);
                }
            }
            
            print_usb("\r\n========================================\r\n");
            print_usb("Firmware update complete!\r\n");
            print_usb("Size: %lu bytes\r\n", fwUpdate.size);
            print_usb("Checksum: 0x%04X\r\n", fwUpdate.checksum);
            print_usb("Pages written: %lu\r\n", (fwUpdate.size + FW_UPDATE_PAGE_SIZE - 1) / FW_UPDATE_PAGE_SIZE);
            print_usb("========================================\r\n");
            print_usb("Type 'fwreset' to restart with new firmware\r\n");
            
            fwUpdate.active = 0;
            // Re-enable watchdog
			cli();
			// Use watchdog to reset
			WDT_EnableAndSetTimeout(WDT_PER_8CLK_gc);
			while(1);  // Wait for watchdog reset
            
            return 0;
            
        case 0x02:  // Extended segment address (not used for AVR)
        case 0x03:  // Start segment address (not used)  
        case 0x04:  // Extended linear address (for >64K, not needed for ATxmega64)
        case 0x05:  // Start linear address (not used)
            // These record types are not needed for ATxmega64A3U
            break;
            
        default:
            print_usb("Unknown record type: 0x%02X\r\n", type);
            return 1;
    }
    
    
    return 0;
}

//==========================================================================================
// Flash page programming function - isolated to minimize interrupt disabled time
void FW_ProgramFlashPage(uint32_t page_addr) {
    /*print_usb("[FLASH] Programming page at 0x%08lX\r\n", page_addr);
    
    // Debug: Print first 16 bytes of page buffer
    print_usb("[FLASH] Buffer: ");
    for(int i = 0; i < 16; i++) {
        print_usb("%02X ", fwUpdate.pageBuffer[i]);
    }
    print_usb("...\r\n");*/
    
    cli();  // Disable interrupts during flash write
    SP_LoadFlashPage(fwUpdate.pageBuffer);
    SP_EraseWriteApplicationPage(page_addr);
    SP_WaitForSPM();
    sei();  // Re-enable interrupts*/
}

//==========================================================================================
// Process packet data from XMODEM - safely accumulates and parses hex lines
// Returns: 0 = success, 1 = error
uint8_t FW_ProcessPacketData(const uint8_t *data, uint16_t length) {
    uint16_t i;
    
    // Process each byte from the packet
    for(i = 0; i < length; i++) {
        uint8_t byte = data[i];
        
        // Skip non-printable characters except CR/LF/colon
        if(byte < 32 && byte != '\r' && byte != '\n') {
            continue;
        }
        
        // Check for buffer overflow
        if(fwUpdate.lineBufferPos >= (FW_HEX_LINE_BUFFER_SIZE - 2)) {
            print_usb("Error: Line buffer overflow\r\n");
            return 1;
        }
        
        // Add byte to line buffer
        fwUpdate.lineBuffer[fwUpdate.lineBufferPos] = byte;
        fwUpdate.lineBufferPos++;
        
        // Check for end of line
        if(byte == '\n') {
            // Null terminate the line
            fwUpdate.lineBuffer[fwUpdate.lineBufferPos] = '\0';
            
            // Remove trailing \r if present
            if(fwUpdate.lineBufferPos > 1 && fwUpdate.lineBuffer[fwUpdate.lineBufferPos-2] == '\r') {
                fwUpdate.lineBuffer[fwUpdate.lineBufferPos-2] = '\0';
            }
            
            // Process the line if it's a hex record
            if(fwUpdate.lineBuffer[0] == ':') {
                if(FW_ParseHexRecord(fwUpdate.lineBuffer) != 0) {
                    print_usb("Error parsing hex line: %s\r\n", fwUpdate.lineBuffer);
                    return 1;
                }
            }
            
            // Reset buffer for next line
            fwUpdate.lineBufferPos = 0;
            memset(fwUpdate.lineBuffer, 0, FW_HEX_LINE_BUFFER_SIZE);
        }
    }
    
    return 0;
}
#endif
