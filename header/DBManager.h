//
// Created by Даниил on 12.05.2025.
//

#ifndef APP_SERVER_DBMANAGER_H
#define APP_SERVER_DBMANAGER_H

#include <sqlite3.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>
#include <type_traits>

class DBManager {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

public:
    void open(const std::string& db_path);
    void close();
    template<typename... Args>
    void execute(const std::string& sql, Args&&... args);
    template<typename T, typename... Args>
    std::vector<T> query(const std::string& sql, Args&&... args);
    ~DBManager();
private:
    void prepare(const std::string& sql);
    template<typename T>
    void bind(int pos, T&& value);
    template<typename T, typename... Args>
    void bindParameters(int pos, T&& first, Args&&... rest);
    void bindParameters(int pos) {};
    template<typename T> T
            extractRow();
    int executeStep();
    void finalize();


};

#endif //APP_SERVER_DBMANAGER_H
