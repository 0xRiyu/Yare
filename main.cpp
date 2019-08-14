//
// Created by Drew on 2019-08-13.
//
#include <iostream>

struct T{
    int_fast32_t f = 1;
};

int main() {
    std::cout << "Hello World!" << std::endl;
    T t = T();
    std::cout << t.f << std::endl;
    return 0;
}
