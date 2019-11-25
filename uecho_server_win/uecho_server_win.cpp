#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET ServSock;
    char message[BUF_SIZE];
    int strLen;
    int ClntAdrSize;
    SOCKADDR_IN  servAdr, ClntAdr;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    ServSock = socket(PF_INET, SOCK_DGRAM, 0);
    if (ServSock == INVALID_SOCKET)
        ErrorHandling("UDP socket() creation error!");

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    if (bind(ServSock, (SOCKADDR *)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("bind() error!");

    while (1)
    {
        ClntAdrSize = sizeof(ClntAdr);
        strLen = recvfrom(ServSock, message, BUF_SIZE, 0, (SOCKADDR *)&ClntAdr, &ClntAdrSize);
        sendto(ServSock, message, strLen, 0, (SOCKADDR *)&ClntAdr, sizeof(ClntAdr));
    }

    closesocket(ServSock);
    WSACleanup();

    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}