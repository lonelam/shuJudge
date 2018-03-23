//
// Created by acm on 3/22/18.
//

#ifndef SHUJUDGE_SUBMISSION_H
#define SHUJUDGE_SUBMISSION_H

#include <string>
#include "util.h"
#include "Test.h"

using std::string;

class Submission {
public:
    State final_result_ = AC;

    Submission(string &&code, int problem_id, int submission_id,
               Language language, unsigned int time_limit, unsigned int memory_limit,
               bool is_spj);

    int compile();

    //checksum function

    int test();

    int run();


private:
    string code_;
    int problem_id_;
    int submission_id_;
    Language language_;
    //in milliseconds
    unsigned int time_limit_ = 1000;
    //in KB
    unsigned int memory_limit_ = 256 * 1024;

    bool is_spj_;

    vector<Test> results_;


    //in milliseconds, should be the max time_usage in all tests
    time_t final_time_usage_ = 0;
    //in KB, should be the max memory_usage in all tests
    long int memory_usage_ = 0;

    int single_test(string test_name);


};


#endif //SHUJUDGE_SUBMISSION_H
