//
// Created by Даниил on 14.03.2025.
//

#ifndef APP_SERVER_REQUEST_AND_RESPORE_H
#define APP_SERVER_REQUEST_AND_RESPORE_H
#include <string>
#include <map>

struct Request
{
   std::string method; //что надо сделать (GET, PUT, POST)
   std::string path; //путь к объекту доступа (куда общается пользователь например /user/id_user, /iot/id_iot)
   std::map<std::string, std::string> headers; // заголовки запроса формата (имя_заголовка, значение)
   std::string http_version; //версия протокла http
   int boby_lenght = 0; // длинна тела запроса (по умолчанию 0)
   std::string boby; //тело запроса (тут передаются данные)
   std::map<std::string, std::string> query_param; // парметры пути(в URL после ?)
};

struct Respore
{
    std::map<int, std::string> status; // статус выполнения запроса формата(код, сообщенеие)
    std::map<std::string, std::string> headers; // заголовки формата (имя_заголовка, значение)
    int boby_lenght = 0; // длинна тела ответа
    std::string boby; // тело ответа
};

#endif //APP_SERVER_REQUEST_AND_RESPORE_H
