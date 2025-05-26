//
// Created by Даниил on 25.05.2025.
//

#ifndef APP_SERVER_DEFAULTHANDLER_H
#define APP_SERVER_DEFAULTHANDLER_H
#include "IRequestHandler.h"

class DefaultHandler : public IRequestHandler {

    Response RequestProcessing(const Request& request) override
    {
        return Response(405,"Method not allowed");
    }

};

#endif //APP_SERVER_DEFAULTHANDLER_H
