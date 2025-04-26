#include <iostream>
#include "sodium.h"
#include "Crypto.h"

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
    std::string test_paswd = "12345678";
    std::string hash = test.hashPassword(test_paswd);
    std::cout << "Password: " << test_paswd << "\n" << "Hash: " << hash << "\n";
    std::cout << "Verify: " << test.verifyPassword(hash, test_paswd) << "\n";
    std::cout << "Valide uuid: " << test.isValidUuid(test_uuid);



    return 0;
}

