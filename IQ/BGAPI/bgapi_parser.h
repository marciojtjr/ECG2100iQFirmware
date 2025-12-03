#ifndef BGAPI_PARSER_H
#define BGAPI_PARSER_H

#include <stdint.h>

// BGAPI Message Types
#define BGAPI_MSG_TYPE_COMMAND   0x00
#define BGAPI_MSG_TYPE_RESPONSE  0x20
#define BGAPI_MSG_TYPE_EVENT     0xA0

// BGAPI Class IDs
#define BGAPI_CLASS_SYSTEM       0x01
#define BGAPI_CLASS_BT_GAP       0x02
#define BGAPI_CLASS_CONNECTION   0x03
#define BGAPI_CLASS_BT_RFCOMM    0x04
#define BGAPI_CLASS_ATTCLIENT    0x05
#define BGAPI_CLASS_LE_GAP       0x06
#define BGAPI_CLASS_BT_CONNECTION 0x07
#define BGAPI_CLASS_ENDPOINT     0x0B
#define BGAPI_CLASS_GATT         0x0B
#define BGAPI_CLASS_PERSISTENT   0x0D
#define BGAPI_CLASS_SM           0x0F

// System Events
#define BGAPI_EVENT_SYSTEM_BOOT         0x00
#define BGAPI_EVENT_SYSTEM_INITIALIZED  0x01

// BT Connection Events  
#define BGAPI_EVENT_BT_CONNECTION_OPENED     0x00

// Endpoint Events
#define BGAPI_EVENT_ENDPOINT_STATUS     0x02
#define BGAPI_EVENT_ENDPOINT_DATA       0x06

// Legacy Connection Events (for compatibility)
#define BGAPI_EVENT_CONNECTION_STATUS       0x00
#define BGAPI_EVENT_CONNECTION_VERSION_IND  0x01
#define BGAPI_EVENT_CONNECTION_FEATURE_IND  0x02
#define BGAPI_EVENT_CONNECTION_DISCONNECTED 0x04

// BGAPI Packet Structure
typedef struct {
    uint8_t message_type;    // 0x00, 0x20, or 0xA0
    uint8_t payload_length;  // Length of payload (excluding 4-byte header)
    uint8_t class_id;        // Class ID
    uint8_t command_id;      // Command/Event ID
    uint8_t payload[256];    // Payload data
    uint16_t total_length;   // Total packet length (header + payload)
} bgapi_packet_t;

// Parser state
typedef struct {
    uint8_t buffer[512];
    uint16_t buffer_pos;
    uint16_t expected_length;
    uint8_t parsing;
} bgapi_parser_state_t;

// Function prototypes
void bgapi_parser_init(bgapi_parser_state_t *state);
int bgapi_parser_feed(bgapi_parser_state_t *state, const uint8_t *data, int length, bgapi_packet_t *packet);
void bgapi_print_packet(const bgapi_packet_t *packet);
const char* bgapi_get_message_type_string(uint8_t type);
const char* bgapi_get_class_name(uint8_t class_id);
const char* bgapi_get_event_name(uint8_t class_id, uint8_t event_id);

#endif // BGAPI_PARSER_H
