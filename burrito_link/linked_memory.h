#ifndef BURRITO_LINK_LINKED_MEMORY_H_
#define BURRITO_LINK_LINKED_MEMORY_H_

#include <assert.h>
#include <stdint.h>

#ifndef wchar_t
#define wchar_t uint16_t
#endif

////////////////////////////////////////////////////////////////////////////////
// LinkedMem struct
//
// This struct represents the Mumble Link shared memory datum that Guild Wars 2
// uses to communicate live player and camera data, as well as some other
// bits of information that are useful for tools like burrito.
//
// https://wiki.guildwars2.com/wiki/API:MumbleLink
// https://www.mumble.info/documentation/developer/positional-audio/link-plugin/
////////////////////////////////////////////////////////////////////////////////
struct LinkedMem {
    uint32_t uiVersion;

    // The current update tick
    uint32_t uiTick;

    // The XYZ location of the player
    float fAvatarPosition[3];

    // A 3D unit vector representing the forward direction of the player character
    float fAvatarFront[3];

    // A 3D unit vector representing the up direction of the player character
    float fAvatarTop[3];

    // The string "Guild Wars 2"
    wchar_t name[256];

    // The XYZ Position of the camera
    float fCameraPosition[3];

    // A 3D unit vector representing the forward direction of the camera
    float fCameraFront[3];

    // A 3D unit vector representing the up direction of the camera
    float fCameraTop[3];

    // A json string containing json data. See https://wiki.guildwars2.com/wiki/API:MumbleLink#identity
    wchar_t identity[256];

    // A value that is always 48
    uint32_t context_len;

    // A binary chunk containing another struct. See the MumbleContext struct below
    uint8_t context[256];

    // An Empty Array, this field is not used by guild wars 2
    wchar_t description[2048];
};


////////////////////////////////////////////////////////////////////////////////
// MumbleContext struct
//
// This struct represents the LinkedMem.context datum that is passed in
// LinkedMem. It is a struct that is entirely specific to Guild Wars 2 which
// is why it is seperated out from the more mumble-generic LinkedMem struct.
//
// https://wiki.guildwars2.com/wiki/API:MumbleLink#context
////////////////////////////////////////////////////////////////////////////////
struct MumbleContext {
    // The current address of the guild wars 2 server the player is connected to
    // can be a ipv4 `sockaddr_in` or a ipv6 `sockaddr_in6`
    uint8_t serverAddress[28];

    // The Guild Wars 2 id for the map the player is currently in
    uint32_t mapId;

    uint32_t mapType;
    uint32_t shardId;
    uint32_t instance;
    uint32_t buildId;

    // A bitmask of various boolean element of the UI state
    //   Bit 1 = IsMapOpen
    //   Bit 2 = IsCompassTopRight
    //   Bit 3 = DoesCompassHaveRotationEnabled
    //   Bit 4 = Game has focus
    //   Bit 5 = Is in Competitive game mode
    //   Bit 6 = Textbox has focus
    //   Bit 7 = Is in Combat
    uint32_t uiState;

    // The width of the minimap in pixels
    uint16_t compassWidth;

    // The height of the minimap in pixels
    uint16_t compassHeight;

    // The rotation of the minimap contents in radians
    float compassRotation;

    // The X location of the player in continentCoords
    float playerX;
    // The Y location of the player in continentCoords
    float playerY;

    // The center X of the current map in continentCoords
    float mapCenterX;
    // The center Y of the current map in continentCoords
    float mapCenterY;

    // The scale of how zoomed in the visible map or minimap is
    float mapScale;

    // The windows process id of the Guild Wars 2 process
    uint32_t processId;

    // An enum representing which mount is currenty being used by the player
    uint8_t mountIndex;

    // Extra bytes that are not currently accounted for or are unused in the context
    uint8_t _padding[171];
};

// Sanity check our memory map structs on compile.
static_assert(sizeof(struct LinkedMem) == 5460, "LinkedMem is expected to be 5460 bytes long.");
static_assert(sizeof(struct MumbleContext) == 256, "MumbleContext is expected to be 256 bytes long.");

// Sanity check our various datatypes on compile.
static_assert(sizeof(float) == 4, "float is expected to be 32 bits long.");
static_assert(sizeof(uint8_t) == 1, "uint8_t is expected to be 8 bits long.");
static_assert(sizeof(uint16_t) == 2, "uint16_t is expected to be 16 bits long.");
static_assert(sizeof(uint32_t) == 4, "uint32_t is expected to be 32 bits long.");
static_assert(sizeof(wchar_t) == 2, "wchar_t is expected to be 16 bits long.");

#endif  // BURRITO_LINK_LINKED_MEMORY_H_
