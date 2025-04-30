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
#include "DBmanager.h"
class UserHandler : public IRequestHandler
{
public:
    Response RequestProcesssing(const Request& request);
private:
    Crypto crypto_module;
    DBmanager db_module;

    Response userReg(const Request& req);
    Response userLogin(const Request& req);
    Response userGetInfo(const Request& req);
    int userAuth(const std::string& uuid);
    std::string getUserHashPassword(const std::string& user_login);
};

typedef struct Data_iot {
    double temp;
    int lamp1;
    int lamp2;
    std::string timestamp;
};
#endif //APP_SERVER_USERHANDLER_H
