#include <iostream>
#include "sodium.h"
#include "Crypto.h"
#include "Request_and_Respore.h"
#include "UserHandler.h"
#include "router.h"
#include "IRequestHandler.h"

int main()
{
    Request req;
    req.method = "POST";
    req.path = "/login";
    req.body = R"({"login": "test", "password": "12345678"})";
    router r;
    r.registerRoute("/reg", std::make_unique<UserHandler>());
    r.registerRoute("/login", std::make_unique<UserHandler>());
    IRequestHandler* handler = r.route(req.path);
    Response res = handler->RequestProcesssing(req);

    return 0;
}

