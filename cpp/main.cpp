#include <iostream>
#include "sodium.h"

int main()
{
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized; it is not safe to use */
    }
    return 0;
}

