#include <iostream>
#include "UserHandler.h"
#include "router.h"
#include "network.h"
#include "IotHandler.h"

int main()
{

    Network net("127.0.0.1",47891);
    router r;
    r.registerRoute("/reg", std::make_unique<UserHandler>());
    r.registerRoute("/login", std::make_unique<UserHandler>());
    r.registerRoute("/get_info", std::make_unique<UserHandler>());
    r.registerRoute("/iot/sendCommand", std::make_unique<UserHandler>());
    r.registerRoute("/addIot", std::make_unique<UserHandler>());
    r.registerRoute("/iot/sendTemp", std::make_unique<IotHandler>());
    while(true) {
        Request req;
        std::cout << net.recieveRequest(req) << std::endl;
        std::cout << req.body << std::endl;
        /*router r;
        r.registerRoute("/reg", std::make_unique<UserHandler>());
        r.registerRoute("/login", std::make_unique<UserHandler>());
        IRequestHandler* handler = r.route(req.path);
        Response res = handler->RequestProcesssing(req);
        std::cout<<net.sendResponse(res)<<std::endl; */

        /* req.method = "POST";
         req.path = "/iot/sendTemp";
         req.body = R"({"mac": "00:AB:CD:EF:11:22", "temp": 10.5, "lamp1": 1, "lamp2": 0})";
         std::cout << req.body << "\n";*/

        IRequestHandler *handler = r.route(req.path);
        Response res = handler->RequestProcessing(req);
        std::cout << res.body << std::endl;
        std::cout << net.sendResponse(res) << std::endl;
        //TODO проверить работоспосбность
        //TODO Перенести req в цикл while
    }
    return 0;
}

