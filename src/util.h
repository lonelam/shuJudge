//
// Created by acm on 3/22/18.
//

#ifndef SHUJUDGE_UTIL_H
#define SHUJUDGE_UTIL_H

#include <cstdio>
#include <cstring>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;
enum Language {
    c, cpp, java
};

int get_proc_status(int pid, const char *mark);

#endif //SHUJUDGE_UTIL_H
