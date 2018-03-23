#include <iostream>
#include <fstream>
#include "Test.h"
#include "Submission.h"

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    Test t("a.in", "a.out");
    ifstream fin("a.cpp");
    std::string file((std::istreambuf_iterator<char>(fin)),
                     std::istreambuf_iterator<char>());
    Submission s(move(file), 1001, 1, cpp, 1000, 256 * 1024, false);
    s.compile();
    s.test();
    cout << STATE[s.final_result_] << endl;
    return 0;
}