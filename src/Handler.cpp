//
// Created by acm on 3/25/18.
//

#include "Handler.h"
#include "Submission.h"
using web::http::status_codes;
using namespace std;
using namespace web;

Handler::Handler(utility::string_t uri) : listener_(uri) {
    using web::http::methods;
    listener_.support(methods::GET, bind(&Handler::handle_get, this, placeholders::_1));
    listener_.support(methods::PUT, std::bind(&Handler::handle_get, this, std::placeholders::_1));
    listener_.support(methods::POST, std::bind(&Handler::handle_get, this, std::placeholders::_1));
    listener_.support(methods::DEL, std::bind(&Handler::handle_get, this, std::placeholders::_1));
}

void Handler::handle_get(http_request msg) {
    ucout << msg.to_string() << endl;
    msg.reply(status_codes::OK, U("<h1>get test</h1>"), U("text/html"))
            .then([=](pplx::task<void> t) {
                try {
                    t.wait();
                }
                catch (...) {
                    msg.reply(status_codes::InternalError, U("500 Internal Error"));
                }
            });
    return;
}

void Handler::handle_post(http_request msg) {
    ucout << msg.to_string() << endl;
    json::value submission_json = msg.extract_json().get();
    try {

        Submission submission();
        submission_json[U("submission_id")].as_integer();
    }
    catch (exception &e) {
        cout << e.what() << endl;
    }

}