#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#include <stdint.h>

// Firmware update configuration
#define FW_UPDATE_PAGE_SIZE 256  // ATxmega64A3U page size
#define FW_UPDATE_MAX_SIZE  65536 // Maximum firmware size in bytes (conservative estimate for application space)
#define FW_HEX_LINE_BUFFER_SIZE 256  // Buffer for accumulating partial hex lines

// Firmware update state structure
typedef struct {
    uint8_t active;
    uint8_t headerVerified;  // Flag to track if we've verified this is a valid firmware file
    uint32_t address;
    uint32_t size;
    uint16_t checksum;
    uint8_t pageBuffer[FW_UPDATE_PAGE_SIZE];
    char lineBuffer[FW_HEX_LINE_BUFFER_SIZE];  // Buffer for partial hex lines
    uint8_t lineBufferPos;                     // Current position in line buffer
} FirmwareUpdate_t;

// Global firmware update state
extern FirmwareUpdate_t fwUpdate;

// Function prototypes
void FW_InitUpdate(void);
void FW_CancelUpdate(void);
uint8_t FW_ParseHexRecord(const char *line);
uint8_t FW_IsUpdateActive(void);
void FW_PrintStatus(void);
uint8_t FW_ProcessPacketData(const uint8_t *data, uint16_t length);

#endif // UPDATE_HANDLER_H