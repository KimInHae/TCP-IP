#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma warning(disable:4996)

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET hServerSock, hClntSock;
    FILE *fp;
    char buf[BUF_SIZE];
    int readCnt;
    SOCKADDR_IN servAdr, clntAdr;
    int clntAdrSize;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    fp = fopen("file_server_win.cpp", "rb");

    hServerSock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    bind(hServerSock, (SOCKADDR *)&servAdr, sizeof(servAdr));
    listen(hServerSock, 5);

    clntAdrSize = sizeof(clntAdr);
    hClntSock = accept(hServerSock, (SOCKADDR *)&clntAdr, &clntAdrSize);

//     if (fp == NULL) //fopen이 파일을찾아오지못해서 NULL값이 들어갔을 경우
//     {
//         printf("사용자가가져올 파일이름 오픈 불가\n");    //화면에 오류 표시
//         fclose(fp);
//         return 0;
//     }

    while (1)
    {
        readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (readCnt < BUF_SIZE)
        {
            send(hServerSock, (char *)&buf, readCnt, 0);
            break;
        }
        send(hClntSock, (char *)&buf, BUF_SIZE, 0);
    }

    shutdown(hClntSock, SD_SEND);
    recv(hClntSock, (char *)buf, BUF_SIZE, 0);
    printf("Message from client: %s \n", buf);

    fclose(fp);
    closesocket(hClntSock);
    closesocket(hServerSock);
    WSACleanup();

    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}