//
// Created by acm on 3/22/18.
//

#include "Submission.h"
#include "config.h"
#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>

using boost::filesystem::path;
using boost::filesystem::directory_iterator;
using std::ofstream;
using std::to_string;


Submission::Submission(string &&code, int problem_id, int submission_id, Language language, unsigned int time_limit,
                       unsigned int memory_limit, bool is_spj)
        : code_(std::move(code)), problem_id_(problem_id), submission_id_(submission_id),
          language_(language), time_limit_(time_limit), memory_limit_(memory_limit), is_spj_(is_spj) {

}


int Submission::compile() {
    ofstream fout("user.cpp");
    fout << code_;
    fout.close();
    int ret = system("g++ -std=c++11 -o Main user.cpp");
}

int Submission::single_test(string test_name) {
    path in_filepath(data_root.c_str());
    in_filepath /= to_string(problem_id_);
    path out_filepath(in_filepath);
    in_filepath /= test_name + ".in";
    out_filepath /= test_name + ".out";
    results_.emplace_back(Test(in_filepath.string(), out_filepath.string()));
    results_.rbegin()->debug();
}

int Submission::test() {
    path problem_dir(data_root.c_str());
    problem_dir /= to_string(problem_id_);
    map<string, int> cnt;
    for (directory_iterator di(problem_dir); di != directory_iterator(); ++di) {
        if (di->path().extension().string() == ".in" || di->path().extension().string() == ".out") {
            cnt[di->path().stem().string()]++;
        }
    }
    for (auto &p: cnt) {
        if (p.second == 2) {
            single_test(p.first);
        }
    }
    for (int i = 0; i < results_.size(); i++) {
        if (results_[i].result_ != AC) {
            final_result_ = results_[i].result_;
            break;
        }
    }
}


int Submission::run() {
    compile();
    test();
}