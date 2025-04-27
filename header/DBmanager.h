//
// Created by Даниил on 27.04.2025.
//

#ifndef APP_SERVER_DBMANAGER_H
#define APP_SERVER_DBMANAGER_H
#include "sqlite3.h"
#include <string>
#include <iostream>

class DBmanager {
private:
    sqlite3* db;
    sqlite3_stmt* stmt;
public:
    sqlite3_stmt* getStmt();
    bool openDB();
    bool closeDB();
    void execQuery(const std::string& sql_query, const std::string& param);
    void execQuery(const std::string& sql_query, const std::string& param1, const std::string& param2);
    void getDataDB(int col_id, int& value);
    void getDataDB(int col_id, std::string& value);


};

#endif //APP_SERVER_DBMANAGER_H
