//
// Created by Даниил on 22.04.2025.
//

#ifndef APP_SERVER_USERHANDLER_H
#define APP_SERVER_USERHANDLER_H

#include "IRequestHandler.h"
#include "Units.h"
#include "sqlite3.h"
#include "json.hpp"
#include "Crypto.h"
#include "DBManager.h"

class UserHandler : public IRequestHandler
{
public:
    Response RequestProcesssing(const Request& req) override;
private:
    Crypto crypto_module;
    DBManager dbManager;
    Response userReg(const Request& req); // регистрация пользователя
    Response userLogin(const Request& req); // вход в аккаунт
    Response userGetInfo(const Request& req); //получени данных из бд
    Response sendCommand(const Request& req); //управление iot
    Response addIot(const Request& req); // добавление iot
    int userAuth(const std::string& uuid); // проверка токена
};

#endif //APP_SERVER_USERHANDLER_H
