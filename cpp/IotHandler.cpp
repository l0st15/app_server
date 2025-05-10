//
// Created by Даниил on 10.05.2025.
//

#include "IotHandler.h"

Response IotHandler::RequestProcesssing(const Request &req) {
    if(req.method == "POST")
    {
        if(req.path == "/iot/power")
            return controlPower(req);
        else if(req.path == "/iot/lamp")
            return controlLamp(req);
        else if(req.path == "/iot/sendTemp")
            return sendTemp(req);
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}

Response IotHandler::controlLamp(const Request &req) {
    return Response();
}

Response IotHandler::sendTemp(const Request &req) {
    return Response();
}
