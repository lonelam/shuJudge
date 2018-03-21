//
// Created by acm on 3/22/18.
//

#include "util.h"

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
