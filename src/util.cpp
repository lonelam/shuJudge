//
// Created by acm on 3/22/18.
//

#include "util.h"

map<string, State> STATE_STR = {
        {"AC",  AC},
        {"PE",  PE},
        {"WA",  WA},
        {"RE",  RE},
        {"TLE", TLE},
        {"MLE", MLE},
        {"OLE", OLE},
        {"CE",  CE}
};

map<string, Language> LANG_STR = {
        {"c",    c},
        {"cpp",  cpp},
        {"java", java},
};

int get_proc_status(int pid, const char *mark) {
    FILE *pf;
    char fn[BUFFER_SIZE], buf[BUFFER_SIZE];
    int ret = 0;
    sprintf(fn, "/proc/%d/status", pid);
    pf = fopen(fn, "re");
    int m = strlen(mark);
    while (pf && fgets(buf, BUFFER_SIZE - 1, pf)) {

        buf[strlen(buf) - 1] = 0;
        if (strncmp(buf, mark, m) == 0) {
            sscanf(buf + m + 1, "%d", &ret);
        }
    }
    if (pf)
        fclose(pf);
    return ret;
}

int get_page_fault_mem(struct rusage ruse, pid_t pidApp) {
    int m_minflt;
    m_minflt = ruse.ru_minflt * getpagesize();
    return (m_minflt >> 10);
}

int get_next_non_space(FILE *fp) {
    int c;
    while ((c = fgetc(fp)) != EOF && isspace(c)) {
    }
    return c;
}


State to_state(const string &st) {
    if (STATE_STR.find(st) != STATE_STR.end()) {
        return STATE_STR[st];
    }
    return SE;
}


Language to_language(const string &lang) {
    if (LANG_STR.find(lang) != LANG_STR.end()) {
        return LANG_STR[lang];
    }
    return cpp;
}