#include <stdio.h>
#include "proc.h"
#include "ipc.h"



void run(global* gl, int id_proc) {
    gl->id_proc = id_proc;
    close_nenuzh_pipes(gl, id_proc);
    close_ne_rw_pipes(gl, id_proc);
    Message* msg = new_started_msg(id_proc);
    send_multicast(gl, msg);
    printf("process %d send START msg\n", id_proc);
    receive_any(gl, msg);
    printf("process %d recieve msg: %s\n", id_proc, msg->s_payload);
    close_ok_rw_pipes(gl, id_proc);
}

void run_parent(global* gl, int id_proc) {
    close_nenuzh_pipes(gl, id_proc);
    close_ne_rw_pipes(gl, id_proc);
    Message* msg = NULL;
    receive_any(gl, msg);
    printf("process %d recieve msg: %s\n", id_proc, msg->s_payload);
    close_ok_rw_pipes(gl, id_proc);
}
