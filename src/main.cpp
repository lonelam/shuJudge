#include <iostream>
#include "Test.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Test t("1.in", "1.out");
    t.debug();
    return 0;
}