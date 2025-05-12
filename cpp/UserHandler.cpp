//
// Created by Даниил on 22.04.2025.
//
#include "UserHandler.h"
#include <iostream>

Response UserHandler::RequestProcesssing(const Request &req) {
    if(req.method == "POST") {
        if(req.path == "/reg")
            return userReg(req);
        else if(req.path == "/login")
            return userLogin(req);
        else if(req.path == "/get_info")
            return userGetInfo(req);
        else if(req.path == "/iot/sendCommand")
            return sendCommand(req);
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}

Response UserHandler::userReg(const Request &req) {
    if(req.body.empty())
    {
        return Response(400, "Bad request");
    }
    try {
        auto json = nlohmann::json::parse(req.body);
        std::string login = json["login"];
        std::string hash = crypto_module.hashPassword(json["password"]);

        dbManager.execute("INSERT INTO user (login, hash) VALUES (?, ?)", login, hash);
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
        return Response(400, "Bad request");
    }

    return Response();
}

Response UserHandler::userLogin(const Request &req) {

    Response res;

    if(req.body.empty())
    {
        return Response(400, "Bad request");
    }
    try {
        auto json = nlohmann::json::parse(req.body);
        std::string login = json["login"];
        std::string password = json["password"];
        auto hash = dbManager.query<std::string>("SELECT hash FROM user WHERE login = ?", login);
        auto user_id = dbManager.query<int>("SELECT id FROM user WHERE login = ?", login);
        if(hash.empty() || user_id.empty()) {
            std::cout << "User not found\n";
            return Response(400, "Bad request");
        }

        if(!crypto_module.verifyPassword(hash[0], password))
        {
            std::cout << "Auth faild\n";
            return Response(400, "Bad request");
        }

        std::string token = crypto_module.uuidGen();
        nlohmann::json res_json;
        res_json["token"] = token;

        auto iot_ids = dbManager.query<int>("SELECT iot_id FROM iot_user WHERE id = ?", user_id[0]);
        if(iot_ids.empty()) {
            res_json["iot_ids"] = "Iots not found";
        }
        else {
            res_json["iot_ids"] = nlohmann::json::array();
            for(const auto& id : iot_ids)
            {
                res_json["iot_ids"].push_back(id);
            }
        }

        res.body = res_json.dump();

    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
        return Response(400, "Bad request");
    }

    return res;
}

Response UserHandler::sendCommand(const Request &req) {

    try {
        if(req.body.empty())
            throw std::invalid_argument("Body is empty");

        auto json = nlohmann::json::parse(req.body);
        std::string token = json["token"];
        userAuth(token);
        int id_command = json["command"];
        int iot_id = json["iot_id"];

        auto it = commands.find(id_command);

        if(it == commands.end())
            throw std::invalid_argument("Command not found");

        dbManager.execute("INSERT INTO commands (iot_id, command) VALUES (?, ?)", iot_id, it->second);

    } catch (std::exception& e)
    {
        return Response(400, e.what());
    }

    return Response();
}

int UserHandler::userAuth(const std::string &uuid) {

    if(!crypto_module.isValidUuid(uuid))
        throw std::invalid_argument("UUID is not valid");

    auto id = dbManager.query<int>("SELECT id FROM token WHERE token = ?", uuid);

    if(id.empty())
        throw std::invalid_argument("User not found");
    if(id.size() != 1)
        throw std::invalid_argument("UUID not unique");

    return id[0];
}




//TODO нормальные коды ошибок (для БАБУ)

