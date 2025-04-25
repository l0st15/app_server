#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
class Socket
{
protected:
    Socket();
    ~Socket();
    int sockInit(WSAData sdata);
    void sockClean();
    SOCKET createSock(int port,sockaddr_in addr);
    void closeSock(SOCKET s);
        };
#endif