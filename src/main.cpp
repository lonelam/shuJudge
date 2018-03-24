#include <iostream>
#include <fstream>
#include "Test.h"
#include "Submission.h"
#include "Handler.h"
using namespace std;
using web::http::uri_builder;
int main() {
    std::cout << "Hello, World!" << std::endl;
//    Test t("a.in", "a.out");
//    ifstream fin("a.cpp");
//    std::string file((std::istreambuf_iterator<char>(fin)),
//                     std::istreambuf_iterator<char>());
//    Submission s(move(file), 1001, 1, cpp, 1000, 256 * 1024, false);
//    s.compile();
//    s.test();
//    cout << STATE[s.final_result_] << endl;
    utility::string_t port = U("34568");
    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);
    uri_builder uri(address);
    auto addr = uri.to_uri().to_string();
    std::unique_ptr<Handler> handler(new Handler(addr));
    handler->open().wait();
    ucout << U("Listening at ") << addr << endl;
    string line;
    getline(cin, line);
    handler->close().wait();
    return 0;
}