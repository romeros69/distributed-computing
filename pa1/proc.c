#include <stdio.h>
#include "proc.h"
#include "ipc.h"
#include "format.h"



void run(global* gl, int id_proc) {
    close_nenuzh_pipes(gl, gl->id_proc);
    close_ne_rw_pipes(gl, gl->id_proc);
    Message* msg = new_started_msg(gl->id_proc);
    send_multicast(gl, msg);
    close_after_write(gl);
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            receive(gl, j, msg);
            printf("proc %d recieve msg from %zu msg = %s\n", gl->id_proc, j, msg->s_payload);
        }
    }
    close_after_read(gl);
}

void run_parent(global* gl, int id_proc) {
    close_nenuzh_pipes(gl, gl->id_proc);
    close_ne_rw_pipes(gl, gl->id_proc);
    close_after_write(gl);
    Message* msg = malloc(sizeof(Message));
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            receive(gl, j, msg);
            printf("proc %d recieve msg from %d msg = %s\n", gl->id_proc, j, msg->s_payload);
        }
    }
    close_after_read(gl);
}
