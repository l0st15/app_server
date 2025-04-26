//
// Created by Даниил on 26.04.2025.
//

#ifndef APP_SERVER_CRYPTO_H
#define APP_SERVER_CRYPTO_H
#include <iostream>
#include <string>
#include <sodium.h>
class Crypto
{
public:
    std::string hashPassword(const std::string &password);
    bool verifyPassword(const std::string &hash, std::string &password);
    std::string uuidGen();
    bool isValidUuid(const std::string &uuid);
};

#endif //APP_SERVER_CRYPTO_H
