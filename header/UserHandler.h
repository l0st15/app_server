//
// Created by Даниил on 22.04.2025.
//

#ifndef APP_SERVER_USERHANDLER_H
#define APP_SERVER_USERHANDLER_H
#include "Request_and_Respore.h"
#include "IRequestHandler.h"
#include "sqlite3.h"
#include "json.hpp"
#include "Crypto.h"
class UserHandler : public IRequestHandler
{
public:
    Response RequestProcesssing(const Request& request);
private:
    sqlite3* db = nullptr;
    Crypto crypto_module;
    Response userReg(const Request& req);
    Response userLogin(const Request& req);
    //Response userInfo(const Request& req);
    //Response userAuth(const Request& req);
    std::string getUserHashPassword(const std::string& user_login);
    bool openDB();
    bool closeDB();
};
#endif //APP_SERVER_USERHANDLER_H
