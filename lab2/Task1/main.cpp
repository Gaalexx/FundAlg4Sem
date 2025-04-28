#include "BigInt.hpp"
#include <iostream>

int main()
{
    /* for (BigInt i = 0; i <= BigInt(40); i++)
    {
        std::cout << i << ": " << (BigInt(1) << i) << " | " << mod_exp(BigInt(2), i) << std::endl;
    } */
    long long length = mod_exp(BigInt("123212141421"), BigInt("12312")).length();
    std::cout << length << std::endl;
    return 0;
}