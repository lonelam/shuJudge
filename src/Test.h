//
// Created by acm on 3/22/18.
//

#ifndef SHUJUDGE_TEST_H
#define SHUJUDGE_TEST_H

#include <string>
#include <vector>
#include "util.h"

using std::string;
using std::vector;
class Test {
private:


    const string bin_position_;
    string output_position_ = "user.out";
    string stdin_position_;
    string stdout_position_;
    Language language_;
    //in milliseconds
    unsigned int time_limit_ = 1000;
    //in KB
    unsigned int memory_limit_ = 256 * 1024;
    pid_t pid_;
    //in milliseconds
    time_t time_usage_ = 0;
    //in KB
    long int memory_usage_ = 0;

    State result = AC;

    vector<string> extra_info_;

    //子进程调用这个方法运行编译后的程序
    int run();

    //父进程调用这个方法监视子进程，更新状态
    int watch();

    //对输出结果进行评判，进入这里应当是能够正常输出的程序
    int judge();

public:
    Test(string stdin_position, string stdout_position, Language language = cpp);

    Test(string &stdin_position, string &stdout_position, Language language);

    int debug();

};


#endif //SHUJUDGE_TEST_H
