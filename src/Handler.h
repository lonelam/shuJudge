//
// Created by acm on 3/25/18.
//

#ifndef SHUJUDGE_HANDLER_H
#define SHUJUDGE_HANDLER_H

#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
using web::http::experimental::listener::http_listener;
using web::http::http_request;

class Handler {
public:
    Handler();

    Handler(utility::string_t uri);

    pplx::task<void> open() { return listener_.open(); }

    pplx::task<void> close() { return listener_.close(); }

private:
    void handle_get(http_request msg);

    void handle_post(http_request msg);

    //void handle_put(http_request msg);
    http_listener listener_;
};


#endif //SHUJUDGE_HANDLER_H
