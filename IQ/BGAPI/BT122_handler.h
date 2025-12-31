#ifndef BT122_HANDLER_H
#define BT122_HANDLER_H

#include <stdint.h>

// Initialize BT122 module and wait for it to be ready
// Returns 0 on success, -1 on timeout
int BT122_handler_init(const char *device_name, int timeout_ms);
void BT122_handler_data_send(uint8_t endpoint, const uint8_t *data, uint8_t data_len);
void BT122_handler_reset_to_discoverable(void);

#endif // BT122_HANDLER_H
