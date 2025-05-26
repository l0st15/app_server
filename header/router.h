//
// Created by Даниил on 22.04.2025.
//
#ifndef APP_SERVER_ROUTER_H
#define APP_SERVER_ROUTER_H
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include "IRequestHandler.h"
#include "DefaultHandler.h"
class router
{
private:
    std::unordered_map<std::string, std::unique_ptr<IRequestHandler>> routes; //в данном поле хранятся маршруты
public:
    router()
    {
        /* решистрация сущ. маршрутов*/
    }
    void registerRoute(const std::string& path, std::unique_ptr<IRequestHandler> handler) // регистрация маршрутов
    {
        routes[path] = std::move(handler); //переадча владения обработчиком std::move создает ссылку на объект а не копирует его
    }
    IRequestHandler* route(const std::string& path) // поиск маршрута и возращение указателя на нужный обработчик
    {
        static DefaultHandler defaultHandler;
        auto it = routes.find(path);
        return (it != routes.end()) ? it->second.get() : &defaultHandler; // если it не конец то возращем обработчик иначе обработчик по умолчанию
    }
};

#endif //APP_SERVER_ROUTER_H
