//
// Created by Misha on 25.04.2025.
//
#include "network.h"

Network::Network(std::string ip,short port)
{
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);

    srvSock = INVALID_SOCKET;
    clientSock = INVALID_SOCKET;
}
void Network::closeAndClear(SOCKET sock)
{
    closesocket(sock);
    WSACleanup();
}
void Network::closeAndClear(std::vector<SOCKET> socks)
{
    for (SOCKET sock : socks)
        closesocket(sock);
    WSACleanup();
}
void Network::closeAndClear()
{
    WSACleanup();
}
int Network::recieveRequest()
{
    if (FAILED(WSAStartup(MAKEWORD(2, 2), &sdata) != 0)) {
        std::cout<<WSAGetLastError()<<std::endl;
        closeAndClear();
        return -1;
    }

    srvSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srvSock == INVALID_SOCKET) {
        std::cout<<WSAGetLastError()<<std::endl;
        closeAndClear();
        return -2;
    }

    if (bind(srvSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cout<<WSAGetLastError()<<std::endl;
        closeAndClear(srvSock);
        return -3;
    }

    if (listen(srvSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cout<<WSAGetLastError()<<std::endl;
        closeAndClear(srvSock);
        return -4;
    }
    std::vector <char> servBuff(4096), clientBuff(4096);
    short packet_size = 0;
    ZeroMemory(&clientInfo, sizeof(clientInfo));
    int clientInfo_size = sizeof(clientInfo);

    clientSock = accept(srvSock, (sockaddr*)&clientInfo, &clientInfo_size);
    if (clientSock == INVALID_SOCKET) {
        std::cout << WSAGetLastError() << std::endl;
        std::vector <SOCKET> toClear{srvSock,clientSock};
        closeAndClear(toClear);
        return -5;
    }
    //packet_size = recv(clientSock, servBuff.data(), servBuff.size(), 0);
    //servBuff.data();

    //TODO передавать параметры в структуру, которой еще нету

}

//TODO sendResponse получает структуру и отправляет ее, затем завершает соединение
//TODO совершить суицид
//TODO проверить поддерживается ли соединение между методами
//TODO ДОБАВИТЬ ПОТОЧНОСТЬ ИЛИ ОТСУТСТВИЕ БЛОКИРОВОК(???????)