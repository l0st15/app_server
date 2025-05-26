//
// Created by Даниил on 22.04.2025.
//
#include "UserHandler.h"
#include <iostream>

Response UserHandler::RequestProcessing(const Request &req) {
    if(req.method == "POST") {
        if(req.path == "/reg")
            return userReg(req);
        else if(req.path == "/login")
            return userLogin(req);
        else if(req.path == "/get_info")
            return userGetInfo(req);
        else if(req.path == "/iot/sendCommand")
            return sendCommand(req);
        else if(req.path == "/addIot")
            return addIot(req);
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}

Response UserHandler::userReg(const Request &req) {
    if (req.body.empty()) {
        return Response(400, "Bad request");
    }
    try {
        auto json = nlohmann::json::parse(req.body);
        std::string login = json["login"];
        std::string hash = crypto_module.hashPassword(json["password"]);

        dbManager.execute("INSERT INTO user (login, hash) VALUES (?, ?)", login, hash);
    } catch (std::exception &e) {
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
        dbManager.execute("DELETE FROM token WHERE user_id = ?", user_id[0]);
        dbManager.execute("INSERT INTO token (user_id, token) VALUES(?, ?)", user_id[0], token);
        nlohmann::json res_json;
        res_json["token"] = token;

        auto iot_ids = dbManager.query<int>("SELECT iot_id FROM iot_user WHERE user_id = ?", user_id[0]);
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
        if (req.body.empty())
            throw std::invalid_argument("Body is empty");

        auto json = nlohmann::json::parse(req.body);
        std::string token = json["token"];
        userAuth(token);
        int id_command = json["command"];
        int iot_id = json["iot_id"];

        auto it = commands.find(id_command);

        if (it == commands.end())
            throw std::invalid_argument("Command not found");

        dbManager.execute("INSERT INTO commands (iot_id, command) VALUES (?, ?)", iot_id, it->second);

    } catch (std::exception& e)
    {
        return Response(400, e.what()); // вот коды + что случилось
    }

    return Response();
}

Response UserHandler::userGetInfo(const Request &req) {

    Response res;

    try {

        if (req.body.empty())
            throw std::invalid_argument("Body is empty");

        auto json = nlohmann::json::parse(req.body);
        std::string token = json["token"];
        int iot_id = json["iot_id"];
        int type = json["type"];
        int user_id = userAuth(token);

        auto iot_user = dbManager.query<int>("SELECT 1 FROM iot_user WHERE iot_id = ? AND user_id = ?", iot_id, user_id);
        nlohmann::json res_body;
        res_body["data"] = nlohmann::json::array();

        if (iot_user.size() != 1) {
            throw std::invalid_argument("iot not found");
        }

        if (type == 0) {
            std::string sql = "SELECT temp, lamp1, lamp2, timestamp FROM data_iot WHERE iot_id = ? ORDER BY timestamp LIMIT 1";
            auto iot_data = dbManager.query<data>(sql, iot_id);
            if (iot_data.size() != 1) {
                throw std::runtime_error("Ошибка чтения данных");
            }

            res_body["data"] = {{"temp:", iot_data[0].temp}
                                ,{"lamp1:", iot_data[0].lamp1}
                                ,{"lamp2:", iot_data[0].lamp2}
                                ,{"timestamp:", iot_data[0].timestamp}};

            res.body = res_body.dump();
        }
        else if (type == 1) {
            std::string start = json["start"];
            std::string  end = json["end"];  //TODO проверку что start < end
            std::string sql = "SELECT temp, lamp1, lamp2, timestamp FROM data_iot WHERE iot_id = ? AND timestamp BETWEEN ? AND ?";
            auto iot_data = dbManager.query<data>(sql, iot_id, start, end);

            if (iot_data.empty())
                throw std::runtime_error("No data");

            for (const auto& it : iot_data) {
                res_body["data"].push_back({{"temp:", it.temp}
                                                ,{"lamp1:", it.lamp1}
                                                ,{"lamp2:", it.lamp2}
                                                ,{"timestamp", it.timestamp}});
            }

            res.body = res_body.dump();
        }
        else {
            std::cout << "Type not found\n";
            throw std::invalid_argument("Type not found");
        }

    } catch (std::exception& e)
    {
        std::cout << e.what();
        res.statusCode = 400;
        res.statusMessage = e.what();
        return res;
    }
    return res;
}

Response UserHandler::addIot(const Request &req) {

    Response res;

    try {

        if(req.body.empty())
            throw std::invalid_argument("Body is empty");

        auto json = nlohmann::json::parse(req.body);
        std::string token = json["token"];

        if (!crypto_module.isValidUuid(token))
            throw std::invalid_argument("Token invalid");

        userAuth(token);

        std::string mac = json["mac"]; //TODO проверка формата mac у нас пользователь отправляет серкте для iot??
        std::string iot_token = json["iot_token"];

        dbManager.execute("INSERT INTO iot (mac, iot_token) VALUES(?, ?)", mac, iot_token);
        auto iot_id = dbManager.query<int>("SELECT id FROM iot WHERE mac = ?", mac);

        if(iot_id.empty())
            throw std::runtime_error("Iot not found");

        nlohmann::json body;
        body["iot_id"] = iot_id[0];
        res.body = body.dump();

    } catch (std::exception& e)
    {
        res.statusCode = 400;
        res.statusMessage = e.what();
        std::cout << e.what() << "\n";
        return res;
    }

    return res;
}

int UserHandler::userAuth(const std::string &uuid) {

    if(!crypto_module.isValidUuid(uuid))
        throw std::invalid_argument("UUID is not valid");

    auto id = dbManager.query<int>("SELECT user_id FROM token WHERE token = ?", uuid);

    if(id.empty())
        throw std::invalid_argument("User not found");
    if(id.size() != 1)
        throw std::invalid_argument("UUID not unique");

    return id[0];
}




//TODO нормальные коды HTTP (для БАБУ)
//TODO ОБработка ошибок json
//TODO Исключения
//TODO время жизни токена


