//
// Created by Даниил on 22.04.2025.
//
#include <iostream>
#include "UserHandler.h"
#include "BCrypt.hpp"



Response UserHandler::process(const Request &req)
{
    if(req.method == "POST")
    {
        if(req.path == "/reg")
            return userReg(req);
        else if(req.path == "/login")
            return userLogin(req);
        else if(req.path == "/get_info")
            return userInfo(req);
        else if(req.path == "/get_info/poll")
            return userPoll(req);
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}

Response UserHandler::userReg(const Request &req)
{
    if(!openDB())
        return Response(400, "Bad request");
    else
    {
        try
        {
            auto json = nlohmann::json::parse(req.boby); // парсинг тела запроса
            // получаем логи и пароль
            std::string login = json["login"];
            std::string password = json["password"];

            if(login.empty() || password.empty())
                return Response(400, "Login and password are required");

            std::string hash_password = BCrypt::generateHash(password); // генерация хеша пароля

            if(!openDB()) // открытие бд
                return Response(400, "Error open DB");

            std::string sql_query = "INSERT INTO user(login, hash_password) VALUES(?, ?)"; // тело запроса
            sqlite3_stmt* stmt; // указатель на параметризованный запрос
            // получение параметризованного запроса
            if(sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            {
                std::cerr << "аааааа БИБЕ сломалось\n";
                return Response(400, "Error DB");
            }
            sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC); // подстановка логина
            sqlite3_bind_text(stmt, 2, hash_password.c_str(), -1, SQLITE_STATIC); // подстановка пароля

            if(sqlite3_step(stmt) != SQLITE_DONE) // выполнение запроса
            {
                sqlite3_finalize(stmt); // удаление запроса
                std::cerr << "User already exists\n";
                return Response(400, "User already exists");
            }

            sqlite3_finalize(stmt); // удаление запроса
            return Response(201, "User registered");

        }
        catch (const std::exception& e)
        {
            std::cerr << e.what();
            return Response(400, "Bad request");
        }
    }
}

Response UserHandler::userLogin(const Request &req)
{
    try
    {
        auto json = nlohmann::json::parse(req.boby); // получение логина и пароля
        std::string login = json["login"];
        std::string password = json["password"];

        if(login.empty() || password.empty())  // проверка на null
        {
            std::cerr << "login or password is null\n";
            return Response(400, "Login and password are required");
        }

        if(!openDB()) // открытие БД
        {
            std::cerr << "DB error\n";
            return Response(400, "DB error");
        }

        sqlite3_stmt* stmt;
        std::string sql_query = "SELECT id, password FROM user WHERE login = ?;"; // тело запроса

        if(sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) // параметризированный запрос
        {
            std::cerr << "prepare error\n";
            return Response(400, "DB error");
        }

        sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC); // подстановка параметров

        if (sqlite3_step(stmt) != SQLITE_ROW)
        {
            sqlite3_finalize(stmt);
            return Response(401, "Invalid credentials");
        }

        int user_id = sqlite3_column_int(stmt, 0); // получнеие id из БД
        std::string stored_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); // получение пароля из БД

        sqlite3_finalize(stmt);

        if(!BCrypt::validatePassword(password, stored_hash)) // проверка валидности пароля
        {
            return Response(401, "Invalid password");
        }

        std::string token = "secure token" + std::to_string(user_id); // типо генерация токена
        Response response;
        response.status[200] = "OK";
        response.headers["token"] = token;
        return response;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return Response(400, "Bad request");
    }
}

bool UserHandler::openDB()
{
    if(sqlite3_open("database/server_db.db", &db))
    {
        std::cerr << "Error open DB" << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else
    {
        std::cerr << "Opened DB Successfully!\n";
        return true;
    }
}



