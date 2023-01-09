//
// Created by romeros on 26.11.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "logger.h"
#include "pa1.h"
#include "common.h"

void log_start(FILE * events, int id_proc) {
    int current_pid = getpid();
    int parent_pid = getppid();
    printf(log_started_fmt, id_proc, current_pid, parent_pid);
    fprintf(events, log_started_fmt, id_proc, current_pid, parent_pid);
}

void log_res_all_start(FILE * events, int id_proc) {
    printf(log_received_all_started_fmt, id_proc);
    fprintf(events, log_received_all_started_fmt, id_proc);
}

void log_done_work(FILE * events, int id_proc) {
    printf(log_done_fmt, id_proc);
    fprintf(events, log_done_fmt, id_proc);
}

void log_res_all_done(FILE * events, int id_proc) {
    printf(log_received_all_done_fmt, id_proc);
    fprintf(events, log_received_all_done_fmt, id_proc);
}
