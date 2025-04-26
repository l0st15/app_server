#include <iostream>
#include "sodium.h"

int main()
{
    std::cout << "Hello";
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized; it is not safe to use */
        std::cout << "World";
    }
    std::string a;
    std::cin >> a;
    return 0;
}

