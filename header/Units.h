//
// Created by Даниил on 04.05.2025.
//

//тут содержаться полезные структуры и вспомогательные функции

#ifndef APP_SERVER_UNITS_H
#define APP_SERVER_UNITS_H

#include <string>
#include <map>

struct data_iot {
    double temp;
    int lamp1;
    int lamp2;
    std::string timestamp;
};

std::map<int, std::string> commands { {1, "power_off"}
                            ,{2, "lamp1_on"}
                            ,{3, "lamp1_off"}
                            ,{4, "lamp2_on"}
                            ,{5, "lamp2_off"}};

struct Request
{
    std::string method; //что надо сделать (GET, PUT, POST)
    std::string path; //путь к объекту доступа (куда общается пользователь например /user/id_user, /iot/id_iot)
    std::map<std::string, std::string> headers; // заголовки запроса формата (имя_заголовка, значение)
    std::string http_version; //версия протокла http
    int body_length = 0; // длинна тела запроса (по умолчанию 0)
    std::string body; //тело запроса (тут передаются данные)
    std::map<std::string, std::string> query_param; // парметры пути(в URL после ?)
};

struct Response
{
    int statusCode; // статус выполнения запроса формата(код)
    std::string statusMessage; // статус выполнения запроса формата(Сообщение)
    std::map<std::string, std::string> headers; // заголовки формата (имя_заголовка, значение)
    int body_length = 0; // длинна тела ответа
    std::string body; // тело ответа
public:
    Response()
    {
        statusCode = 200;
        statusMessage = "OK";
    }
    Response(int code, const std::string& mes)
    {
        statusCode = code;
        statusMessage = mes;
    }
};
#endif //APP_SERVER_UNITS_H
