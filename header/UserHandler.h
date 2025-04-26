//
// Created by Даниил on 22.04.2025.
//

#ifndef APP_SERVER_USERHANDLER_H
#define APP_SERVER_USERHANDLER_H
#include "Request_and_Respore.h"
#include "IRequestHandler.h"
#include "sqlite3.h"
#include "json.hpp"
class UserHandler : public IRequestHandler
{
public:
    Response process(const Request& req);
private:
    sqlite3* db = nullptr;
    Response userReg(const Request& req);
    Response userLogin(const Request& req);
    Response userInfo(const Request& req);
    Response userAuth(const Request& req);
    bool openDB();
};
#endif //APP_SERVER_USERHANDLER_H
