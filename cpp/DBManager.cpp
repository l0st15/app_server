//
// Created by Даниил on 12.05.2025.
//
#include "DBManager.h"

void DBManager::open(const std::string &db_path) {
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
}
void DBManager::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

template<typename ...Args>
void DBManager::execute(const std::string &sql, Args &&...args) {
    prepare(sql);
    bindParameters(1, std::forward<Args>(args)...);
    executeStep();
    finalize();
}

template<typename T, typename ...Args>
std::vector<T> DBManager::query(const std::string &sql, Args &&...args) {
    std::vector<T> results;
    prepare(sql);
    bindParameters(1, std::forward<Args>(args)...);

    while (executeStep() == SQLITE_ROW) {
        results.emplace_back(extractRow<T>());
    }

    finalize();
    return results;

}
DBManager::~DBManager() {
    open("database/data.sqlite");
}
DBManager::~DBManager() {
    finalize();
    close();
}

void DBManager::prepare(const std::string &sql) {
    finalize();
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
}

template<typename T>
void DBManager::bind(int pos, T &&value) {
    if constexpr (std::is_integral_v<T>) {
        sqlite3_bind_int(stmt, pos, value);
    } else if constexpr (std::is_floating_point_v<T>) {
        sqlite3_bind_double(stmt, pos, value);
    } else if constexpr (std::is_same_v<T, std::string>) {
        sqlite3_bind_text(stmt, pos, value.c_str(), value.size(), SQLITE_TRANSIENT);
    } else {
        static_assert(sizeof(T) == 0, "Unsupported parameter type");
    }
}

template<typename T, typename ...Args>
void DBManager::bindParameters(int pos, T &&first, Args &&...rest) {
    bind(pos, std::forward<T>(first));
    bindParameters(pos + 1, std::forward<Args>(rest)...);
}

template<typename T>
T DBManager::extractRow() {
    if constexpr (std::is_same_v<T, std::string>) {
        return reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else if constexpr (std::is_integral_v<T>) {
        return sqlite3_column_int(stmt, 0);
    } else if constexpr (std::is_floating_point_v<T>) {
        return sqlite3_column_double(stmt, 0);
    } else {
        static_assert(sizeof(T) == 0, "Unsupported result type");
    }
}

int DBManager::executeStep() {
    int step = sqlite3_step(stmt);
    if(step != SQLITE_ROW && step != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
    return step;
}

void DBManager::finalize() {
    if(stmt) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
}