//
// Created by Даниил on 26.04.2025.
//
#include <iomanip>
#include <sstream>
#include <regex>
#include "Crypto.h"

std::string Crypto::hashPassword(const std::string &password) {
    if(sodium_init() < 0) {
        throw std::runtime_error("Libsodium initialization failed");
    }

    char hash_password[crypto_pwhash_STRBYTES];
    if(crypto_pwhash_str(hash_password, password.c_str(), password.size(), crypto_pwhash_OPSLIMIT_MODERATE
            , crypto_pwhash_MEMLIMIT_MODERATE) != 0) {
        std::cerr << "Hash password error\n";
        throw std::runtime_error("Hash password error");
    }
    return hash_password;
}

bool Crypto::verifyPassword(const std::string &hash, std::string &password) {
    if(sodium_init() < 0) {
        throw std::runtime_error("Libsodium initialization failed");
    }
    return crypto_pwhash_str_verify(hash.c_str(), password.c_str(), password.size()) == 0;
}

std::string Crypto::uuidGen() {

    if (sodium_init() < 0) {
        throw std::runtime_error("Libsodium initialization failed");
    }

    unsigned char uuid_bytes[16];
    randombytes_buf(uuid_bytes, sizeof(uuid_bytes));
    //установка службных байтов для соотвестсвия стандарту
    uuid_bytes[6] = (uuid_bytes[6] & 0x0F) | 0x40; // Версия 4
    uuid_bytes[8] = (uuid_bytes[8] & 0x3F) | 0x80; // Вариант RFC 4122

    //генерация строки
    std::stringstream ss;
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(uuid_bytes[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            ss << "-";
        }
    }
    return ss.str();
}
bool Crypto::isValidUuid(const std::string &uuid) {
    static const std::regex pattern(
            "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-4[0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$"
    );
    return std::regex_match(uuid, pattern);
}