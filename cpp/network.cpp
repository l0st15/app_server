//
// Created by Misha on 25.04.2025.
//
#include "network.h"
#include <regex>
#include "openssl/ssl.h"


Network::Network(std::string ip,short port)
{
    if (ctx == nullptr) {
        SSL_library_init();
        SSL_load_error_strings();
        ctx = SSL_CTX_new(TLS_server_method());
        SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);
    }

    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);

    srvSock = INVALID_SOCKET;
    clientSock = INVALID_SOCKET;
}
Network::~Network()
{
    WSACleanup();
    if (ssl != nullptr) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
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

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientSock);
    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        closeAndClear(std::vector<SOCKET>{srvSock, clientSock});
        return -10;
    }
    packet_size = SSL_read(ssl, servBuff.data(), servBuff.size());

    std::string request(servBuff.begin(), servBuff.end());
    //первая строка
    std::regex start_line_regex(R"(^([A-Z]+)\s+([^?# ]*)(?:\?([^ ]*))?\s+(HTTP/\d+\.\d+)\r\n)", std::regex::icase);
    //CONTENT LENGTH
    std::regex cl_regex(R"(Content-Length:\s*(\d+))", std::regex::icase);
    std::smatch cl_match;
    std::smatch start_line_match;
    if (!std::regex_search(request, start_line_match, start_line_regex))
        return -6;
    if (!std::regex_search(request, cl_match, cl_regex))
        return -7;
    if (false)
        return -8; //Зарезервировано под query params
        try {
            req.body_length = std::stoi(cl_match[1]);
            req.method = start_line_match[1];
            req.path = start_line_match[2];
            req.http_version = start_line_match[4];
            //body
            size_t headers_end = request.find("\r\n\r\n");
            size_t body_start = headers_end + 4;
            if (body_start <= request.size()) {
                std::string body = request.substr(body_start, req.body_length);
                body.erase(std::remove(body.begin(), body.end(), '\\'), body.end());
                if (body[0]  != '{')
                    body = body.substr(1, body.length() - 2);
                req.body = body;
            }
        } catch (std::exception &e) {
            std::cout << e.what() << "\n";
            return -9;
        }
    //TEMPORARY
    //closeAndClear(std::vector<SOCKET>{srvSock, clientSock});
    return 1;
}

int Network::sendResponse(Response &res)
{
    std::string http_response = "HTTP/1.1 ";
    http_response += std::to_string(res.statusCode) + " ";
    http_response += "" + res.statusMessage + "\r\n";
    http_response += "Content-Type: application/json\r\n";
    http_response += "headerForSasha: U+1F4A9\r\n";
    http_response += "Content-Length: " + std::to_string(res.body.size())+ "\r\n";
    http_response += "Connection: close\r\n\r\n";
    http_response += res.body;

    if (SSL_write(ssl, http_response.c_str(), http_response.size()) <= 0)
    {
        closeAndClear(std::vector<SOCKET>{srvSock, clientSock});
        return WSAGetLastError();
    }
    shutdown(srvSock,SD_BOTH);
    closeAndClear(std::vector<SOCKET>{srvSock, clientSock});
    return 1;
}
//TODO sendResponse получает структуру и отправляет ее, затем завершает соединение
//TODO совершить суицид
//TODO ДОБАВИТЬ ПОТОЧНОСТЬ ИЛИ ОТСУТСТВИЕ БЛОКИРОВОК(???????)