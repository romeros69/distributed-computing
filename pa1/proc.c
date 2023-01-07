#include <stdio.h>
#include "proc.h"
#include "ipc.h"
#include "format.h"



void run(global* gl, int id_proc) {
    //gl->id_proc = id_proc;
    //close_nenuzh_pipes(gl, id_proc);
    //close_ne_rw_pipes(gl, id_proc);
    Message* msg = new_started_msg(id_proc);
    close(4);
    close(5);
    send_multicast(gl, msg);
    close(6);
    // if (gl->id_proc != 2) {
    //     printf("PEZDECCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC!!!!\n");
    // }
    //close_after_write(gl);
    printf("process %d send START msg\n", id_proc);
    int schet = 0;
    while (1) {
        if (schet == gl->count_proc - 2) {
            break;
        }
        if (receive_any(gl, msg) == 0) {
            schet++;
            printf("process %d recieve msg: %s\n", id_proc, msg->s_payload);
        }
    }
    close_after_read(gl);
    close(3);
}

void run_parent(global* gl, int id_proc) {
    print_global_pipes(gl, PARENT_ID);
    close(4);
    close(3);
    close(6);
    gl->id_proc = PARENT_ID;
    //close_nenuzh_pipes(gl, id_proc);
    //close_ne_rw_pipes(gl, id_proc);
    //close_after_write(gl);
    Message* msg = NULL;
    int schet = 0;
    while (1) {
        if (schet == gl->count_proc - 1) {
            break;
        }
        if (receive_any(gl, msg) == 0) {
            schet++;
            printf("process %d recieve msg: %s\n", id_proc, msg->s_payload);
        }
    }
    // close(5);
    // close_after_read(gl);
}
