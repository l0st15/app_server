//
// Created by Misha on 25.04.2025.
//
#include "socket.h"

Socket::Socket(){};
Socket::~Socket(){};

int Socket::sockInit(WSADATA sdata) {
    if (FAILED(WSAStartup(MAKEWORD(2, 2), &sdata) != 0)) {
        return WSAGetLastError();
    }
}

void Socket::sockClean() {
    WSACleanup();
}
SOCKET Socket::createSock(int port,sockaddr_in addr) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

void Socket::closeSock(SOCKET s) {
    closesocket(s);
}