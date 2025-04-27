#include <iostream>
#include "sodium.h"
#include "Crypto.h"
#include "Request_and_Respore.h"
#include "UserHandler.h"
#include "router.h"
#include "IRequestHandler.h"
#include "network.h"

int main()
{
    Request req;
    Network net("127.0.0.1",47891);
    net.recieveRequest(req);
    std::cout<<req.body;
    router r;
    r.registerRoute("/reg", std::make_unique<UserHandler>());
    r.registerRoute("/login", std::make_unique<UserHandler>());
    IRequestHandler* handler = r.route(req.path);
    Response res = handler->RequestProcesssing(req);

    return 0;
}

