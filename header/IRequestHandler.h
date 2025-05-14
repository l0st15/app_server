//
// Created by Даниил on 14.03.2025.
//

#ifndef APP_SERVER_IREQUESTHANDLER_H
#define APP_SERVER_IREQUESTHANDLER_H
#include "Units.h"

class IRequestHandler // интерфейс для всех обработчиков
{
public:
   virtual ~IRequestHandler() = default;
   virtual Response RequestProcessing(const Request& request) = 0;
};

#endif //APP_SERVER_IREQUESTHANDLER_H
