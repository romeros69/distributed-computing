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
    
    int count = write(des, msg, 8 + msg->s_header.s_payload_len);
    printf("proc %d write to dst=%d use des=%d data= %s len = %zu\n", gl->id_proc, dst, des, msg->s_payload, msg->s_header.s_payload_len + 8);
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
    //printf("PROC: %d 11111\n", gl->id_proc);
    if (gl->id_proc < from) {
        des = gl->gen[gl->id_proc][from].b_to_s[0];
    } else {
        des = gl->gen[gl->id_proc][from].s_to_b[0];
    }
    //printf("22222\n");
    if (gl->id_proc == PARENT_ID) {
        printf("parent try recieve msg from %d des = %d\n", from, gl->gen[gl->id_proc][from].b_to_s[0]);
    }
    int header_res = read(des, msg, 8);
    printf("--------- proc %d read from %d res read = %d\n", gl->id_proc, from, header_res);

    if (header_res == -1) {
        return -1;
    }
    //printf("333333\n");
    int msg_res = read(des, msg->s_payload, msg->s_header.s_payload_len);
    if (msg_res == -1) {
        return -1;
    }
    //printf("444444\n");
    return 0;
}


// тут j = 1, т к родительский ничего не отправляет
int receive_any(void * self, Message * msg){
    global* gl = (global*) self;
    for (size_t j = 1; j < gl->count_proc; j++) {
        if (j != gl->id_proc) {
            int res_recieve = receive(gl, j, msg);
            if (res_recieve == -1) {
                continue;
            } else {
                return 0;
            }
        }
    }
    return -1;
}

    // global* gl = (global*) self;
    // int schet = 0;
    // int * flags = (int*) malloc(sizeof(int) * gl->count_proc);
    // for (size_t i = 0; i < gl->count_proc; i++) {
    //     flags[i] = 1;
    // }
    // while(1) {
    //     for (size_t i = 0; i < gl->count_proc; i++) {
    //         if (flags[i] == 1 && i != gl->id_proc) {
    //             int res_recieve = receive(gl, i, msg);
    //             if (res_recieve == 0) {
    //                 schet++;
    //                 flags[i] = 0;
    //                 if (schet == gl->count_proc - 1) {
    //                     return 0;
    //                 }
    //             }
    //         } else {
    //             continue;
    //         }
    //     }
    // }
    