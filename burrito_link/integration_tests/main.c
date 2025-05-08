#include <errhandlingapi.h>
#include <libloaderapi.h>
#include <stdint.h>
#include <stdio.h>
#include <stringapiset.h>
#include <synchapi.h>
#include <time.h>
#include <winerror.h>
#include <winsock2.h>

#include "../linked_memory.h"
#include "../serializer.h"
#include "data.h"

#define PORT 4242
#define BUFFER_SIZE 1024

#define bool int
#define false 0
#define true 1

int get_message(
    SOCKET server_socket,
    struct sockaddr_in* client_address,
    char* buffer
) {
    int client_address_length = sizeof(struct sockaddr_in);

    int receive_length = recvfrom(
        server_socket,
        buffer,
        BUFFER_SIZE,
        0,
        (struct sockaddr*)&client_address,
        &client_address_length
    );

    if (receive_length == SOCKET_ERROR) {
        int receive_error = WSAGetLastError();
        if (receive_error == WSAEWOULDBLOCK) {
            return -2;
        }
        printf("recvfrom failed: %d\n", receive_error);
        return -1;
    }

    return receive_length;
}

int clear_all_messages(
    SOCKET server_socket,
    struct sockaddr_in* client_address,
    char* buffer
) {
    int count = 0;
    int size;
    do {
        size = get_message(
            server_socket,
            client_address,
            buffer
        );
        if (size > 0) {
            printf("---cleared a message %i\n", buffer[0]);
            if (buffer[0] == 0x01) {
                ++count;
            }
        }
    } while (size > 0);
    return count;
}

int udp_listen(SOCKET* server_socket) {
    // Windows Socket API Data
    WSADATA win_sock_api_data;

    struct sockaddr_in server_address;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &win_sock_api_data) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Create a UDP socket
    *server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (*server_socket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    u_long value = TRUE;
    ioctlsocket(*server_socket, FIONBIO, &value);

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    int bind_result = bind(*server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (bind_result == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(*server_socket);
        WSACleanup();
        return 1;
    }

    printf("UDP server listening on port %d...\n", PORT);
    return 0;
}

int udp_close(SOCKET server_socket) {
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// initMumble
//
// Duplicated from the burrito_link.c It should deduplicated when we split out
// functions from burrito_link.c into other files.
// We dont want to do that work first yet because this integration test harness
// is also doing performance testing.
////////////////////////////////////////////////////////////////////////////////
HANDLE handle_lm;
LPCTSTR mapped_lm;
struct LinkedMem* lm = NULL;
struct MumbleContext* lc = NULL;

void initMumble() {
    size_t BUF_SIZE = sizeof(struct LinkedMem);

    handle_lm = CreateFileMapping(
        INVALID_HANDLE_VALUE,  // use paging file
        NULL,  // default security
        PAGE_READWRITE,  // read/write access
        0,  // maximum object size (high-order DWORD)
        BUF_SIZE,  // maximum object size (low-order DWORD)
        "MumbleLink"  // name of mapping object
    );

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
        BUF_SIZE
    );

    if (mapped_lm == NULL) {
        printf("Could not map view of file (%lu).\n", GetLastError());

        CloseHandle(handle_lm);

        return;
    }
    lm = (struct LinkedMem*)mapped_lm;

    lc = (struct MumbleContext*)lm->context;
    printf("successfully opened mumble link shared memory..\n");
}

void closeMumble() {
    // unmap the shared memory from our process address space.
    UnmapViewOfFile(mapped_lm);
    // close LinkedMemory handle
    CloseHandle(handle_lm);
}

typedef int (*MyFunctionType)(int, int);  // Example function signature: int MyFunction(int, int)

int main() {
    bool all_tests_passed = true;
    build_testcases();

    SOCKET server_socket;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_address;

    // Create a UDP Server
    udp_listen(&server_socket);

    // Create a shared memory buffer
    initMumble();

    lm->uiTick = 0;
    lm->fCameraPosition[0] = 1;
    lm->fCameraPosition[1] = 2;
    lm->fCameraPosition[2] = 3;

    lm->fCameraFront[0] = 1.11;
    lm->fCameraFront[1] = 2.22;
    lm->fCameraFront[2] = 3.33;

    lm->fAvatarPosition[0] = 32.0;
    lm->fAvatarPosition[1] = 32.0;
    lm->fAvatarPosition[2] = 32.0;

    lc->compassWidth = 12;
    lc->compassHeight = 300;

    lc->playerX = 5.0;
    lc->playerY = 5.0;
    lc->mapCenterX = 128.0;
    lc->mapCenterY = 129.0;
    lc->mapScale = 2.5;
    lc->compassRotation = 130.0;

    lc->uiState = 0x00000000;

    lc->mapId = 12345;

    char* identity = "{\n  \"name\": \"Irwene\",\n  \"profession\": 4,\n  \"spec\": 55,\n  \"race\": 4,\n  \"map_id\": 50,\n  \"world_id\": 268435505,\n  \"team_color_id\": 0,\n  \"commander\": false,\n  \"fov\": 0.873,\n  \"uisz\": 1\n}";
    // char* identity = "randomidentity";
    MultiByteToWideChar(
        CP_UTF8,  // CodePage,
        0,  // dwFlags,
        identity,  // lpMultiByteStr,
        -1,  // cbMultiByte,
        lm->identity,  // lpWideCharStr,
        256  // cchWideChar
    );

    lc->mapId = 12345;

    // Load the burrito link dll
    HINSTANCE burrito_link_dll = LoadLibrary("burrito_link.dll");
    if (burrito_link_dll == NULL) {
        printf("Failed to load burrito link DLL. %lu\n", GetLastError());
        return 1;
    }

    MyFunctionType MyFunction = (MyFunctionType)GetProcAddress(burrito_link_dll, "GetAddonDef");
    if (MyFunction == NULL) {
        printf("Could not find the function. Error code: %lu\n", GetLastError());
        FreeLibrary(burrito_link_dll);
        return 1;
    }

    // Launch the burrito_link using the thread function
    int buffer_length;

    for (int i = 1001; i < 2000; i++) {
        lm->uiTick = i;
        buffer_length = get_message(server_socket, &client_address, buffer);
        if (buffer_length > 0) {
            break;
        }
        Sleep(100);
    }

    printf("Got first message %i\n", buffer[0]);
    Sleep(2000);
    int frame_message_count = 1;

    do {
        buffer_length = get_message(
            server_socket,
            &client_address,
            buffer
        );
        if (buffer_length > 0) {
            printf("Skipping message of type %i\n", buffer[0]);
            if (buffer[0] == 0x01) {
                ++frame_message_count;
            }
        }
    } while (buffer_length > 0);

    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;

    // Repeat
    for (; frame_message_count <= 1004; frame_message_count++) {
        // printf("Starting with %i\n", frame_message_count);

        bool per_frame_message_expected = true;
        bool metadata_message_expected = false;
        if (frame_message_count % 501 == 0) {
            metadata_message_expected = true;
        }

        // Update shared memory
        QueryPerformanceFrequency(&frequency);

        lm->fCameraPosition[0] = frame_message_count;

        lm->uiTick = frame_message_count;
        QueryPerformanceCounter(&start);
        // Wait for a response over the network (with a timeout?)
        do {
            buffer_length = get_message(server_socket, &client_address, buffer);
        } while (buffer_length == -2);
        QueryPerformanceCounter(&end);

        while (buffer_length > 0) {
            // Sleep for a moment to allow network packets to fully send.
            Sleep(10);
            switch (buffer[0]) {
                case 0x01: {
                    struct BurritoFrameData* burrito_frame = (struct BurritoFrameData*)buffer;
                    // printf("!!FrameNumber: %f %i\n", burrito_frame->camera_position[0], frame_message_count);

                    if (!per_frame_message_expected) {
                        printf("Got an unexpected frame message\n");
                        all_tests_passed = false;
                    }
                    per_frame_message_expected = false;

                    size_t frame_message_index = burrito_frame->camera_position[0];
                    if (frame_message_index != frame_message_count) {
                        printf("Mismatched expected frame message and actual frame message %lli %i", frame_message_index, frame_message_count);
                        all_tests_passed = false;
                    }

                    struct SizedBuffer test_buffer = get_testcase(frame_message_count);
                    int same_buffers = compare((uint8_t*)buffer, buffer_length, test_buffer.buffer, test_buffer.size);
                    if (!same_buffers) {
                        printf("Expected per-frame message not the same as actual per-frame message %i\n", frame_message_count);
                        all_tests_passed = false;
                    }
                    printf("%i\r", frame_message_count);
                    break;
                }
                case 0x02: {
                    if (!metadata_message_expected) {
                        printf("Got an unexpected metadata message\n");
                        all_tests_passed = false;
                    }
                    metadata_message_expected = false;

                    mask_bytes(9, 4, (uint8_t*)buffer);

                    int same_buffers = compare((uint8_t*)buffer, buffer_length, test_case_heavy.buffer, test_case_heavy.size);
                    if (!same_buffers) {
                        printf("Expected Heavy message no the same as actual heavy message\n");
                        all_tests_passed = false;
                    }

                    break;
                }
                default:
                    printf("Unknown messagetype %i\n", buffer[0]);
                    all_tests_passed = false;
            }

            buffer_length = get_message(server_socket, &client_address, buffer);
        }

        double elapsed_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart * 1e3;
        // printf("%.12fms\n", elapsed_time);
        // printf("Buffer Length %i\n", buffer_length);

        if (per_frame_message_expected) {
            printf("Did not get an expected per-frame message");
            all_tests_passed = false;
        }
        if (metadata_message_expected) {
            printf("Did not get an expected metadata message");
            all_tests_passed = false;
        }

        clear_all_messages(server_socket, &client_address, buffer);
    }
    printf("\n");

    // Unload the burrito link dll
    FreeLibrary(burrito_link_dll);

    // Close the UDP socket
    udp_close(server_socket);

    return !all_tests_passed;
}
