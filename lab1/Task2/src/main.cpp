#include <iostream>
#include "my_container.h"

using namespace my_container;

int main(){
    List<int> list{1, 2, 3, 4};

    list.push_back(33);

    std::cout << list[3] << std::endl;

    return 0;
}