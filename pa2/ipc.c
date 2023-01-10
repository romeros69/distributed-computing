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

    if (gl->id_proc == 1 && msg->s_header.s_type == TRANSFER) {
        printf("proc 1 peresend transfer msg dst = %d, des = %d, len = %d\n", dst ,  des, msg->s_header.s_payload_len);
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
    if (gl->id_proc == 2) {
        printf("OP\n");
    }
    int header_res = read(des, &(msg->s_header), sizeof(MessageHeader));
    if (gl->id_proc == 2) {
        printf("OP\n");
    }
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
    if (gl->id_proc == 2) {
        printf("RECEIVE 2 START\n");
    }
    while(1) {
        for (size_t j = 0; j < gl->count_proc; j++) {
            if (gl->id_proc == 2) {
                printf("PROC 2 GO RECIEVE FROM %zu\n", j);
            }
            if (gl->id_proc == j) {
                continue;
            } else {
                int res = receive(self, j, msg);
                if (res == 0) {
                    if (gl->id_proc == 2) {
                        printf("RECEIVE 2 END\n");
                    }
                    return 0;
                }
            }
        }
    }
}

