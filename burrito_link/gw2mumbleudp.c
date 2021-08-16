#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>

// Do a rolling average on the player position because that seems to be
// Roughly what the camera position is doing and doing this will remove
// some weird jitter I hope
struct rolling_average_5
{
    UINT8 index;
    float points[5];
};
float get_rolling_average(struct rolling_average_5 *points)
{
    float sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += points->points[i];
    }
    return sum / 5.0;
}
void replace_point_in_rolling_average(struct rolling_average_5 *points, float newvalue)
{
    points->points[points->index] = newvalue;
    points->index = points->index + 1;
    if (points->index > 4)
    {
        points->index = 0;
    }
}
struct rolling_average_5 playerx_avg;
struct rolling_average_5 playery_avg;
struct rolling_average_5 playerz_avg;

float fAvatarAveragePosition[3];

// https://wiki.guildwars2.com/wiki/API:MumbleLink
struct LinkedMem
{
    UINT32 uiVersion;
    DWORD uiTick;
    float fAvatarPosition[3]; // The XYZ location of the player
    float fAvatarFront[3];
    float fAvatarTop[3];
    wchar_t name[256];          // The string "Guild Wars 2" [Ignored]
    float fCameraPosition[3];   // The XYZ position of the camera
    float fCameraFront[3];      // A unit vector extending out the front of the camera
    float fCameraTop[3];        // A perpendicular vector to fCameraFront, used for calculating roll [Ignored]
    wchar_t identity[256];      // A json string containing json data
    UINT32 context_len;         // A value that is always 48 [Ignored]
    unsigned char context[256]; // See MumbleContext struct
    wchar_t description[2048];  // Empty [Ignored]
};

struct MumbleContext
{
    unsigned char serverAddress[28]; // contains sockaddr_in or sockaddr_in6 // IGNORED
    UINT32 mapId;
    UINT32 mapType;
    UINT32 shardId;
    UINT32 instance;
    UINT32 buildId;
    // Additional data beyond the 48 bytes Mumble uses for identification
    UINT32 uiState;        // Bitmask: Bit 1 = IsMapOpen, Bit 2 = IsCompassTopRight, Bit 3 = DoesCompassHaveRotationEnabled, Bit 4 = Game has focus, Bit 5 = Is in Competitive game mode, Bit 6 = Textbox has focus, Bit 7 = Is in Combat
    UINT16 compassWidth;   // pixels
    UINT16 compassHeight;  // pixels
    float compassRotation; // radians
    float playerX;         // continentCoords
    float playerY;         // continentCoords
    float mapCenterX;      // continentCoords
    float mapCenterY;      // continentCoords
    float mapScale;
    UINT32 processId;
    UINT8 mountIndex;
};

// global variables
// mumble link pointer
struct LinkedMem *lm = NULL;
// mumble context pointer into the `lm` variable above.
struct MumbleContext *lc = NULL;
#ifdef _WIN32

// handle to the shared memory of Mumble link . close at the end of program. windows will only release the shared memory once ALL handles are closed,
// so we don't have to worry about other processes like arcdps or other overlays if they are using this.
HANDLE handle_lm;
// the pointer to the mapped view of the file. close before handle.
LPCTSTR mapped_lm;
#endif

void initMumble()
{

#ifdef _WIN32
    // creates a shared memory IF it doesn't exist. otherwise, it returns the existing shared memory handle.
    // reference: https://docs.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory

    size_t BUF_SIZE = sizeof(struct LinkedMem);

    handle_lm = CreateFileMapping(
        INVALID_HANDLE_VALUE, // use paging file
        NULL,                 // default security
        PAGE_READWRITE,       // read/write access
        0,                    // maximum object size (high-order DWORD)
        BUF_SIZE,             // maximum object size (low-order DWORD)
        "MumbleLink");        // name of mapping object
                              // createfilemapping returns NULL when it fails, we print the error code for debugging purposes.

    if (handle_lm == NULL)
    {
        printf("Could not create file mapping object (%lu).\n",
               GetLastError());
        return;
    }

    mapped_lm = (LPTSTR)MapViewOfFile(handle_lm,           // handle to map object
                                      FILE_MAP_ALL_ACCESS, // read/write permission
                                      0,
                                      0,
                                      BUF_SIZE);

    if (mapped_lm == NULL)
    {
        printf("Could not map view of file (%lu).\n",
               GetLastError());

        CloseHandle(handle_lm);

        return;
    }
    lm = (struct LinkedMem *)mapped_lm;

    lc = (struct MumbleContext *)lm->context;
    printf("successfully opened mumble link shared memory..\n");
#else
    char memname[256];
    snprintf(memname, 256, "/MumbleLink.%d", getuid());

    int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmfd < 0)
    {
        return;
    }

    lm = (struct LinkedMem *)(mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));

    if (lm == (void *)(-1))
    {
        lm = NULL;
        return;
    }
#endif
}

int last_map_id = 0;

#define MaxBufferSize 1024
int connect_and_or_send()
{
    WSADATA wsaData;
    SOCKET SendingSocket;
    SOCKADDR_IN ReceiverAddr, SrcInfo;
    int Port = 4242;
    int BufLength = 1024;
    char SendBuf[MaxBufferSize];
    int len;
    int TotalByteSent;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {

        printf("Client: WSAStartup failed with error %d\n", WSAGetLastError());

        // Clean up
        WSACleanup();

        // Exit with error
        return -1;
    }
    else
    {
        printf("Client: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);
    }
    // Create a new socket to receive datagrams on.

    SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (SendingSocket == INVALID_SOCKET)
    {

        // Print error message
        printf("Client: Error at socket(): %d\n", WSAGetLastError());

        // Clean up
        WSACleanup();

        // Exit with error
        return -1;
    }
    else
    {
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
    do
    {

        if (lm->uiTick == lastuitick)
        {
            Sleep(1);
            continue;
        }
        lastuitick = lm->uiTick;
        //printf("%ld\n", lm->uiTick);

        replace_point_in_rolling_average(&playerx_avg, lm->fAvatarPosition[0]);
        replace_point_in_rolling_average(&playery_avg, lm->fAvatarPosition[1]);
        replace_point_in_rolling_average(&playerz_avg, lm->fAvatarPosition[2]);

        // Get the new rolling average values
        fAvatarAveragePosition[0] = get_rolling_average(&playerx_avg);
        fAvatarAveragePosition[1] = get_rolling_average(&playery_avg);
        fAvatarAveragePosition[2] = get_rolling_average(&playerz_avg);

        BufLength = 1;
        SendBuf[0] = 1; // Per Frame Updater

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

        if (count == 0 || lc->mapId != last_map_id)
        {
            last_map_id = lc->mapId;
            BufLength = 1;
            SendBuf[0] = 2; // Heaver Context Updater

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

            // break;
        }

        // Sleep(16); // Slightly faster then 60fps which would be 16.6666666...ms

        count += 1;
        if (count > 500)
        {
            count = 0;
        }

        // TODO: Maybe make a way to break out of this loop beyond program termination
    } while (TRUE);

    // Print some info on the receiver(Server) side...

    // Allocate the required resources

    memset(&SrcInfo, 0, sizeof(SrcInfo));

    len = sizeof(SrcInfo);

    getsockname(SendingSocket, (SOCKADDR *)&SrcInfo, &len);

    printf("Client: Sending IP(s) used: %s\n", inet_ntoa(SrcInfo.sin_addr));

    printf("Client: Sending port used: %d\n", htons(SrcInfo.sin_port));

    // Print some info on the sender(Client) side...

    getpeername(SendingSocket, (SOCKADDR *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));

    printf("Client: Receiving IP used: %s\n", inet_ntoa(ReceiverAddr.sin_addr));

    printf("Client: Receiving port used: %d\n", htons(ReceiverAddr.sin_port));

    printf("Client: Total byte sent: %d\n", TotalByteSent);

    // When your application is finished receiving datagrams close the socket.

    printf("Client: Finished sending. Closing the sending socket...\n");

    if (closesocket(SendingSocket) != 0)
    {

        printf("Client: closesocket() failed! Error code: %d\n", WSAGetLastError());
    }
    else
    {
        printf("Server: closesocket() is OK\n");
    }

    // When your application is finished call WSACleanup.

    printf("Client: Cleaning up...\n");

    if (WSACleanup() != 0)
    {
        printf("Client: WSACleanup() failed! Error code: %d\n", WSAGetLastError());
    }

    else
    {
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

int main(int argc, char **argv)
{
    playerx_avg.index = 0;
    playery_avg.index = 0;
    playerz_avg.index = 0;

    printf("hello world\n");
    initMumble();
    // sockmain(argc, argv);
    // initMumble();
    // for (int i = 0; i < 100; i++) {
    //     printf("%f\n", lm->fAvatarPosition[0]);
    //     Sleep(16); // Slightly faster then 60fps which would be 16.6666666...ms
    // }

    connect_and_or_send();
}