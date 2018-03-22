//
// Created by acm on 3/22/18.
//

#ifndef SHUJUDGE_SUBMISSION_H
#define SHUJUDGE_SUBMISSION_H

#include <string>
#include "util.h"

using std::string;

class Submission {
private:
    string code_;
    int problem_id_;
    int submission_id_;
    Language language_;

public:
    int compile();

    int test();
};


#endif //SHUJUDGE_SUBMISSION_H
