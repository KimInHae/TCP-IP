#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    char *addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if (!inet_(addr, &addr_inet.sin_addr));


}