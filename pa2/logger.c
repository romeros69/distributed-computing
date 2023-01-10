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

void log_start(FILE * events, int id_proc) {
    int current_pid = getpid();
    int parent_pid = getppid();
    int cur_balance = 10;
    printf(log_started_fmt, get_physical_time(), id_proc, current_pid, parent_pid, cur_balance);
    fprintf(events, log_started_fmt, get_physical_time(), id_proc, current_pid, parent_pid, cur_balance );
}

void log_res_all_start(FILE * events, int id_proc) {
    printf(log_received_all_started_fmt, get_physical_time(), id_proc);
    fprintf(events, log_received_all_started_fmt, get_physical_time(), id_proc);
}

void log_done_work(FILE * events, int id_proc) {
    int cur_balance = 10;
    printf(log_done_fmt, get_physical_time(), id_proc, cur_balance);
    fprintf(events, log_done_fmt, get_physical_time(), id_proc, cur_balance);
}

void log_res_all_done(FILE * events, int id_proc) {
    printf(log_received_all_done_fmt, get_physical_time(), id_proc);
    fprintf(events, log_received_all_done_fmt, get_physical_time(), id_proc);
}


void log_transfer_send(FILE * events, int id_proc, int amount, int dst) {
    printf(log_transfer_out_fmt, get_physical_time(), id_proc, amount, dst);
    fprintf(events, log_transfer_out_fmt, get_physical_time(), id_proc, amount, dst);
}

void log_transfer_receive(FILE * events, int id_proc, int amount, int from) {
    printf(log_transfer_in_fmt, get_physical_time(), id_proc, amount, from);
    fprintf(events, log_transfer_in_fmt, get_physical_time(), id_proc, amount, from);
}
