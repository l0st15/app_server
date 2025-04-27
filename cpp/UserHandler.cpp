//
// Created by Даниил on 22.04.2025.
//
#include "UserHandler.h"


Response UserHandler::RequestProcesssing(const Request &req) {
    if(req.method == "POST") {
        if(req.path == "/reg")
            return userReg(req);
        else if(req.path == "/login")
            return userLogin(req);
        //else if(req.path == "/get_info")
          //  return userInfo(req);
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}

Response UserHandler::userReg(const Request &req) {
    if(!openDB())
        return Response(400, "Bad request");
    else {
        try {
            auto json = nlohmann::json::parse(req.body); // парсинг тела запроса
            // получаем логи и пароль
            std::string login = json["login"];
            std::string password = json["password"];

            if(login.empty() || password.empty())
                return Response(400, "Login and password are required");

            std::string hash_password = crypto_module.hashPassword(password); // генерация хеша пароля

            if(!openDB()) // открытие бд
                return Response(400, "Error open DB");

            std::string sql_query = "INSERT INTO user(login, hash_password) VALUES(?, ?)"; // тело запроса
            sqlite3_stmt* stmt; // указатель на параметризованный запрос
            // получение параметризованного запроса
            if(sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                std::cerr << "аааааа БИБЕ сломалось\n";
                return Response(400, "Error DB");
            }
            sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC); // подстановка логина
            sqlite3_bind_text(stmt, 2, hash_password.c_str(), -1, SQLITE_STATIC); // подстановка пароля

            if(sqlite3_step(stmt) != SQLITE_DONE) { // выполнение запроса
                sqlite3_finalize(stmt); // удаление запроса
                std::cerr << "User already exists\n";
                return Response(400, "User already exists");
            }

            sqlite3_finalize(stmt); // удаление запроса
            return Response(201, "User registered");

        }
        catch (const std::exception& e) {
            std::cerr << e.what();
            return Response(400, "Bad request");
        }
        closeDB();
    }
}

Response UserHandler::userLogin(const Request &req) {

    auto json = nlohmann::json::parse(req.body);
    std::string login = json["login"];
    std::string password = json["password"];

    if(login.empty() || password.empty())
        return Response(400, "Login and password are required");

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

std::string UserHandler::getUserHashPassword(const std::string& user_login) {

    if(!openDB())
        return "";

    std::string sql_query = "SELECT hash_password FROM user WHERE login = ?;";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "аааааа БИБЕ сломалось\n";
        return "";
    }
    sqlite3_bind_text(stmt, 1, user_login.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return "";
    }
    size_t length = sqlite3_column_bytes(stmt, 0);
    std::string hash_password((const char*)sqlite3_column_text(stmt, 0), length);

    sqlite3_finalize(stmt);
    closeDB();
    return hash_password;
}

bool UserHandler::openDB() {
    if(sqlite3_open(R"(D:\study\project\app_server\database\server_db)", &db)) {
        std::cerr << "Error open DB" << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else {
        std::cerr << "Opened DB Successfully!\n";
        return true;
    }
}

bool UserHandler::closeDB() {
    if(sqlite3_close(db)) {
        std::cerr << "Error close DB" << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else {
        std::cerr << "Opened DB Successfully!\n";
        return true;
    }
}



