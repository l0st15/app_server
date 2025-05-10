//
// Created by Даниил on 27.04.2025.
//
#pragma once

#ifndef APP_SERVER_DBMANAGER_H
#define APP_SERVER_DBMANAGER_H
#include "sqlite3.h"
#include "Units.h"
#include <string>
#include <iostream>
#include <map>

class DBmanager {
private:
    sqlite3* db;
    sqlite3_stmt* stmt;
public:
    DBmanager();
    ~DBmanager();
    sqlite3* getDB();
    sqlite3_stmt* getStmt();
    bool openDB();
    bool closeDB();
    void execQuery(const std::string& sql_query, const std::string& param);
    void execQuery(const std::string& sql_query, const int& param1, const int& param2);
    void execQuery(const std::string& sql_query, const std::string& param1, const std::string& param2);
    void execQuery(const std::string& sql_query, const int& param1, const int& param2,
                   const std::string& param3, const std::string& param4);
    void execQuery(const std::string & sql_query, const int& param1, const std::string& param2);
    void getDataDB(unsigned int col_id, int& value);
    void getDataDB(unsigned int col_id, std::string& value);
    data_iot getDataDB();
};



#endif //APP_SERVER_DBMANAGER_H
