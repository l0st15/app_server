#include <iostream>
#include "sodium.h"
#include "Crypto.h"
#include "Request_and_Respore.h"
#include "UserHandler.h"
#include "router.h"
#include "IRequestHandler.h"

int main()
{
    std::cout << "Hello";
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized; it is not safe to use */
        std::cout << "World";
    }
     Crypto test;

    std::string test_uuid = test.uuidGen();
    std::cout << "uuid: " << test_uuid << "\n";
    std::string test_password = "12345678";
    std::string hash = test.hashPassword(test_password);
    std::cout << "Password: " << test_password << "\n" << "Hash: " << hash << "\n";
    std::cout << "Verify: " << test.verifyPassword(hash, test_password) << "\n";
    std::cout << "Valid uuid: " << test.isValidUuid(test_uuid);

    Request req;
    req.path = "/reg";
    req.method = "POST";
    std::string json_str = R"({"login": "test", "password": "1234"})";
    req.boby = json_str;
    router r;
    IRequestHandler* handler = new UserHandler();
    r.registerRoute("/reg", std::make_unique<UserHandler>());
    Response res = handler->RequestProcesssing(req);
    delete handler;

    return 0;
}

