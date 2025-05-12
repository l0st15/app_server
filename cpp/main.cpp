#include <iostream>
#include "sodium.h"
#include "Crypto.h"
#include "Units.h"
#include "UserHandler.h"
#include "router.h"
#include "IRequestHandler.h"
#include "network.h"

int main()
{
    Request req;
    Network net("127.0.0.1",47891);
    std::cout<<net.recieveRequest(req)<<std::endl;
    std::cout<<"Body: "<<req.body<<"EOB"<<std::endl;
    router r;
    r.registerRoute("/reg", std::make_unique<UserHandler>());
    r.registerRoute("/login", std::make_unique<UserHandler>());
    IRequestHandler* handler = r.route(req.path);
    Response res = handler->RequestProcesssing(req);
    std::cout<<net.sendResponse(res)<<std::endl;
    //TODO проверить работоспосбность
    return 0;
}

