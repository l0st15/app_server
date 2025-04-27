#ifndef APP_SERVER_NETWORK_H
#define APP_SERVER_NETWORK_H
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <vector>
#include <iostream>
#include <string>
#include "Request_and_Respore.h"
class Network
{
public:
    Network(std::string ip, short port);
    ~Network();
    int recieveRequest(Request &req);
    int sendResponse(Response &res);
protected:
    void closeAndClear(SOCKET sock);
    void closeAndClear(std::vector<SOCKET> socks);
    void closeAndClear();
private:
    sockaddr_in addr{};
    SOCKET srvSock;
    SOCKET clientSock;
    WSADATA sdata;
    sockaddr_in clientInfo{};
};
#endif