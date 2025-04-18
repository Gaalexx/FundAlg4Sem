#include <iostream>

#include "my_container.h"

using namespace my_container;

int main(){
    Deque<int> d = {1, 2, 3, 4};
    for (auto i = d.begin(); i != d.end(); i++)
    {
        std::cout << *i << std::endl;
    }
    return 0;
}