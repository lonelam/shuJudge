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
    listener_.support(methods::POST, std::bind(&Handler::handle_post, this, std::placeholders::_1));
    listener_.support(methods::DEL, std::bind(&Handler::handle_get, this, std::placeholders::_1));
}

void Handler::handle_get(http_request msg) {
    ucout << "GET API" << endl;
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
//    ucout << "POST API" << endl;
    msg.content_ready().wait();
    ucout << msg.to_string() << endl;

    unique_ptr<Submission> submission;
    try {
        json::value submission_json = msg.extract_json().get();
        string code_raw = submission_json[U("code")].as_string();
        string code_file;
//        code_file.resize(code_raw.size());
        for (int i = 0; i < code_raw.size(); i++) {
            if (code_raw[i] == '\\' && i + 1 < code_raw.size() && code_raw[i + 1] == 'n') {
                code_file.push_back('\n');
                i++;
            } else {
                code_file.push_back(code_raw[i]);
            }
        }
        submission.reset(new Submission(
                move(code_file),
                submission_json[U("problem_id")].as_integer(),
                submission_json[U("submission_id")].as_integer(),
                to_language(submission_json[U("language")].as_string()),
                submission_json[U("time_limit")].as_integer(),
                submission_json[U("memory_limit")].as_integer(),
                submission_json[U("is_spj")].as_bool()
        ));
        msg.reply(status_codes::OK, U("success")).then([&](pplx::task<void> t) {
            submission->run();
        }).wait();
    }
    catch (exception &e) {
        cout << e.what() << endl;
        msg.reply(status_codes::InternalError, U("500 Internal Error")).wait();
    }

}

