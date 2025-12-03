#ifndef BT122_HANDLER_H
#define BT122_HANDLER_H

#include <stdint.h>

// Initialize BT122 module and wait for it to be ready
// Returns 0 on success, -1 on timeout
int legacybt_handler_init(const char *device_name, int timeout_ms);

#endif // BT122_HANDLER_H
