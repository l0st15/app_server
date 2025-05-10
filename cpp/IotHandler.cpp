//
// Created by Даниил on 10.05.2025.
//

#include "IotHandler.h"
#include "iostream"

Response IotHandler::RequestProcesssing(const Request &req) {
    if (req.path == "/iot/power")
        return controlPower(req);
    else if (req.path == "/iot/lamp")
        return controlLamp(req);
    else if (req.path == "/iot/sendTemp")
        return polling(req);
    else
        return Response(405, "Method Not Allowed");
}

Response IotHandler::controlLamp(const Request &req) {

    auto json = nlohmann::json::parse(req.body);
    std::string uuid = json["token"];
    if(!verifyUser(uuid))
        return Response(405, "Method Not Allowed");

    std::string command = json["command"];
    // TODO проверка на валидность команды
    int iot_id = std::stoi(std::string(json["iot_id"]));
    std::string sql_query = "INSERT INTO iot_commands(iot_id, command) VALUES(?, ?);";
    dBmanager.openDB();
    dBmanager.execQuery(sql_query, iot_id, command);
    dBmanager.closeDB();
    return Response();

}

Response IotHandler::polling(const Request &req) {
    return Response(405, "Method Not Allowed");
}

Response IotHandler::controlPower(const Request &req) {

    auto json = nlohmann::json::parse(req.body);
    std::string uuid = json["token"];
    if(!verifyUser(uuid))
        return Response(405, "Method Not Allowed");

    std::string command = json["command"];
    // TODO проверка на валидность команды
    int iot_id = std::stoi(std::string(json["iot_id"]));
    std::string sql_query = "INSERT INTO iot_commands(iot_id, command) VALUES(?, ?);";
    dBmanager.openDB();
    dBmanager.execQuery(sql_query, iot_id, command);
    dBmanager.closeDB();
    return Response();
}

bool IotHandler::verifyUser(const std::string& uuid) {

    std::string sql_query = "SELECT user_id FROM user_ token WHERE token = ?;";
    int user_id = 0;
    dBmanager.openDB();
    dBmanager.execQuery(sql_query, uuid);
    dBmanager.getDataDB(1, user_id);
    dBmanager.closeDB();
    if(user_id == 0)
        return false;
    return true;
}
