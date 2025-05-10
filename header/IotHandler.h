//
// Created by Даниил on 10.05.2025.
//

#ifndef APP_SERVER_IOTHANDLER_H
#define APP_SERVER_IOTHANDLER_H
#include "IRequestHandler.h"
#include "DBmanager.h"
#include "UserHandler.h"
class IotHandler : public IRequestHandler {

public:
    Response RequestProcesssing(const Request& request) override;
private:
    DBmanager dBmanager;
    UserHandler userHandler;
private:
    Response controlPower(const Request& req);
    Response controlLamp(const Request& req);
    Response polling(const Request& req);
    bool verifyUser(const std::string& uuid);
};

#endif //APP_SERVER_IOTHANDLER_H
