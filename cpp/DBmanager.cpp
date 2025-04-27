//
// Created by Даниил on 27.04.2025.
//
#include "DBmanager.h"

sqlite3_stmt* DBmanager::getStmt() {
    return stmt;
}

bool DBmanager::openDB() {

    if(sqlite3_open(R"(D:\study\project\app_server\database\server_db)", &db)) {
        std::cerr << "Error open DB" << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else {
        std::cerr << "Opened DB Successfully!\n";
        return true;
    }
}

bool DBmanager::closeDB() {

    sqlite3_finalize(stmt);

    if(sqlite3_close(db)) {
        std::cerr << "Error close DB" << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else {
        std::cerr << "Opened DB Successfully!\n";
        return true;
    }
}

void DBmanager::execQuery(const std::string& sql_query, const std::string& param) {

    if(sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "аааааа БИБЕ сломалось\n";
        stmt = nullptr;
    }
    sqlite3_bind_text(stmt, 1, param.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        stmt =  nullptr;
    }

}

void DBmanager::execQuery(const std::string& sql_query, const std::string& param1, const std::string& param2) {

    if(sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "аааааа БИБЕ сломалось\n";
        stmt = nullptr;
    }

    sqlite3_bind_text(stmt, 1, param1.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, param2.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        stmt =  nullptr;
    }
}

void DBmanager::getDataDB(int col_id, int& value) {

    value = sqlite3_column_int(stmt, col_id);
}

void DBmanager::getDataDB(int col_id, std::string& value) {

    size_t length = sqlite3_column_bytes(stmt, col_id);
    std::string buf((const char*)sqlite3_column_text(stmt, col_id), length);
    value = buf;
}


