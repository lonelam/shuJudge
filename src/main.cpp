#include <iostream>
#include "Test.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Test t("a.in", "a.out");
    system("g++ -std=c++11 -o Main a.cpp");
    t.debug();
    return 0;
}