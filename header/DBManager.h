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
    void open(const std::string& db_path) {
        if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(db));
        }
    } // открыть БД

    void close() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    } // Закрыть БД

    template<typename... Args>
    void execute(const std::string& sql, Args&&... args) {
        prepare(sql);
        bindParameters(1, std::forward<Args>(args)...);
        executeStep();
        finalize();
    } // выполнение запроса без возращаемого значения

    template<typename T, typename... Args>
    std::vector<T> query(const std::string& sql, Args&&... args) {
        std::vector<T> results;
        prepare(sql);
        bindParameters(1, std::forward<Args>(args)...);

        while (executeStep() == SQLITE_ROW) {
            results.emplace_back(extractRow<T>());
        }

        finalize();
        return results;
    } // выполнение запросов с возращаемымм значением

    DBManager() {
        open("D:/study/project/app_server/database/data.sqlite");
    } // конструктор по умолчанию

    ~DBManager() {
        finalize();
        close();
    } // деструктор

private:
    void prepare(const std::string& sql) {
        finalize();
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(db));
        }
    } // Подготовка запроса

    template<typename T>
    void bind(int pos, T&& value) {
        using DecayedT = std::decay_t<T>;

        if constexpr (std::is_integral_v<DecayedT>) {
            sqlite3_bind_int(stmt, pos, value);
        } else if constexpr (std::is_floating_point_v<DecayedT>) {
            sqlite3_bind_double(stmt, pos, value);
        } else if constexpr (std::is_same_v<DecayedT, std::string>) {
            sqlite3_bind_text(stmt, pos, value.c_str(), value.size(), SQLITE_TRANSIENT);
        } else if constexpr (std::is_convertible_v<T, const char*>) {
            sqlite3_bind_text(stmt, pos, value, -1, SQLITE_TRANSIENT);
        } else {
            static_assert(sizeof(T) == 0, "Unsupported parameter type");
        }
    } // подстановка параметра

    template<typename T, typename... Args>
    void bindParameters(int pos, T&& first, Args&&... rest) {
        bind(pos, std::forward<T>(first));
        bindParameters(pos + 1, std::forward<Args>(rest)...);
    } // подстановка параметров

    void bindParameters(int pos) {} // базовый случай подстановки для реализации рекурсивного вызова
    template<typename T>
    T extractRow() {
        if constexpr (std::is_same_v<T, std::string>) {
            return reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else if constexpr (std::is_integral_v<T>) {
            return sqlite3_column_int(stmt, 0);
        } else if constexpr (std::is_floating_point_v<T>) {
            return sqlite3_column_double(stmt, 0);
        } else {
            static_assert(sizeof(T) == 0, "Unsupported result type");
        }
    } // извлечение данных из строки

    int executeStep() {
        int step = sqlite3_step(stmt);
        if(step != SQLITE_ROW && step != SQLITE_DONE) {
            throw std::runtime_error(sqlite3_errmsg(db));
        }
        return step;
    } // шаг выволнение запрса

    void finalize() {
        if(stmt) {
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    } // очистка мусора
};

#endif //APP_SERVER_DBMANAGER_H
