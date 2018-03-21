//
// Created by acm on 3/22/18.
//

#include "Test.h"
#include <sys/ptrace.h>


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
    LIM.rlim_cur = time_limit_;
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
    LIM.rlim_cur = STD_MB * memory_limit_ / 2 * 3;
    LIM.rlim_max = STD_MB * memory_limit_ * 2;
    setrlimit(RLIMIT_AS, &LIM);

    switch (language_) {
        case cpp:
            execl("./Main", "./Main", (char *) NULL);
            break;
    }
    fflush(stderr);
    return 0;
}

//parent run
int Test::watch() {

}

int Test::debug() {
    pid_ = fork();
    if (pid_ == 0) {
        run();
    }
    watch();
}
