#ifndef BOOTLOADER
#include "bgapi_parser.h"
#include <stdio.h>
#include <string.h>
#include "../taskUSB.h"

void bgapi_parser_init(bgapi_parser_state_t *state) {
    memset(state, 0, sizeof(bgapi_parser_state_t));
    state->buffer_pos = 0;
    state->expected_length = 0;
    state->parsing = 0;
}

int bgapi_parser_feed(bgapi_parser_state_t *state, const uint8_t *data, int length, bgapi_packet_t *packet) {
    int packets_found = 0;

    for (int i = 0; i < length; i++) {
        uint8_t byte = data[i];

        // Check if this is the start of a BGAPI packet
        if (!state->parsing) {
            // BGAPI packets start with 0x00, 0x20, or 0xA0
            if (byte == BGAPI_MSG_TYPE_COMMAND ||
                byte == BGAPI_MSG_TYPE_RESPONSE ||
                byte == BGAPI_MSG_TYPE_EVENT) {
                state->buffer[0] = byte;
                state->buffer_pos = 1;
                state->parsing = 1;
                state->expected_length = 0; // Will be set when we get length byte
            }
            continue;
        }

        // Add byte to buffer
        state->buffer[state->buffer_pos++] = byte;

        // After header byte, next byte is length
        if (state->buffer_pos == 2) {
            state->expected_length = state->buffer[1] + 4; // payload + 4 byte header
        }

        // Check if we have a complete packet
        if (state->expected_length > 0 && state->buffer_pos >= state->expected_length) {
            // Parse the packet
            if (packet != NULL) {
                packet->message_type = state->buffer[0];
                packet->payload_length = state->buffer[1];
                packet->class_id = state->buffer[2];
                packet->command_id = state->buffer[3];
                packet->total_length = state->expected_length;

                // Copy payload
                for (int j = 0; j < packet->payload_length; j++) {
                    packet->payload[j] = state->buffer[4 + j];
                }
            }

            packets_found++;

            // Reset parser for next packet
            state->buffer_pos = 0;
            state->expected_length = 0;
            state->parsing = 0;

            // Return immediately after finding one packet
            // Caller should call again with remaining data
            return packets_found;
        }

        // Prevent buffer overflow
        if (state->buffer_pos >= sizeof(state->buffer)) {
            // Reset and try to resync
            state->buffer_pos = 0;
            state->expected_length = 0;
            state->parsing = 0;
        }
    }

    return packets_found;
}

const char* bgapi_get_message_type_string(uint8_t type) {
    switch (type) {
        case BGAPI_MSG_TYPE_COMMAND:  return "Command";
        case BGAPI_MSG_TYPE_RESPONSE: return "Response";
        case BGAPI_MSG_TYPE_EVENT:    return "Event";
        default:                       return "Unknown";
    }
}

const char* bgapi_get_class_name(uint8_t class_id) {
    switch (class_id) {
        case BGAPI_CLASS_SYSTEM:      return "System";
        case BGAPI_CLASS_BT_GAP:      return "BT GAP";
        case BGAPI_CLASS_CONNECTION:  return "Connection";
        case BGAPI_CLASS_BT_RFCOMM:   return "BT RFCOMM";
        case BGAPI_CLASS_ATTCLIENT:   return "ATT Client";
        case BGAPI_CLASS_LE_GAP:      return "LE GAP";
        case BGAPI_CLASS_BT_CONNECTION: return "BT Connection";
        case BGAPI_CLASS_ENDPOINT:    return "Endpoint";
        case BGAPI_CLASS_PERSISTENT:  return "Persistent Store";
        case BGAPI_CLASS_SM:          return "Security Manager";
        default:                       return "Unknown";
    }
}

const char* bgapi_get_event_name(uint8_t class_id, uint8_t event_id) {
    if (class_id == BGAPI_CLASS_SYSTEM) {
        switch (event_id) {
            case 0x00: return "boot";
            case 0x01: return "initialized";
            case 0x02: return "debug";
            case 0x03: return "endpoint_watermark_rx";
            case 0x04: return "endpoint_watermark_tx";
            case 0x05: return "set_local_name";
            case 0x06: return "set_class_of_device";
            case 0x08: return "get_info";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_BT_GAP) {
        switch (event_id) {
            case 0x03: return "set_mode";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_CONNECTION) {
        switch (event_id) {
            case 0x00: return "status";
            case 0x01: return "version_ind";
            case 0x02: return "feature_ind";
            case 0x03: return "raw_rx";
            case 0x04: return "disconnected";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_BT_CONNECTION) {
        switch (event_id) {
            case 0x00: return "opened";
            case 0x02: return "parameters";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_BT_RFCOMM) {
        switch (event_id) {
            case 0x00: return "opened";
            case 0x01: return "start_server";
            case 0x03: return "stop_server";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_ENDPOINT) {
        switch (event_id) {
            case 0x00: return "send";
            case 0x01: return "data";
            case 0x02: return "status";
            case 0x06: return "data_alt";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_GATT) {
        switch (event_id) {
            case 0x00: return "attribute_value";
            case 0x01: return "procedure_completed";
            case 0x02: return "attribute_status";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_SM) {
        switch (event_id) {
            case 0x00: return "set_bondable_mode";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_LE_GAP) {
        switch (event_id) {
            case 0x00: return "scan_response";
            case 0x01: return "mode_changed";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_PERSISTENT) {
        switch (event_id) {
            case 0x00: return "load";
            case 0x01: return "save";
            case 0x02: return "erase";
            default:   return "unknown";
        }
    }
    return "unknown";
}

void bgapi_print_packet(const bgapi_packet_t *packet) {
    // Enhanced BGAPI packet debug with detailed information
    const char* msg_type = bgapi_get_message_type_string(packet->message_type);
    const char* class_name = bgapi_get_class_name(packet->class_id);
    const char* event_name = bgapi_get_event_name(packet->class_id, packet->command_id);
    
    debug_message("BT[%s %s::%s T:0x%02X C:0x%02X E:0x%02X L:%d",
           msg_type, class_name, event_name,
           packet->message_type, packet->class_id,
           packet->command_id, packet->payload_length);

    // Print full payload for detailed analysis
    if (packet->payload_length > 0) {
        debug_message(" D:");
        for (int i = 0; i < packet->payload_length && i < 16; i++) {
            debug_message("%02X", packet->payload[i]);
        }
        if (packet->payload_length > 16) {
            debug_message("...");
        }
    }
    debug_message("]\n");
}
#endif