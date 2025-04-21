#include <iostream>
#include "my_container.h"

using namespace my_container;


int main(int argc, char *argv[])
{

    (void)argc; (void)argv;

    Array<const int, 5> arr{10, 20, 30, 40, 50};

    for (auto i = arr.cbegin(); i != arr.cend(); i++)
    {
        std::cout << *i << " ";
        
    }
    std::cout << std::endl;
    for (auto i = arr.crbegin(); i != arr.crend(); i++)
    {
        std::cout << *i << " ";
        
    }
    std::cout << std::endl;
    return 0;
}
