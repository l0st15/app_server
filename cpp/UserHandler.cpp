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
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}

Response UserHandler::userReg(const Request &req) {
    if(!db_module.openDB())
        return Response(503, "Service Unavailable");
    else {
        try {
            std::string body = req.body;
            body.erase(std::remove(body.begin(), body.end(), '\\'),body.end());
            std::cout<<body<<std::endl;
            auto json = nlohmann::json::parse(body.c_str()); // парсинг тела запроса
            // получаем логи и пароль
            std::string login = json["login"];
            std::string password = json["password"];

            if(login.empty() || password.empty())
                return Response(403, "Forbidden");

            std::string hash_password = crypto_module.hashPassword(password); // генерация хеша пароля

            if(!db_module.openDB()) // открытие бд
                return Response(503, "Error open DB");

            std::string sql_query = "INSERT INTO user(login, hash_password) VALUES(?, ?)"; // тело запроса

            db_module.execQuery(sql_query, login, hash_password);
            if(db_module.getStmt() == nullptr)
                return Response(500, "Something went wrong");

            return Response(201, "User registered");

        }
        catch (const std::exception& e) {
            std::cerr << e.what();
            return Response(500, "Internal Server Error");
        }
        db_module.closeDB();
    }
}

Response UserHandler::userLogin(const Request &req) {

    auto json = nlohmann::json::parse(req.body);
    std::string login = json["login"];
    std::string password = json["password"];

    if(login.empty() || password.empty())
        return Response(403, "Forbidden");

    std::string hash_password = getUserHashPassword(login);
    if(crypto_module.verifyPassword(hash_password, password)) {
        nlohmann::json user_uuid;
        user_uuid["uuid"] = crypto_module.uuidGen();
        Response res(200, "OK");
        res.body = user_uuid.dump();
        res.body_length = res.body.size();
        return res;
    } else
        return Response(401, "ТИ Мошенник");

}

Response UserHandler::userGetInfo(const Request &req) {

    auto json = nlohmann::json::parse(req.body);
    std::string uuid = json["uuid"];
    int iot_id = stoi(std::string(json["iot_id"]));
    int user_id = userAuth(uuid);
    if(user_id < 0)
        return Response(401, "Ти мошенник");

    int type = stoi(std::string(json["type"]));

    if(!db_module.openDB())
        return Response(503, "Error open DB");

    std::string sql_query;
    sqlite3_stmt* stmt;

    switch (type) {
        case 0: {
            sql_query = "SELECT temp, lamp1, lamp2, timestamp FROM data "
                        "WHERE iot_id = ? AND user_id = ? ORDER BY created_at DESC LIMIT;";
            db_module.openDB();
            db_module.execQuery(sql_query, iot_id, user_id);
            data_iot data = db_module.getDataDB();
            db_module.closeDB();
            nlohmann::json json_data;
            json_data["data"] = {{"temp", data.temp}, {"lamp1", data.lamp1}, {"lamp2", data.lamp2},
                                 {"timestamp", data.timestamp}};
            Response res;
            res.body = json_data.dump();

            return  res;
        }
        case 2: {
            std::string start = json["start"];
            std::string end = json["end"];

            sql_query = "SELECT temp, lamp1, lamp2, timestamp FROM data "
                        "WHERE iot_id = ? AND user_id = ? AND timestamp BETWEEN ? AND ?;";
            db_module.openDB();
            db_module.execQuery(sql_query, iot_id, user_id, start, end);
            nlohmann::json j_data;
            j_data["data"] = nlohmann::json::array();
            while (sqlite3_step(db_module.getStmt()) == SQLITE_ROW) {
                data_iot data = db_module.getDataDB();
                j_data["data"].push_back({
                                                 {"temp",      data.temp},
                                                 {"lamp1",     data.lamp1},
                                                 {"lamp2",     data.lamp2},
                                                 {"timestamp", data.timestamp},
                                         });
            };

            Response res;
            res.body = j_data.dump();
            return res;
        }
        default:
            return Response(404, "Content not found");
    }
}

int UserHandler::userAuth(const std::string& uuid) {

    if(!db_module.openDB()) {
        return -1;
    }

    std::string sql_query = "SELECT user_id FROM user_token WHERE token = ?;";
    db_module.execQuery(sql_query, uuid);
    if(db_module.getStmt() == nullptr)
        return -1;

    int id;
    db_module.getDataDB(0, id);
    db_module.closeDB();
    return id;
}

std::string UserHandler::getUserHashPassword(const std::string& user_login) {

    if(!db_module.openDB())
        return "";

    std::string sql_query = "SELECT hash_password FROM user WHERE login = ?;";
    db_module.execQuery(sql_query, user_login);
    if(db_module.getStmt() == nullptr)
        return "";

    std::string hash_password;
    db_module.getDataDB(0, hash_password);
    db_module.closeDB();

    return hash_password;
}





