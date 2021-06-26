#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>

struct LinkedMem {
    UINT32  uiVersion;
    DWORD   uiTick;
    float   fAvatarPosition[3];
    float   fAvatarFront[3];
    float   fAvatarTop[3];
    wchar_t name[256];
    float   fCameraPosition[3];
    float   fCameraFront[3];
    float   fCameraTop[3];
    wchar_t identity[256];
    UINT32  context_len;
    unsigned char context[256];
    wchar_t description[2048];
};

struct LinkedMem *lm = NULL;

void initMumble() {

#ifdef _WIN32
    HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
    if (hMapObject == NULL)
        return;

    lm = (struct LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(struct LinkedMem));
    if (lm == NULL) {
        CloseHandle(hMapObject);
        hMapObject = NULL;
        return;
    }
#else
    char memname[256];
    snprintf(memname, 256, "/MumbleLink.%d", getuid());

    int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmfd < 0) {
        return;
    }

    lm = (struct LinkedMem *)(mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

    if (lm == (void *)(-1)) {
        lm = NULL;
        return;
    }
#endif
}

#define MaxBufferSize 1024
int connect_and_or_send() {
    WSADATA wsaData;
    SOCKET SendingSocket;
    SOCKADDR_IN ReceiverAddr, SrcInfo;
    int Port = 4242;
    int BufLength = 1024;
    char SendBuf[MaxBufferSize];
    int len;
    int TotalByteSent;

     if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0){

          printf("Client: WSAStartup failed with error %d\n", WSAGetLastError());

          // Clean up
          WSACleanup();

          // Exit with error
          return -1;
     }
     else{
          printf("Client: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);
     }
     // Create a new socket to receive datagrams on.

     SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

     if (SendingSocket == INVALID_SOCKET){

          // Print error message
          printf("Client: Error at socket(): %d\n", WSAGetLastError());

          // Clean up
          WSACleanup();

          // Exit with error
          return -1;
     }
     else{
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

    // Send data packages to the receiver(Server).
     do{
      //       printf("\nPlease, type your message: "); //Ask user for message
      //       fgets(SendBuf, sizeof(SendBuf), stdin); //Read user's input

      //       //Print user's input and a progress message
      //       printf("Client: Data to be sent: %s\n", SendBuf);
      //       printf("Client: Sending data...\n");

      //       //Send message to receiver(Server)
      //       TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));
      //       //Print success message
      //       printf("Client: sendto() looks OK!\n");
      //       count += 1;
      // /*Program is asking user for messages and sending the to Server,until you will close it.
      // (You can replace while(1) with a condition to stop asking/sending messages.)*/
            
       // for (int i = 0; i < 100; i++) {
        // printf("%f\n", );
        

        // int ret = snprintf(SendBuf, BufLength, "%f%f%f", lm->fCameraPosition[0], lm->fCameraPosition[1], lm->fCameraPosition[2])

        BufLength = 0;

        memcpy(SendBuf+BufLength, lm->fCameraPosition, sizeof(lm->fCameraPosition));
        BufLength += sizeof(lm->fCameraPosition);

        memcpy(SendBuf+BufLength, lm->fCameraFront, sizeof(lm->fCameraFront));
        BufLength += sizeof(lm->fCameraFront);

        memcpy(SendBuf+BufLength, lm->fAvatarPosition, sizeof(lm->fAvatarPosition));
        BufLength += sizeof(lm->fAvatarPosition);

        TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));


        Sleep(16); // Slightly faster then 60fps which would be 16.6666666...ms
    // }
        //count += 1;


     }while(count < 500);


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

   if (closesocket(SendingSocket) != 0){

        printf("Client: closesocket() failed! Error code: %d\n", WSAGetLastError());
   }
   else{
        printf("Server: closesocket() is OK\n");
   }



   // When your application is finished call WSACleanup.

   printf("Client: Cleaning up...\n");

   if(WSACleanup() != 0){
        printf("Client: WSACleanup() failed! Error code: %d\n", WSAGetLastError());
   }

   else{
        printf("Client: WSACleanup() is OK\n");
   }

   // Back to the system
   return 0;

}




int main(int argc, char** argv) {
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