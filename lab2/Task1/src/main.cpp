#include "BigInt.hpp"
#include <iostream>

int main()
{
    std::cout << BigInt(10100) / BigInt(10) << std::endl;
    return 0;
}