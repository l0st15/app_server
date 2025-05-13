#include <iostream>
#include "sodium.h"
#include "Crypto.h"
#include "Units.h"
#include "UserHandler.h"
#include "router.h"
#include "IRequestHandler.h"
#include "network.h"
#include "DBManager.h"

int main()
{
    /*
    Request req;
    Network net("127.0.0.1",47891);
    std::cout<<net.recieveRequest(req)<<std::endl;
    std::cout<<req.body<<std::endl;
    router r;
    r.registerRoute("/reg", std::make_unique<UserHandler>());
    r.registerRoute("/login", std::make_unique<UserHandler>());
    IRequestHandler* handler = r.route(req.path);
    Response res = handler->RequestProcesssing(req);
    std::cout<<net.sendResponse(res)<<std::endl; */

    Request req;
    req.method = "POST";
    req.path = "/addIot";
    req.body = R"({"token": "fe466db2-4019-4239-8e26-4543668863d4", "mac": "00:AB:CD:EF:11:22"})";
    std::cout << req.body << "\n";
    router r;
    r.registerRoute("/reg", std::make_unique<UserHandler>());
    r.registerRoute("/login", std::make_unique<UserHandler>());
    r.registerRoute("/get_info", std::make_unique<UserHandler>());
    r.registerRoute("/iot/sendCommand", std::make_unique<UserHandler>());
    r.registerRoute("/addIot", std::make_unique<UserHandler>());
    IRequestHandler* handler = r.route(req.path);
    Response res = handler->RequestProcesssing(req);
    std::cout << res.body << std::endl;

    //TODO проверить работоспосбность
    return 0;
}

