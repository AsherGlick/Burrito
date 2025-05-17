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

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
struct BurritoFrameMessage {
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
static_assert(sizeof(struct BurritoFrameMessage) == 57, "BurritoFrameMessage is expected to be 57 bytes long.");

struct BurritoFrameMessage buildBurritoFrameMessage(
    const float camera_position[3],
    const float camera_front[3],
    const float avatar_position[3],
    const float map_offset_x,
    const float map_offset_y,
    const float map_scale,
    const float compass_rotation,
    const uint32_t ui_state
) {
    struct BurritoFrameMessage data;
    data.message_type = 0x01;
    data.camera_position[0] = camera_position[0];
    data.camera_position[1] = camera_position[1];
    data.camera_position[2] = camera_position[2];
    data.camera_front[0] = camera_front[0];
    data.camera_front[1] = camera_front[1];
    data.camera_front[2] = camera_front[2];
    data.avatar_position[0] = avatar_position[0];
    data.avatar_position[1] = avatar_position[1];
    data.avatar_position[2] = avatar_position[2];
    data.map_offset_x = map_offset_x;
    data.map_offset_y = map_offset_y;
    data.map_scale = map_scale;
    data.compass_rotation = compass_rotation;
    data.ui_state = ui_state;
    return data;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
struct BurritoMetadataMessage {
    uint8_t message_type;  // 0x02
    uint16_t compass_width;
    uint16_t compass_height;
    uint32_t map_id;
    uint32_t x11_window_id;
    uint32_t identity_size;
    char utf8_identity[1024];
} __attribute__((packed));
#define BurritoMetadataMessageFixedSize offsetof(struct BurritoMetadataMessage, utf8_identity)
static_assert(BurritoMetadataMessageFixedSize == 17, "error");
static_assert(sizeof(((struct BurritoMetadataMessage*)0)->utf8_identity) == 1024, "error");
static_assert(sizeof(struct BurritoMetadataMessage) == 1041, "BurritoMetadataMessage is expected to be 1041 bytes long.");

struct BurritoMetadataMessage buildBurritoMetadataMessage(
    uint16_t compass_width,
    uint16_t compass_height,
    uint32_t map_id,
    uint32_t x11_window_id,
    uint16_t* identity
) {
    struct BurritoMetadataMessage data;
    data.message_type = 0x02;
    data.compass_width = compass_width;
    data.compass_height = compass_height;
    data.map_id = map_id;
    data.x11_window_id = x11_window_id;

    // Convert the JSON 'identity' payload from widechar to utf8 encoded char
    data.identity_size = WideCharToMultiByte(
        CP_UTF8,  // CodePage
        0,  // dwFlags
        identity,  // lpWideCharStr
        -1,  // cchWideChar
        data.utf8_identity,  // lpMultiByteStr
        1024,  // cbMultiByte
        NULL,  // lpDefaultChar
        NULL  // lpUsedDefaultChar
    );

    return data;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
struct BurritoTimeoutMessage {
    uint8_t message_type;  // 0x03
} __attribute__((packed));
static_assert(sizeof(struct BurritoTimeoutMessage) == 1, "BurritoTimeoutMessage is expected to be 1 byte long.");





// Sanity check our various datatypes on compile.
static_assert(sizeof(float) == 4, "float is expected to be 32 bits long.");
static_assert(sizeof(uint8_t) == 1, "uint8_t is expected to be 8 bits long.");
static_assert(sizeof(uint16_t) == 2, "uint16_t is expected to be 16 bits long.");
static_assert(sizeof(uint32_t) == 4, "uint32_t is expected to be 32 bits long.");
static_assert(sizeof(wchar_t) == 2, "wchar_t is expected to be 16 bits long.");

#endif  // BURRITO_LINK_SERIALIZER_H_
