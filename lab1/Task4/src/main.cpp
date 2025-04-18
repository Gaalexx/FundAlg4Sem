#include <iostream>

#include "my_container.h"

using namespace my_container;

int main(){
    Stack<int> d = {1, 2, 3, 4};
    while(!d.empty()){
        std::cout << d.top() << " ";
        d.pop();
    }
    std::cout << std::endl;
    return 0;
}