//
// Created by acm on 3/22/18.
//

#ifndef SHUJUDGE_UTIL_H
#define SHUJUDGE_UTIL_H

#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <experimental/filesystem>
#include <map>


const int BUFFER_SIZE = 1024;
enum Language {
    c, cpp, java
};

enum State {
    AC, PE, WA, RE, TLE, MLE, OLE, CE, SE
};

using std::string;
using std::vector;
using std::map;

const string STATE[] = {
        "AC", "PE", "WA", "RE", "TLE", "MLE", "OLE", "CE", "SE"
};

int get_proc_status(int pid, const char *mark);

int get_next_non_space(FILE *fp);

vector<string> get_std_filelist(string path);
#endif //SHUJUDGE_UTIL_H
