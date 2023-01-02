#include <malloc.h>
#include "ipc.h"
#include "pipes.h"

int send(void * self, local_id dst, const Message * msg){
    global* gl = (global*) self;
    int des;
    if (gl->id_proc < dst) {
        des = gl->gen[gl->id_proc][dst].s_to_b[1];
    } else {
        des = gl->gen[gl->id_proc][dst].b_to_s[1];
    }
    write(des, msg, 8 + msg->s_header.s_payload_len);
    return 0;
}


int send_multicast(void * self, const Message * msg){
    global* gl = (global*) self;
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc) {
            send(gl, j, msg);
        }
    }
    return 0;
}


int receive(void * self, local_id from, Message * msg){
    global* gl = (global*) self;
    int des;
    if (gl->id_proc < from) {
        des = gl->gen[gl->id_proc][from].b_to_s[0];
    } else {
        des = gl->gen[gl->id_proc][from].s_to_b[0];
    }
    int header_res = read(des, msg, 8);
    if (header_res == -1) {
        return -1;
    }
    int msg_res = read(des, msg->s_payload, msg->s_header.s_payload_len);
    if (msg_res == -1) {
        return -1;
    }

    return 0;
}


int receive_any(void * self, Message * msg){
    global* gl = (global*) self;
    int schet = 0;
    int * flags = (int*) malloc(sizeof(int) * gl->count_proc);
    for (size_t i = 0; i < gl->count_proc; i++) {
        flags[i] = 1;
    }
    while(1) {
        for (size_t i = 0; i < gl->count_proc; i++) {
            if (flags[i] == 1 && i != gl->id_proc) {
                int res_recieve = receive(gl, i, msg);
                if (res_recieve == 0) {
                    schet++;
                    flags[i] = 0;
                    if (schet == gl->count_proc - 1) {
                        return 0;
                    }
                }
            } else {
                continue;
            }
        }
    }
}
