#ifndef APP_SERVER_SOCKET_H
#define APP_SERVER_SOCKET_H
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
class Socket
{
public:
    Socket();
    ~Socket();
    static int sockInit(WSAData sdata);
    static void sockClean();
    static SOCKET createSock(int port, sockaddr_in addr);
    static void closeSock(SOCKET s);
        };
#endif