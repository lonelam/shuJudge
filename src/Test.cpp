//
// Created by acm on 3/22/18.
//

#include "Test.h"
#include <sys/ptrace.h>
#include <iostream>
#include <cassert>
#include <algorithm>

#define STD_KB 1024
#define STD_MB 1048576

#define STD_F_LIM (STD_MB<<5)

Test::Test(string stdin_position, string stdout_position, Language language)
        : stdin_position_(stdin_position),
          stdout_position_(stdout_position),
          language_(language),
          bin_position_("./Main") {
}

Test::Test(string &stdin_position, string &stdout_position, Language language)
        : stdin_position_(stdin_position),
          stdout_position_(stdout_position),
          language_(language),
          bin_position_("./Main") {
}


//child run
int Test::run() {
    //redirect iostream
    freopen(stdin_position_.c_str(), "r", stdin);
    freopen(output_position_.c_str(), "w", stdout);
    freopen("error.out", "+", stderr);
    //enable PTRACE
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    // set the limit
    struct rlimit LIM; // time limit, file limit& memory limit
    // time limit
    LIM.rlim_cur = time_limit_ / 1000 + 1;
    LIM.rlim_max = LIM.rlim_cur + 1;
    setrlimit(RLIMIT_CPU, &LIM);
    //File Limit
    LIM.rlim_max = STD_F_LIM + STD_MB;
    LIM.rlim_cur = STD_F_LIM;
    setrlimit(RLIMIT_FSIZE, &LIM);
    // proc limit
    LIM.rlim_cur = LIM.rlim_max = 1;
    setrlimit(RLIMIT_NPROC, &LIM);
    // set the stack
    LIM.rlim_cur = STD_MB << 6;
    LIM.rlim_max = STD_MB << 6;
    setrlimit(RLIMIT_STACK, &LIM);
    // set the memory
    LIM.rlim_cur = STD_KB * memory_limit_ / 2 * 3;
    LIM.rlim_max = STD_KB * memory_limit_ * 2;
    setrlimit(RLIMIT_AS, &LIM);

    switch (language_) {
        case cpp:
            execl("./Main", "./Main", (char *) NULL);
            break;
    }
    fflush(stderr);
    return 0;
}

using std::cout;
using std::endl;
using std::max;
//parent run
int Test::watch() {
    int stat = 0;
    //assume the first signal be stop and triggered by PTRACE_TRACEME
    pid_ = waitpid(pid_, &stat, 0);
    cout << "status: " << stat << endl;
    //存疑
    assert(WIFSTOPPED(stat) && (stat >> 8) == PTRACE_EVENT_VFORK_DONE);
    //设置监视信号
    ptrace(PTRACE_SETOPTIONS, pid_, NULL,
           PTRACE_O_EXITKILL |
           PTRACE_O_TRACESYSGOOD |
           PTRACE_O_TRACEEXIT);
    //continue
    ptrace(PTRACE_CONT, pid_, NULL, NULL);

    while (true) {
        //wait until next signal
        pid_ = waitpid(pid_, &stat, 0);
        cout << "*** new status " << stat << endl;
        //read from system
        long int tmp = get_proc_status(pid_, "VmPeak:");
        cout << "Memory Usage in proc file " << tmp << "KB " << endl;
        memory_usage_ = max(memory_usage_, tmp);

        if (WIFEXITED(stat)) {
            cout << "Exited Code: " << WEXITSTATUS(stat) << endl;
            break;
        }

        if (WIFSIGNALED(stat)) {
            int sig = WTERMSIG(stat);
            cout << "Signaled: " << sig << endl;

            if (sig == SIGSEGV) {
                cout << "** Memory Error" << endl;
                result = RE;
                ptrace(PTRACE_KILL, pid_, NULL, NULL);
            } else if (sig == SIGXCPU) {
                cout << "** Time Limit Exceeded" << endl;
                result = TLE;
                ptrace(PTRACE_KILL, pid_, NULL, NULL);
            } else if (sig == SIGKILL) {
                cout << "killed" << endl;
                break;
            }

        }
        if (WIFSTOPPED(stat)) {
            cout << "Stopped" << endl;
            int sig = WSTOPSIG(stat);
            cout << "Stop signal number: " << sig << endl;
            if (sig == SIGSEGV) {
                cout << "segment fault" << endl;
                result = RE;
                ptrace(PTRACE_KILL, pid_, NULL, NULL);

            }
            if (sig == SIGABRT) {
                cout << "aborting call" << endl;
                ptrace(PTRACE_KILL, pid_, NULL, NULL);
            }
            if (sig == SIGXFSZ) {
                cout << "File Size Exceed" << endl;
                result = OLE;
                ptrace(PTRACE_KILL, pid_, NULL, NULL);
            }
            ptrace(PTRACE_CONT, pid_, NULL, NULL);
        }
        if (WIFCONTINUED(stat)) {
            cout << "continued" << endl;
        }

        if ((memory_usage_) > memory_limit_) {
            cout << "Memory Error, Killed" << endl;
            ptrace(PTRACE_KILL, pid_, NULL, NULL);
        }
    }

    rusage ruse;
    getrusage(RUSAGE_CHILDREN, &ruse);
    cout << "Time Usage in miliseconds:" << endl;
    time_usage_ = max(time_usage_, ruse.ru_utime.tv_sec * 1000 + ruse.ru_utime.tv_usec / 1000
                                   + ruse.ru_stime.tv_sec * 1000 + ruse.ru_stime.tv_usec / 1000);
    cout << time_usage_ << endl;

    if (time_usage_ > time_limit_) {
        result = TLE;
    }
    memory_usage_ = max(memory_usage_, ruse.ru_maxrss);
    cout << "Memory Usage in maxrss" << endl;
    cout << ruse.ru_maxrss << endl;
    if (result == AC && memory_usage_ > memory_limit_) {
        result = MLE;
    }
}

//run this to judge succesful output
int Test::judge() {
    FILE *user_out, *std_out;

    user_out = fopen(output_position_.c_str(), "re");
    std_out = fopen(stdout_position_.c_str(), "re");
    if (!user_out) {
        result = RE;
    } else if (!std_out) {
        result = SE;
    } else {
        int c1, c2, line_cnt = 0;
        string line_user, line_std;
        while (result == AC && (c2 = get_next_non_space(std_out)) != EOF) {
            c1 = get_next_non_space(user_out);
            if (c1 != c2) {
                result = WA;
                while (c1 != EOF && c1 != '\n') {
                    line_user.push_back(c1);
                    c1 = get_next_non_space(user_out);
                }
                while (c2 != EOF && c2 != '\n') {
                    line_std.push_back(c2);
                    c2 = get_next_non_space(std_out);
                }
                string tmp = "You get different answer at line ";
                tmp.append(std::to_string(line_cnt));
                tmp.append(": \n >>>>>>>>>>>>> your answer: \n");
                tmp.append(line_user);
                tmp.append("\n <<<<<<<<<<<<< standard answer: \n");
                tmp.append(line_std);
                extra_info_.push_back(
                        std::move(tmp)
                );
                return 0;
            }
            if (c2 == '\n') {
                line_user.clear();
                line_std.clear();
                line_cnt++;
            }
            line_user.push_back(c1);
            line_std.push_back(c2);
        }
        if (result == AC && (c1 = get_next_non_space(user_out)) != EOF) {
            result = WA;
            return 0;
        }
    }

    if (user_out)
        fclose(user_out);
    if (std_out)
        fclose(std_out);
    return 0;
}
int Test::debug() {
    pid_ = fork();
    if (pid_ == 0) {
        run();
    }
    watch();
    judge();
    cout << "Result: " << endl;
    cout << STATE[result] << endl;
    if (result == WA) {
        cout << extra_info_[0] << endl;
    }
}
