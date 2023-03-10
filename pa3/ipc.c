#include "ipc.h"
#include "pipes.h"
#include "format.h"

int send(void * self, local_id dst, const Message * msg){
    global* gl = (global*) self;
    int des;
    if (gl->id_proc < dst) {
        des = gl->gen[gl->id_proc][dst].s_to_b[1];
    } else {
        des = gl->gen[gl->id_proc][dst].b_to_s[1];
    }
    
    write(des, msg, sizeof(MessageHeader) + msg->s_header.s_payload_len);
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
    int header_res = read(des, &(msg->s_header), 8);
    if (header_res == -1 || header_res == 0) {
        return -1;
    }
    // if (msg->s_header.s_type == DONE && from == 5) {
    //     printf("proc %d receive msg from %d header_type = %d, payload_len = %d\n", gl->id_proc, from, msg->s_header.s_type, msg->s_header.s_payload_len);
    // }
    int msg_res = read(des, msg->s_payload, msg->s_header.s_payload_len);
    if (header_res == -1 || msg_res == -1) {
        return -1;
    }
    // if (msg->s_header.s_type == DONE) {
    //     printf("SUKA proc %d header_len %d msg_len %d \n", gl->id_proc, header_res, msg_res);
    // }
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

