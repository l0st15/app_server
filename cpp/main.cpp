#include <iostream>
#include "UserHandler.h"
#include "router.h"
#include "network.h"
#include "IotHandler.h"
#include "Logger.h"

int main()
{
    Logger logger;
    logger.log("INFO", "NET", "Server start");
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
        IRequestHandler *handler = r.route(req.path);
        Response res = handler->RequestProcessing(req);
        std::cout << res.body << std::endl;
        std::cout << net.sendResponse(res) << std::endl;
    }


    return 0;
}

