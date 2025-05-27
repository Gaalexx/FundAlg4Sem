#include "BigInt.hpp"
#include <iostream>

int main()
{
/*     BigInt a("2579");
    BigInt b("371"); */

    

    BigInt a("3564728996789543236726789423542678943542");
    BigInt b("564372898972564836574867859433678953449335647892");

    /* BigInt a("1000000000000000000000000000000000");
    BigInt b("10000000000000000"); */
    BigInt expected("121932631571862410339465161399045027508533565115");



    std::cout << a * b << std::endl;
    std::cout << a.fft_multiply(b) << std::endl;
    std::cout << a.karatsuba_multiply(b) << std::endl;
    /* std::vector<unsigned long long> v{1, 2, 3, 4, 5};
    auto vr = BigInt::rev(v, 4);
    for (size_t i = 0; i < vr.size(); i++)
    {
        std::cout << vr[i] << " ";
    }
    std::cout << std::endl; */

/* 
    std::cout << a.newton_divide(b) << std::endl; */
    
    return 0;
}