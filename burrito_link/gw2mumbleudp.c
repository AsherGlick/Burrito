#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <winsock2.h>
#include <windows.h>

// Enumerations of the different packet types that can be sent
#define PACKET_FRAME 1
#define PACKET_METADATA 2
#define PACKET_LINK_TIMEOUT 3

// Do a rolling average on the player position because that seems to be
// Roughly what the camera position is doing and doing this will remove
// some weird jitter I hope
struct rolling_average_5 {
    UINT8 index;
    float points[5];
};

float get_rolling_average(struct rolling_average_5 *points) {
    float sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += points->points[i];
    }
    return sum / 5.0;
}

void replace_point_in_rolling_average(struct rolling_average_5 *points, float newvalue) {
    points->points[points->index] = newvalue;
    points->index = points->index + 1;
    if (points->index > 4) {
        points->index = 0;
    }
}
struct rolling_average_5 playerx_avg;
struct rolling_average_5 playery_avg;
struct rolling_average_5 playerz_avg;

float fAvatarAveragePosition[3];

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

// global variables
// mumble link pointer
struct LinkedMem *lm = NULL;
// mumble context pointer into the `lm` variable above.
struct MumbleContext *lc = NULL;

long program_timeout = 0;
long program_startime = 0;

time_t rawtime;

#ifdef _WIN32

// handle to the shared memory of Mumble link . close at the end of program. windows will only release the shared memory once ALL handles are closed,
// so we don't have to worry about other processes like arcdps or other overlays if they are using this.
HANDLE handle_lm;
// the pointer to the mapped view of the file. close before handle.
LPCTSTR mapped_lm;
#endif

void initMumble() {
#ifdef _WIN32
    // creates a shared memory IF it doesn't exist. otherwise, it returns the existing shared memory handle.
    // reference: https://docs.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory

    size_t BUF_SIZE = sizeof(struct LinkedMem);

    handle_lm = CreateFileMapping(
        INVALID_HANDLE_VALUE,  // use paging file
        NULL,  // default security
        PAGE_READWRITE,  // read/write access
        0,  // maximum object size (high-order DWORD)
        BUF_SIZE,  // maximum object size (low-order DWORD)
        "MumbleLink");  // name of mapping object

    // CreateFileMapping returns NULL when it fails, we print the error code for debugging purposes.
    if (handle_lm == NULL) {
        printf("Could not create file mapping object (%lu).\n", GetLastError());
        return;
    }

    mapped_lm = (LPTSTR)MapViewOfFile(
        handle_lm,  // handle to map object
        FILE_MAP_ALL_ACCESS,  // read/write permission
        0,
        0,
        BUF_SIZE);

    if (mapped_lm == NULL) {
        printf("Could not map view of file (%lu).\n", GetLastError());

        CloseHandle(handle_lm);

        return;
    }
    lm = (struct LinkedMem *)mapped_lm;

    lc = (struct MumbleContext *)lm->context;
    printf("successfully opened mumble link shared memory..\n");
#else
    char memname[256];
    snprintf(memname, sizeof(memname), "/MumbleLink.%d", getuid());

    int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmfd < 0) {
        return;
    }

    lm = (struct LinkedMem *)(mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));

    if (lm == (void *)(-1)) {
        lm = NULL;
        return;
    }
#endif
}

int last_map_id = 0;

// The max buffer size for data that is being sent to burriot over the UDP socket
#define MaxBufferSize 1024

////////////////////////////////////////////////////////////////////////////////
// connect_and_or_send()
//
// This function loops until termination, grabbing information from the shared
// memory block and sending the memory over to burrito over a UDP socket.
////////////////////////////////////////////////////////////////////////////////
int connect_and_or_send() {
    WSADATA wsaData;
    SOCKET SendingSocket;
    SOCKADDR_IN ReceiverAddr;
    int Port = 4242;
    int BufLength = 1024;
    char SendBuf[MaxBufferSize];
    int TotalByteSent;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Client: WSAStartup failed with error %d\n", WSAGetLastError());

        // Clean up
        WSACleanup();

        // Exit with error
        return -1;
    }
    else {
        printf("Client: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);
    }
    // Create a new socket to receive datagrams on.

    SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (SendingSocket == INVALID_SOCKET) {
        // Print error message
        printf("Client: Error at socket(): %d\n", WSAGetLastError());

        // Clean up
        WSACleanup();

        // Exit with error
        return -1;
    }
    else {
        printf("Client: socket() is OK!\n");
    }

    /*Set up a SOCKADDR_IN structure that will identify who we
     will send datagrams to.
     For demonstration purposes, let's assume our receiver's IP address is 127.0.0.1
     and waiting for datagrams on port 5150.*/

    ReceiverAddr.sin_family = AF_INET;

    ReceiverAddr.sin_port = htons(Port);

    ReceiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int count = 0;
    DWORD lastuitick = 0;
    // Send data packages to the receiver(Server).
    do {
        if (program_timeout != 0 && clock() - program_startime > program_timeout) {
            BufLength = 1;
            // Set the first byte of the packet to indicate this packet is a `Heaver Context Updater` packet
            SendBuf[0] = PACKET_LINK_TIMEOUT;
            TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));
            if (TotalByteSent != BufLength) {
                printf("Not all Bytes Sent");
            }

            printf("Breaking out due to timeout");
            break;
        }
        if (lm->uiTick == lastuitick) {
            Sleep(1);
            continue;
        }
        lastuitick = lm->uiTick;

        replace_point_in_rolling_average(&playerx_avg, lm->fAvatarPosition[0]);
        replace_point_in_rolling_average(&playery_avg, lm->fAvatarPosition[1]);
        replace_point_in_rolling_average(&playerz_avg, lm->fAvatarPosition[2]);

        // Get the new rolling average values
        fAvatarAveragePosition[0] = get_rolling_average(&playerx_avg);
        fAvatarAveragePosition[1] = get_rolling_average(&playery_avg);
        fAvatarAveragePosition[2] = get_rolling_average(&playerz_avg);

        BufLength = 1;
        // Set the first byte of the packet to indicate this packet is a `Per Frame Updater` packet
        SendBuf[0] = PACKET_FRAME;

        memcpy(SendBuf + BufLength, lm->fCameraPosition, sizeof(lm->fCameraPosition));
        BufLength += sizeof(lm->fCameraPosition);

        memcpy(SendBuf + BufLength, lm->fCameraFront, sizeof(lm->fCameraFront));
        BufLength += sizeof(lm->fCameraFront);

        memcpy(SendBuf + BufLength, fAvatarAveragePosition, sizeof(fAvatarAveragePosition));
        BufLength += sizeof(fAvatarAveragePosition);

        // memcpy(SendBuf+BufLength, lm->fAvatarPosition, sizeof(lm->fAvatarPosition));
        // BufLength += sizeof(lm->fAvatarPosition);

        float map_offset_x = lc->playerX - lc->mapCenterX;
        memcpy(SendBuf + BufLength, &map_offset_x, sizeof(map_offset_x));
        BufLength += sizeof(map_offset_x);

        float map_offset_y = lc->playerY - lc->mapCenterY;
        memcpy(SendBuf + BufLength, &map_offset_y, sizeof(map_offset_y));
        BufLength += sizeof(map_offset_y);

        memcpy(SendBuf + BufLength, &lc->mapScale, sizeof(lc->mapScale));
        BufLength += sizeof(lc->mapScale);

        memcpy(SendBuf + BufLength, &lc->compassRotation, sizeof(lc->compassRotation));
        BufLength += sizeof(lc->compassRotation);

        memcpy(SendBuf + BufLength, &lc->uiState, sizeof(lc->uiState));
        BufLength += sizeof(lc->uiState);

        // UINT32

        // printf("map_offset_x: %f\n", lc->playerX - lc->mapCenterX);
        // printf("map_offset_y: %f\n", lc->playerY - lc->mapCenterY);
        // printf("mapScale: %f\n", lc->mapScale);
        // printf("compassRotation: %f\n", lc->compassRotation); // radians
        // printf("UI State: %i\n", lc->uiState); // Bitmask: Bit 1 = IsMapOpen, Bit 2 = IsCompassTopRight, Bit 3 = DoesCompassHaveRotationEnabled, Bit 4 = Game has focus, Bit 5 = Is in Competitive game mode, Bit 6 = Textbox has focus, Bit 7 = Is in Combat

        TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));
        if (TotalByteSent != BufLength) {
            printf("Not all Bytes Sent");
        }

        // After so many iterations have passed or under specific conditions
        // we will send a larger packet that contains more information about
        // the current state of the game.
        if (count == 0 || lc->mapId != last_map_id) {
            last_map_id = lc->mapId;
            BufLength = 1;
            // Set the first byte of the packet to indicate this packet is a `Heaver Context Updater` packet
            SendBuf[0] = PACKET_METADATA;

            // printf("hello world\n");
            // printf("%ls\n", lm->description);

            printf("%ls\n", lm->identity);

            // printf("%i\n", lc->mapId);
            // printf("\n", lc->serverAddress); // contains sockaddr_in or sockaddr_in6
            // printf("Map Id: %i\n", lc->mapId);
            // printf("Map Type: %i\n", lc->mapType);
            // printf("shardId: %i\n", lc->shardId);
            // printf("instance: %i\n", lc->instance);
            // printf("buildId: %i\n", lc->buildId);
            // printf("UI State: %i\n", lc->uiState); // Bitmask: Bit 1 = IsMapOpen, Bit 2 = IsCompassTopRight, Bit 3 = DoesCompassHaveRotationEnabled, Bit 4 = Game has focus, Bit 5 = Is in Competitive game mode, Bit 6 = Textbox has focus, Bit 7 = Is in Combat
            // printf("compassWidth %i\n", lc->compassWidth); // pixels
            // printf("compassHeight %i\n", lc->compassHeight); // pixels
            // printf("compassRotation: %f\n", lc->compassRotation); // radians
            // printf("playerX: %f\n", lc->playerX); // continentCoords
            // printf("playerY: %f\n", lc->playerY); // continentCoords
            // printf("mapCenterX: %f\n", lc->mapCenterX); // continentCoords
            // printf("mapCenterY: %f\n", lc->mapCenterY); // continentCoords
            // printf("mapScale: %f\n", lc->mapScale);
            // printf("\n", UINT32 processId;
            // printf("mountIndex: %i\n", lc->mountIndex);

            // New things for the normal packet

            // Things for the context packet
            // printf("compassWidth %i\n", lc->compassWidth); // pixels
            // printf("compassHeight %i\n", lc->compassHeight); // pixels
            // printf("Map Id: %i\n", lc->mapId);
            // printf("%ls\n", lm->identity);

            memcpy(SendBuf + BufLength, &lc->compassWidth, sizeof(lc->compassWidth));
            BufLength += sizeof(lc->compassWidth);

            memcpy(SendBuf + BufLength, &lc->compassHeight, sizeof(lc->compassHeight));
            BufLength += sizeof(lc->compassHeight);

            memcpy(SendBuf + BufLength, &lc->mapId, sizeof(lc->mapId));
            BufLength += sizeof(lc->mapId);

            // Get and send the linux x server window id
            UINT32 x11_window_id = 0;
            HWND window_handle = NULL;
            BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam) {
                DWORD processId;
                GetWindowThreadProcessId(hwnd, &processId);
                if (processId == lParam) {
                    window_handle = hwnd;
                    return FALSE;
                }
                return TRUE;
            }
            EnumWindows(EnumWindowsProcMy, lc->processId);

            HANDLE possible_x11_window_id = GetProp(window_handle, "__wine_x11_whole_window");
            if (possible_x11_window_id != NULL) {
                x11_window_id = (size_t)possible_x11_window_id;
            }
            // else {
            //     printf("No Linux ID\n");
            // }

            memcpy(SendBuf + BufLength, &x11_window_id, sizeof(x11_window_id));
            BufLength += sizeof(x11_window_id);

            // Convert and send the JSON 'identity' payload
            char utf8str[1024];

            UINT32 converted_size = WideCharToMultiByte(
                CP_UTF8,
                0,
                lm->identity,
                -1,
                utf8str,
                1024,
                NULL,
                NULL);

            // printf("UTF8 Length: %i\n", converted_size);
            // printf("%s\n", utf8str);

            // UINT16 identity_size = wcslen(lm->identity);
            memcpy(SendBuf + BufLength, &converted_size, sizeof(converted_size));
            BufLength += sizeof(converted_size);

            memcpy(SendBuf + BufLength, utf8str, converted_size);
            BufLength += converted_size;

            TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));
            if (TotalByteSent != BufLength) {
                printf("Not all Bytes Sent");
            }
            // break;
        }

        // Update the count for the `Heaver Context Updater` packet and reset
        // it to 0 when it hits a threshold value.
        count += 1;
        if (count > 500) {
            count = 0;
        }
    } while (TRUE);

    if (closesocket(SendingSocket) != 0) {
        printf("Client: closesocket() failed! Error code: %d\n", WSAGetLastError());
    }
    else {
        printf("Server: closesocket() is OK\n");
    }

    // When your application is finished call WSACleanup.

    printf("Client: Cleaning up...\n");

    if (WSACleanup() != 0) {
        printf("Client: WSACleanup() failed! Error code: %d\n", WSAGetLastError());
    }
    else {
        printf("Client: WSACleanup() is OK\n");
    }

#ifdef _WIN32
    // unmap the shared memory from our process address space.
    UnmapViewOfFile(mapped_lm);
    // close LinkedMemory handle
    CloseHandle(handle_lm);
#endif

    // Back to the system
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// The main function initializes some global variables and shared memory. Then
// calls the connect_and_or_send process which loops until termination.
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        // If a timeout flag is passed in then set the timeout value
        if (strcmp(argv[i], "--timeout") == 0) {
            i = i + 1;
            program_timeout = atol(argv[i]) * CLOCKS_PER_SEC;
            program_startime = clock();
        }
    }

    playerx_avg.index = 0;
    playery_avg.index = 0;
    playerz_avg.index = 0;

    initMumble();

    connect_and_or_send();
}


// Sanity check our memory map structs on compile.
static_assert(sizeof(struct LinkedMem) == 5460, "LinkedMem is expected to be 5460 bytes long.");
static_assert(sizeof(struct MumbleContext) == 256, "MumbleContext is expected to be 256 bytes long.");

// Sanity check our various datatypes on compile.
static_assert(sizeof(float) == 4, "float is expected to be 32 bits long.");
static_assert(sizeof(uint8_t) == 1, "uint8_t is expected to be 8 bits long.");
static_assert(sizeof(uint16_t) == 2, "uint16_t is expected to be 16 bits long.");
static_assert(sizeof(uint32_t) == 4, "uint32_t is expected to be 32 bits long.");
static_assert(sizeof(wchar_t) == 2, "wchar_t is expected to be 16 bits long.");
