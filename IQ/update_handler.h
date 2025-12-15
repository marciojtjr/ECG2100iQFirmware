#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#include <stdint.h>

#define FW_UPDATE_PAGE_SIZE 256
#define FW_UPDATE_MAX_SIZE  65536

typedef struct {
    uint8_t active;
    uint8_t headerVerified;
    uint16_t size;
    uint16_t lastAddr;
    uint8_t pageBuffer[FW_UPDATE_PAGE_SIZE];
} FirmwareUpdate_t;

extern FirmwareUpdate_t fwUpdate;

void FW_InitUpdate(void);
void FW_CancelUpdate(void);
uint8_t FW_ParseHexRecord(const char *line);
uint8_t FW_IsUpdateActive(void);
void FW_ProgramFlashPage(uint16_t page_addr);

#endif
