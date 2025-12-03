#include "bgapi_parser.h"
#include <stdio.h>
#include <string.h>

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
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_BT_RFCOMM) {
        switch (event_id) {
            case 0x01: return "start_server";
            case 0x03: return "stop_server";
            default:   return "unknown";
        }
    } else if (class_id == BGAPI_CLASS_ENDPOINT) {
        switch (event_id) {
            case 0x00: return "send";
            case 0x02: return "status";
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
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║ BGAPI Packet                                                 ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ Type:     %-49s ║\n", bgapi_get_message_type_string(packet->message_type));
    printf("║ Class:    0x%02X %-44s ║\n", packet->class_id, bgapi_get_class_name(packet->class_id));
    printf("║ Event/Cmd: 0x%02X %-43s ║\n", packet->command_id, bgapi_get_event_name(packet->class_id, packet->command_id));
    printf("║ Length:   %3d bytes                                          ║\n", packet->payload_length);
    printf("╠══════════════════════════════════════════════════════════════╣\n");

    // Decode specific events and responses
    if (packet->message_type == BGAPI_MSG_TYPE_RESPONSE) {
        printf("║ >>> This is a RESPONSE to a command                         ║\n");
        if (packet->payload_length >= 2) {
            uint16_t error_code = packet->payload[0] | (packet->payload[1] << 8);
            if (error_code != 0) {
                printf("║ >>> Error Code: 0x%04X                                       ║\n", error_code);
            } else {
                printf("║ >>> Command successful (no error)                           ║\n");
            }
        } else if (packet->payload_length == 1 && packet->payload[0] != 0) {
            printf("║ >>> Error Code: 0x%02X                                         ║\n", packet->payload[0]);
        }
    } else if (packet->class_id == BGAPI_CLASS_SYSTEM && packet->command_id == BGAPI_EVENT_SYSTEM_BOOT) {
        if (packet->payload_length >= 12) {
            uint16_t major = packet->payload[0] | (packet->payload[1] << 8);
            uint16_t minor = packet->payload[2] | (packet->payload[3] << 8);
            uint16_t patch = packet->payload[4] | (packet->payload[5] << 8);
            uint16_t build = packet->payload[6] | (packet->payload[7] << 8);
            uint16_t ll_version = packet->payload[8] | (packet->payload[9] << 8);
            uint8_t protocol_version = packet->payload[10];
            uint8_t hw = packet->payload[11];

            printf("║ Firmware: v%d.%d.%d build %d                                 ║\n", major, minor, patch, build);
            printf("║ Link Layer: v%d                                              ║\n", ll_version);
            printf("║ Protocol: v%d, Hardware: 0x%02X                              ║\n", protocol_version, hw);
        }
    } else if (packet->class_id == BGAPI_CLASS_CONNECTION && packet->command_id == 0x04) {
        // Disconnected event
        if (packet->payload_length >= 3) {
            uint8_t connection = packet->payload[0];
            uint16_t reason = packet->payload[1] | (packet->payload[2] << 8);
            printf("║ Connection: %d, Reason: 0x%04X                               ║\n", connection, reason);
        }
    } else if (packet->class_id == BGAPI_CLASS_CONNECTION && packet->command_id == 0x00) {
        // Connection status
        if (packet->payload_length >= 8) {
            uint8_t connection = packet->payload[0];
            uint8_t flags = packet->payload[1];
            printf("║ Connection: %d, Flags: 0x%02X                                ║\n", connection, flags);
            printf("║ Address: %02X:%02X:%02X:%02X:%02X:%02X                           ║\n",
                   packet->payload[7], packet->payload[6], packet->payload[5],
                   packet->payload[4], packet->payload[3], packet->payload[2]);
        }
    } else if (packet->class_id == BGAPI_CLASS_GATT && packet->command_id == 0x00) {
        // GATT attribute value
        if (packet->payload_length >= 2) {
            uint8_t connection = packet->payload[0];
            uint16_t handle = packet->payload[1] | (packet->payload[2] << 8);
            printf("║ Connection: %d, Attribute Handle: 0x%04X                    ║\n", connection, handle);
        }
    } else if (packet->class_id == BGAPI_CLASS_GATT && packet->command_id == 0x01) {
        // GATT procedure completed
        if (packet->payload_length >= 3) {
            uint8_t connection = packet->payload[0];
            uint16_t result = packet->payload[1] | (packet->payload[2] << 8);
            printf("║ Connection: %d, Result: 0x%04X                              ║\n", connection, result);
        }
    } else if (packet->class_id == BGAPI_CLASS_GATT && packet->command_id == 0x02) {
        // GATT attribute status
        if (packet->payload_length >= 3) {
            uint16_t handle = packet->payload[0] | (packet->payload[1] << 8);
            uint8_t flags = packet->payload[2];
            printf("║ Attribute Handle: 0x%04X, Flags: 0x%02X                     ║\n", handle, flags);
        }
    }

    // Print raw payload
    if (packet->payload_length > 0) {
        printf("╠══════════════════════════════════════════════════════════════╣\n");
        printf("║ Payload (hex):                                               ║\n");

        for (int i = 0; i < packet->payload_length; i += 16) {
            printf("║ ");
            for (int j = 0; j < 16 && (i + j) < packet->payload_length; j++) {
                printf("%02X ", packet->payload[i + j]);
            }
            // Pad to align
            for (int j = packet->payload_length - i; j < 16; j++) {
                printf("   ");
            }
            printf("║\n");
        }
    }

    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}
