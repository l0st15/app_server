//
// Created by Даниил on 02.03.2025.
//

#ifndef APP_SERVER_DBMANAGER_H
#define APP_SERVER_DBMANAGER_H

#include "sqlite3.h"
#include "string"
#include "vector"
#include "map"

class DBManager
{
protected:
    /*поля класса*/
    sqlite3* db = nullptr; // указатель на БД
    std::string path_to_db; // содержит путь до БД
    std::string path_to_logfile; // путь до файла с логами
    /*методы класса*/
    DBManager();
    DBManager(std::string& in_path_db, std::string& in_path_logfile); // конструктор по умолчанию(путь к БД прочитается из файла)
    ~DBManager(); // деструктор класса
    bool open_connection(); // подлючение к базе данных
    bool check_connection(); // проверка подключения к БД
    bool insert(sqlite3_stmt &in_stmt); // функция для выполнения запроса INSERT
    std::vector<std::map<std::string, std::string>> select(sqlite3_stmt &in_stmt); // функция для выполнения запросов SELECT
    bool close_connection();

};



#endif //APP_SERVER_DBMANAGER_H
