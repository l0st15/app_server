//
// Created by Даниил on 14.03.2025.
//

#ifndef APP_SERVER_IREQUESTHANDLER_H
#define APP_SERVER_IREQUESTHANDLER_H
#include "Request_and_Respore.h"

class IRequestHandler // интерфейс для всех обработчиков
{
public:
   virtual ~IRequestHandler() = default;
   virtual Response RequestProcesssing(const Request& request) = 0;
};

#endif //APP_SERVER_IREQUESTHANDLER_H
