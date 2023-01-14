#include <stdio.h>
#include "proc.h"
#include "ipc.h"
#include "logger.h"



void run(FILE * ev, FILE * ps, global* gl, int id_proc) {
    close_nenuzh_pipes(ps, gl, gl->id_proc);
    close_ne_rw_pipes(ps,gl, gl->id_proc);
    log_start(gl, ev, id_proc);
    Message* msg = new_started_msg(gl, gl->id_proc);
    send_multicast(gl, msg);
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            receive(gl, j, msg);
//            printf("proc %d recieve msg from %zu msg = %s\n", gl->id_proc, j, msg->s_payload);
        }
    }
    log_res_all_start(gl, ev, id_proc);
    log_done_work(gl, ev, id_proc);
    msg = new_done_msg(gl, gl->id_proc);
    send_multicast(gl, msg);
    close_after_write(ps,gl);
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            receive(gl, j, msg);
//            printf("proc %d recieve msg from %zu msg = %s\n", gl->id_proc, j, msg->s_payload);
        }
    }
    close_after_read(ps,gl);
    log_res_all_done(gl, ev, id_proc);
}

void run_parent(FILE * ev, FILE * ps, global* gl, int id_proc) {
    close_nenuzh_pipes(ps,gl, gl->id_proc);
    close_ne_rw_pipes(ps,gl, gl->id_proc);
    Message* msg = malloc(sizeof(Message));
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            receive(gl, j, msg);
//            printf("proc %d recieve msg from %zu msg = %s\n", gl->id_proc, j, msg->s_payload);
        }
    }
    log_res_all_start(gl, ev, gl->id_proc);
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            receive(gl, j, msg);
//            printf("proc %d recieve msg from %zu msg = %s\n", gl->id_proc, j, msg->s_payload);
        }
    }
    log_res_all_done(gl, ev, gl->id_proc);
    close_after_write(ps,gl);
    close_after_read(ps,gl);
}
