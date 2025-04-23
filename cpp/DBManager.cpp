//
// Created by Даниил on 02.03.2025.
//
#include "DBManager.h"
#include <iostream>

// переписать методы используя параметизированые запросы и и sqlite3_stmt!!!

DBManager::DBManager() {};

DBManager::DBManager(std::string &in_path_db, std::string &in_path_logfile) : path_to_db(in_path_db), path_to_logfile(in_path_logfile) {};

DBManager::~DBManager()
{
    if(sqlite3_close(db))
        sqlite3_free(db);
    else
        std::cerr << std::string(sqlite3_errmsg(db));
};

bool DBManager::open_connection()
{
    if(!sqlite3_open(path_to_db.c_str(), &db))
    {
        std::cerr << "Ошибка открытия БД: " << std::string(sqlite3_errmsg(db)); // записывать также в логи
        return false;
    }
    else
    {
        std::cerr << "Успешное открытие БД"; // записывать также в логи
        return true;
    }
}

bool DBManager::check_connection()
{
    return true;
}






