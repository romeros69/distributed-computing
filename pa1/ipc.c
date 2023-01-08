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
    //printf("proc %d write to dst=%d use des=%d data= %s len = %zu\n", gl->id_proc, dst, des, msg->s_payload, msg->s_header.s_payload_len + 8);
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
    if (gl->id_proc == from) {
        return -1;
    }
    int header_res = read(des, &(msg->s_header), sizeof(MessageHeader));
    
    if (header_res == -1) {
        return -1;
    }
    int msg_res = read(des, msg->s_payload, msg->s_header.s_payload_len);
    if (header_res == -1 || msg_res == -1) {
        return -1;
    }
    return 0;
}


// тут j = 1, т к родительский ничего не отправляет
int receive_any(void * self, Message * msg){
    global* gl = (global*) self;
    while(1) {
        for (size_t j = 0; j < gl->count_proc; j++) {
            if (gl->id_proc == j) {
                continue;
            } else {
                int res = receive(self, j, msg);
                if (res == 0) {
                    return 0;
                }
            }
        }
    }
}

