//
// Created by romeros on 26.11.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "logger.h"
#include "pa2345.h"
#include "common.h"
#include "banking.h"
#include "pipes.h"

void log_start(global* gl ,FILE * events, int id_proc) {
    int current_pid = getpid();
    int parent_pid = getppid();
    printf(log_started_fmt, gl->time_now, id_proc, current_pid, parent_pid, gl->dollar);
    fprintf(events, log_started_fmt, gl->time_now, id_proc, current_pid, parent_pid, gl->dollar );
}

void log_res_all_start(global* gl ,FILE * events, int id_proc) {
    printf(log_received_all_started_fmt, gl->time_now, id_proc);
    fprintf(events, log_received_all_started_fmt, gl->time_now, id_proc);
}

void log_done_work(global* gl ,FILE * events, int id_proc) {
    printf(log_done_fmt, gl->time_now, id_proc, gl->dollar);
    fprintf(events, log_done_fmt, gl->time_now, id_proc, gl->dollar);
}

void log_res_all_done(global* gl ,FILE * events, int id_proc) {
    printf(log_received_all_done_fmt, gl->time_now, id_proc);
    fprintf(events, log_received_all_done_fmt, gl->time_now, id_proc);
}


void log_transfer_send(global* gl ,FILE * events, int id_proc, int amount, int dst) {
    printf(log_transfer_out_fmt, gl->time_now, id_proc, amount, dst);
    fprintf(events, log_transfer_out_fmt, gl->time_now, id_proc, amount, dst);
}

void log_transfer_receive(global* gl ,FILE * events, int id_proc, int amount, int from) {
    printf(log_transfer_in_fmt, gl->time_now, id_proc, amount, from);
    fprintf(events, log_transfer_in_fmt, gl->time_now, id_proc, amount, from);
}
