//
// Created by romeros on 26.11.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "logger.h"
#include "pa2345.h"
#include "common.h"
#include "pipes.h"

void log_start(global* gl ,FILE * events, int id_proc) {
    int current_pid = getpid();
    int parent_pid = getppid();
    printf(log_started_fmt, gl->time_now, id_proc, current_pid, parent_pid, 0);
    fprintf(events, log_started_fmt, gl->time_now, id_proc, current_pid, parent_pid, 0);
}

void log_res_all_start(global* gl ,FILE * events, int id_proc) {
    printf(log_received_all_started_fmt, gl->time_now, id_proc);
    fprintf(events, log_received_all_started_fmt, gl->time_now, id_proc);
}

void log_done_work(global* gl ,FILE * events, int id_proc) {
    printf(log_done_fmt, gl->time_now, id_proc, 0);
    fprintf(events, log_done_fmt, gl->time_now, id_proc, 0);
}

void log_res_all_done(global* gl ,FILE * events, int id_proc) {
    printf(log_received_all_done_fmt, gl->time_now, id_proc);
    fprintf(events, log_received_all_done_fmt, gl->time_now, id_proc);
}

