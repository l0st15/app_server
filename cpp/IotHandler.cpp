//
// Created by Даниил on 13.05.2025.
//
#include "IotHandler.h"
#include <iostream>

Response IotHandler::RequestProcesssing(const Request &req) {
    if (req.method == "POST")
    {
        if (req.path == "/iot/sendTemp")
            return polling(req);
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}

Response IotHandler::polling(const Request &req) {

    Response res;

    try {

        if(req.body.empty())
            throw std::invalid_argument("Body is empty");

        auto json = nlohmann::json::parse(req.body);
        std::string mac = json["mac"];
        int id = iotAuth(mac);

        data in_data;
        in_data.temp = json["temp"];
        in_data.lamp1 = json["lamp1"];
        in_data.lamp2 = json["lamp2"];

        sendData(id, in_data.temp, in_data.lamp1, in_data.lamp2);

        res.body = getCommands(id).dump();


    } catch (std::exception& e)
    {
        res.statusCode = 400;
        res.statusMessage = e.what();
        std::cerr << e.what() << std::endl;
        return res;
    }

    return res;
}

nlohmann::json IotHandler::getCommands(const int &iot_id) {

    nlohmann::json json;
    json["commands"] = nlohmann::json::array();

    std::string sql_select = "SELECT id, command FROM commands WHERE iot_id = ? AND status = 'not completed'";
    std::string sql_update = "UPDATE commands SET status = 'completed' WHERE id = ?";

    try {
        dbManager.execute("BEGIN TRANSACTION");

        auto cmds = dbManager.query<Command>(sql_select, iot_id);

        for (const auto& it : cmds) {
            json["commands"].push_back(it.cmd);
            dbManager.execute(sql_update, it.id);
        }
        dbManager.execute("COMMIT");
    } catch (const std::exception& e) {
        dbManager.execute("ROLLBACK");
        std::cerr << "Ошибка: " << e.what() << std::endl;
        throw std::runtime_error("DB error");
    }
    return json;
}

int IotHandler::iotAuth(const std::string &iot_mac) {

    std::string sql = "SELECT id FROM iot WHERE mac = ?";
    auto iot_id = dbManager.query<int>(sql, iot_mac);
    if(iot_id.empty())
        throw std::invalid_argument("id not found");

    return iot_id[0];
}

void IotHandler::sendData(const int& iot_id, const double &temp, const bool &lamp1, const bool &lamp2) {

    std::string sql = "INSERT INTO iot_data (iot_id, temp, lamp1, lamp2) VALUES(?, ?, ?, ?)";
    dbManager.execute(sql, iot_id, temp, lamp1, lamp2);

}
