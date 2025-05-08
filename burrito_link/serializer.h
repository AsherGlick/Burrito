#ifndef BURRITO_LINK_SERIALIZER_H_
#define BURRITO_LINK_SERIALIZER_H_
// This file will contain macros or somesuch that helps identify how things should be serialized.
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
// winsock2.h must be imported before windows.h or a warning is thrown.
// clang-format wants to order these alphabetically which would put winsock2.h
// after windows.h so clang-format must be disabled for this import.
// clang-format off
#include <winsock2.h>
#include <windows.h>
// clang-format on

#include "linked_memory.h"

// The max buffer size for data that is being sent to burrito over the UDP socket
#define MAX_BUFFER_SIZE 1024

// Enumerations of the different packet types that can be sent
#define PACKET_FRAME 1
#define PACKET_METADATA 2
#define PACKET_LINK_TIMEOUT 3

struct BurritoFrameData {
    uint8_t message_type;  // 0x01
    float camera_position[3];
    float camera_front[3];
    float avatar_position[3];
    float map_offset_x;
    float map_offset_y;
    float map_scale;
    float compass_rotation;
    uint32_t ui_state;
} __attribute__((packed));
static_assert(sizeof(struct BurritoFrameData) == 57, "BurritoFrameData is expected to be 57 bytes long.");

struct MetadataMessage {
    uint8_t message_type;  // 0x02
    uint16_t compass_width;
    uint16_t compass_height;
    uint32_t map_id;
    uint32_t x11_window_id;
    uint32_t identity_size;
    uint8_t utf8_identity[1024];
} __attribute__((packed));
#define MetadataMessageFixedSize offsetof(struct MetadataMessage, utf8_identity)
static_assert(MetadataMessageFixedSize == 17, "error");
static_assert(sizeof(((struct MetadataMessage*)0)->utf8_identity) == 1024, "error");
static_assert(sizeof(struct MetadataMessage) == 1041, "MetadataMessage is expected to be 1041 bytes long.");

struct TimeoutMessage {
    uint8_t message_type;  // 0x03
} __attribute__((packed));
static_assert(sizeof(struct TimeoutMessage) == 1, "TimeoutMessage is expected to be 1 byte long.");

// Sanity check our various datatypes on compile.
static_assert(sizeof(float) == 4, "float is expected to be 32 bits long.");
static_assert(sizeof(uint8_t) == 1, "uint8_t is expected to be 8 bits long.");
static_assert(sizeof(uint16_t) == 2, "uint16_t is expected to be 16 bits long.");
static_assert(sizeof(uint32_t) == 4, "uint32_t is expected to be 32 bits long.");
static_assert(sizeof(wchar_t) == 2, "wchar_t is expected to be 16 bits long.");

void send_metadata_message(
    struct MetadataMessage* message,
    SOCKET SendingSocket,
    SOCKADDR_IN ReceiverAddr
) {
    uint32_t total_bytes_sent = 0;
    total_bytes_sent = sendto(SendingSocket, (const char*)message, sizeof(*message), 0, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr));
    if (total_bytes_sent != sizeof(*message)) {
        printf("Not all Bytes Sent");
    }
}

#endif  // BURRITO_LINK_SERIALIZER_H_
