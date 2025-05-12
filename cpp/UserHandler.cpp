//
// Created by Даниил on 22.04.2025.
//
#include "UserHandler.h"
#include <iostream>


Response UserHandler::RequestProcesssing(const Request &req) {
    if(req.method == "POST") {
        if(req.path == "/reg")
            return userReg(req);
        else if(req.path == "/login")
            return userLogin(req);
        else if(req.path == "/get_info")
            return userGetInfo(req);
        else
            return Response(405, "Method Not Allowed");
    }
    else
        return Response(405, "Method Not Allowed");
}





