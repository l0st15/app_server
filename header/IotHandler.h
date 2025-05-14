//
// Created by Даниил on 13.05.2025.
//

#ifndef APP_SERVER_IOTHANDLER_H
#define APP_SERVER_IOTHANDLER_H
#include "IRequestHandler.h"
#include "DBManager.h"
#include "Units.h"
#include "json.hpp"

class IotHandler : public IRequestHandler
{
private:
    DBManager dbManager;

public:
    Response RequestProcessing(const Request& req) override;

private:
    Response polling(const Request& req);
    nlohmann::json getCommands(const int& iot_id);
    int iotAuth(const std::string& iot_mac);
    void sendData(const int& iot_id, const double& temp, const bool& lamp1, const bool& lamp2);
};

#endif //APP_SERVER_IOTHANDLER_H
