//
// Created by Misha on 25.04.2025.
//
#include "network.h"
#include <regex>


Network::Network(std::string ip,short port)
{
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);

    srvSock = INVALID_SOCKET;
    clientSock = INVALID_SOCKET;
}
Network::~Network()
{
    WSACleanup();
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
int Network::recieveRequest(Request &req) {
    if (FAILED(WSAStartup(MAKEWORD(2, 2), &sdata) != 0)) {
        std::cout << WSAGetLastError() << std::endl;
        closeAndClear();
        return -1;
    }

    srvSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srvSock == INVALID_SOCKET) {
        std::cout << WSAGetLastError() << std::endl;
        closeAndClear();
        return -2;
    }

    if (bind(srvSock, (SOCKADDR *) &addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cout << WSAGetLastError() << std::endl;
        closeAndClear(srvSock);
        return -3;
    }

    if (listen(srvSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << WSAGetLastError() << std::endl;
        closeAndClear(srvSock);
        return -4;
    }
    std::vector<char> servBuff(4096), clientBuff(4096);
    short packet_size = 0;
    ZeroMemory(&clientInfo, sizeof(clientInfo));
    int clientInfo_size = sizeof(clientInfo);

    clientSock = accept(srvSock, (sockaddr *) &clientInfo, &clientInfo_size);
    if (clientSock == INVALID_SOCKET) {
        std::cout << WSAGetLastError() << std::endl;
        closeAndClear(std::vector<SOCKET>{srvSock, clientSock});
        return -5;
    }
    packet_size = recv(clientSock, servBuff.data(), servBuff.size(), 0);
    std::string request(servBuff.begin(), servBuff.end());
    //первая строка
    std::regex start_line_regex(R"(^([A-Z]+)\s+([^?# ]*)(?:\?([^ ]*))?\s+(HTTP/\d+\.\d+)\r\n)", std::regex::icase);
    //TODO сделать обработку query params
    //body and TODO content-length
    size_t headers_end = request.find("\r\n\r\n");
    size_t body_start = headers_end + 4;
    if (body_start <= request.size()) {
        req.body = request.substr(body_start);
    }
    std::smatch start_line_match;
    if (!std::regex_search(request, start_line_match, start_line_regex))
        return -6;
    if (false)
        return -8; //Зарезервировано под query params

    req.method = start_line_match[1];
    req.path = start_line_match[2];
    req.http_version = start_line_match[4];
    //TEMPORARY
    closeAndClear(std::vector<SOCKET>{srvSock, clientSock});
    return 1;
}

int Network::sendResponse(Response &res)
{
    std::string http_response;

    send(clientSock, http_response.c_str(), http_response.size(), 0);
}
//TODO sendResponse получает структуру и отправляет ее, затем завершает соединение
//TODO совершить суицид
//TODO ДОБАВИТЬ ПОТОЧНОСТЬ ИЛИ ОТСУТСТВИЕ БЛОКИРОВОК(???????)