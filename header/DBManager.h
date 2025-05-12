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
    void open(const std::string& db_path); // открыть БД
    void close(); // Закрыть БД
    template<typename... Args>
    void execute(const std::string& sql, Args&&... args); // выполнение запроса без возращаемого значения
    template<typename T, typename... Args>
    std::vector<T> query(const std::string& sql, Args&&... args); // выполнение запросов с возращаемымм значением
    DBManager(); // конструктор по умолчанию
    ~DBManager(); // деструктор
private:
    void prepare(const std::string& sql); // Подготовка запроса
    template<typename T>
    void bind(int pos, T&& value); // подстановка параметра
    template<typename T, typename... Args>
    void bindParameters(int pos, T&& first, Args&&... rest); // подстановка параметров
    void bindParameters(int pos) {}; // базовый случай подстановки для реализации рекурсивного вызова
    template<typename T>
    T extractRow(); // извлечение данных из строки
    int executeStep(); // шаг выволнение запрса
    void finalize(); // очистка мусора


};

#endif //APP_SERVER_DBMANAGER_H
