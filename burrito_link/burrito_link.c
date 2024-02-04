#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <winsock2.h>
#include <windows.h>

#include "linked_memory.h"

// Enumerations of the different packet types that can be sent
#define PACKET_FRAME 1
#define PACKET_METADATA 2
#define PACKET_LINK_TIMEOUT 3

// The max buffer size for data that is being sent to burrito over the UDP socket
#define MAX_BUFFER_SIZE 1024

// Number of uiTicks that should pass between when each "heavy" packet is sent.
#define HEAVY_DATA_INTERVAL 500

// Define the port and address we are trying to connect to. The address is a
// loopback address because we are trying to talk to another process running on
// the same machine.
#define COMMUNICATION_PORT 4242
#define COMMUNICATION_IPV4 "127.0.0.1"

// A state variable to keep track of the previous cycle's map_id to determine
// if the map_id has changed in this cycle.
int last_map_id = 0;

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


// Handle to the shared memory. This is kept so that the handle can be closed
// when the program exits because Windows will only release the shared memory
// once all handles are closed.
HANDLE handle_lm;

// The pointer to the mapped view of the file. This should be unmapped before
// `handle_lm` is closed.
LPCTSTR mapped_lm;

// Pointer to the mapped LinkedMem object. Only available after initMumble()
// is called.
struct LinkedMem *lm = NULL;

// Pointer to the mapped MumbleContext object found at `lm->context`. Only
// available after initMumble() is called.
struct MumbleContext *lc = NULL;


// How many "clocks", as defined by CLOCKS_PER_SECOND and by the return value
// of clock(), will elapse before the program should time out. If this is left
// at 0 burrito link will never time out.
long program_timeout = 0;

// What was the value of `clock()` when the program started. This is used with
// program_timeout to determine if the program should time out or continue.
long program_startime = 0;


////////////////////////////////////////////////////////////////////////////////
// initMumble
//
// Creates or reads a shared memory object named "MumbleLink" which has the
// data from Guild Wars 2. We must create the memory object if it does not
// exist because, while Guild Wars 2 will write to it if it exists, it will not
// create it if it does not already exist.
//
// Reference: https://docs.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory
////////////////////////////////////////////////////////////////////////////////
void initMumble() {
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
}


////////////////////////////////////////////////////////////////////////////////
// x11_window_id_from_windows_process_id()
//
// When running a program in wine a property `__wine_x11_whole_window` is set.
// This function attempts to read that property and return it.
////////////////////////////////////////////////////////////////////////////////
uint32_t x11_window_id_from_windows_process_id(uint32_t windows_process_id) {
    // Get and send the Linux x server window id
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
    EnumWindows(EnumWindowsProcMy, windows_process_id);

    HANDLE possible_x11_window_id = GetProp(window_handle, "__wine_x11_whole_window");
    if (possible_x11_window_id != NULL) {
        x11_window_id = (size_t)possible_x11_window_id;
    }
    // else {
    //     printf("No Linux x11 window ID found\n");
    // }
    return x11_window_id;
}

////////////////////////////////////////////////////////////////////////////////
// connect_and_or_send()
//
// This function loops until termination, grabbing information from the shared
// memory block and sending the memory over to burrito over a UDP socket.
////////////////////////////////////////////////////////////////////////////////
int connect_and_or_send() {
    int BufLength = 1024;
    char SendBuf[MAX_BUFFER_SIZE];
    int TotalByteSent;

    // Setup and Initialize Winsock library
    WSADATA wsaData;
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

    // Create a new socket to send datagrams on.
    SOCKET SendingSocket;
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

    // Setup the destination where we want to send the parsed data to.
    SOCKADDR_IN ReceiverAddr;
    ReceiverAddr.sin_family = AF_INET;
    ReceiverAddr.sin_port = htons(COMMUNICATION_PORT);
    ReceiverAddr.sin_addr.s_addr = inet_addr(COMMUNICATION_IPV4);

    int count = 0;
    DWORD last_ui_tick = 0;
    // Send data packages to the receiver(Server).
    do {
        // Trigger the builtin auto close timeout if a timeout was set. Send a
        // message to burrito that the program hit its timeout so that burrito
        // can decide if it should launch a new instance of burrito link or not.
        if (program_timeout != 0 && clock() - program_startime > program_timeout) {
            // Set the first and only byte of the packet to indicate this
            // packet is a `Packet Link Timeout` packet.
            SendBuf[0] = PACKET_LINK_TIMEOUT;
            BufLength = 1;

            TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));
            if (TotalByteSent != BufLength) {
                printf("Not all Bytes Sent");
            }

            printf("Exiting due to timeout being reached.");
            break;
        }

        // If uiTick is the same value as it was the previous loop then Guild
        // Wars 2 has not updated any data. Sleep for 1ms and check again.
        if (lm->uiTick == last_ui_tick) {
            Sleep(1);
            continue;
        }
        last_ui_tick = lm->uiTick;

        // Update the player position to calculate the new rolling average.
        replace_point_in_rolling_average(&playerx_avg, lm->fAvatarPosition[0]);
        replace_point_in_rolling_average(&playery_avg, lm->fAvatarPosition[1]);
        replace_point_in_rolling_average(&playerz_avg, lm->fAvatarPosition[2]);

        // Get the new rolling average values
        fAvatarAveragePosition[0] = get_rolling_average(&playerx_avg);
        fAvatarAveragePosition[1] = get_rolling_average(&playery_avg);
        fAvatarAveragePosition[2] = get_rolling_average(&playerz_avg);

        // Set the first byte of the packet to indicate this packet is a
        // `Per Frame Updater` packet
        SendBuf[0] = PACKET_FRAME;
        BufLength = 1;

        memcpy(SendBuf + BufLength, lm->fCameraPosition, sizeof(lm->fCameraPosition));
        BufLength += sizeof(lm->fCameraPosition);

        memcpy(SendBuf + BufLength, lm->fCameraFront, sizeof(lm->fCameraFront));
        BufLength += sizeof(lm->fCameraFront);

        memcpy(SendBuf + BufLength, fAvatarAveragePosition, sizeof(fAvatarAveragePosition));
        BufLength += sizeof(fAvatarAveragePosition);

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

        // memcpy(SendBuf + BufLength, &lc->mountIndex, sizeof(lc->mountIndex));
        // BufLength += sizeof(lc->mountIndex);

        TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));
        if (TotalByteSent != BufLength) {
            printf("Not all Bytes Sent");
        }

        // After so many iterations have passed or under specific conditions
        // we will send a larger packet that contains more information about
        // the current state of the game.
        if (count == 0 || lc->mapId != last_map_id) {
            last_map_id = lc->mapId;

            // Set the first byte of the packet to indicate this packet is a
            // `Heaver Context Updater` packet.
            SendBuf[0] = PACKET_METADATA;
            BufLength = 1;

            memcpy(SendBuf + BufLength, &lc->compassWidth, sizeof(lc->compassWidth));
            BufLength += sizeof(lc->compassWidth);

            memcpy(SendBuf + BufLength, &lc->compassHeight, sizeof(lc->compassHeight));
            BufLength += sizeof(lc->compassHeight);

            memcpy(SendBuf + BufLength, &lc->mapId, sizeof(lc->mapId));
            BufLength += sizeof(lc->mapId);

            uint32_t x11_window_id = x11_window_id_from_windows_process_id(lc->processId);
            memcpy(SendBuf + BufLength, &x11_window_id, sizeof(x11_window_id));
            BufLength += sizeof(x11_window_id);

            // Convert the JSON 'identity' payload from widechar to utf8
            // encoded char and send.
            char utf8_identity[1024];
            UINT32 utf8_identity_size = WideCharToMultiByte(
                CP_UTF8, // CodePage
                0, // dwFlags
                lm->identity, // lpWideCharStr
                -1, // cchWideChar
                utf8_identity, // lpMultiByteStr
                1024, // cbMultiByte
                NULL, // lpDefaultChar
                NULL); // lpUsedDefaultChar
            memcpy(SendBuf + BufLength, &utf8_identity_size, sizeof(utf8_identity_size));
            BufLength += sizeof(utf8_identity_size);
            memcpy(SendBuf + BufLength, utf8_identity, utf8_identity_size);
            BufLength += utf8_identity_size;

            TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));
            if (TotalByteSent != BufLength) {
                printf("Not all Bytes Sent");
            }
        }

        // Update the count for the `Heaver Context Updater` packet and reset
        // it to 0 when it hits a threshold value.
        count += 1;
        if (count > HEAVY_DATA_INTERVAL) {
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

    // unmap the shared memory from our process address space.
    UnmapViewOfFile(mapped_lm);
    // close LinkedMemory handle
    CloseHandle(handle_lm);

    // Back to the system
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// run_link
//
// The entry point for all burrito link processes. This is called by either the
// command line entry point, or by a dll entry point. It initializes any global
// that and then launches the infinite loop of reading data from shared memory
// and sending the data over the wire to burrito.
////////////////////////////////////////////////////////////////////////////////
void run_link() {
    playerx_avg.index = 0;
    playery_avg.index = 0;
    playerz_avg.index = 0;

    initMumble();

    connect_and_or_send();
}

////////////////////////////////////////////////////////////////////////////////
// main
//
// The entry point to burrito link when called as an executable. Sets up any
// data that needs to be configured for just command line executions, then
// calls run_link() which sets up all other state necessary for burrito link.
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

    run_link();
}
