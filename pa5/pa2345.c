#include "pa2345.h"
#include "pipes.h"
#include "ipc.h"
#include "message.h"
#include "my_lamport.h"
#include "malloc.h"

int request_cs(const void * self) {
    global* gl = (global*) self;
    
    // отправка всем CS_REQUEST
    Message* msg = new_cs_request_msg(gl);
    msg->s_header.s_local_time = my_get_lamport_time(gl);
    gl->my_metka = gl->time_now;
    // push(gl, (my_queue){.id_proc = gl->id_proc, .time = gl->time_now});
    send_multicast(gl, msg);
    
    //printf("%d: PROC %d send multicast REQUEST\n", gl->time_now, gl->id_proc);

    Message* lov_msg = (Message*) malloc(sizeof(Message));
    int count_reply = 0;
    while (1)
    {
        while (receive_any(gl, lov_msg) != 0) {}
        if (lov_msg->s_header.s_type == CS_REPLY) {
            gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
            my_get_lamport_time(gl);
            //printf("%d: PROC %d receive REPLY from %d\n", gl->time_now, gl->id_proc, gl->num_from);

            //if (lov_msg->s_header.s_local_time >= gl->mq[0].time) { // это может быть нужно убрать
                count_reply++;
                if (count_reply == gl->count_proc - 1) {     // ВОТ ТУТ МБ НУЖНО ТАКЖЕ УЧИТЫВАТЬ ПРИЕМ ОТ РОДОКА
                    break;
                }
            //}
        } else if (lov_msg->s_header.s_type == CS_REQUEST) {
            // push(gl, (my_queue){.id_proc = gl->num_from, .time = lov_msg->s_header.s_local_time});
            gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
            my_get_lamport_time(gl);
            //printf("%d: PROC %d receive REQUEST from %d\n", gl->time_now, gl->id_proc, gl->num_from);

            if (gl->my_metka > lov_msg->s_header.s_local_time) {
                Message* reply_msg = new_cs_reply_msg(gl);
                reply_msg->s_header.s_local_time = my_get_lamport_time(gl);
                send(gl, gl->num_from, reply_msg);
            } else if (gl->my_metka < lov_msg->s_header.s_local_time) {
                gl->my_dle[gl->num_from] = 1;
            } else {
                if (gl->id_proc < gl->num_from) {
                    gl->my_dle[gl->num_from] = 1;
                } else {
                    Message* reply_msg = new_cs_reply_msg(gl);
                    reply_msg->s_header.s_local_time = my_get_lamport_time(gl);
                    send(gl, gl->num_from, reply_msg);
                }
            }
            
            //printf("%d: PROC %d send REPLY to %d\n", gl->time_now, gl->id_proc, gl->num_from);
            
        } else if (lov_msg->s_header.s_type == DONE) {
            gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
            my_get_lamport_time(gl);
            gl->count_done_msg++;
            //printf("%d: PROC %d receive DONE msg from %d\n", gl->time_now, gl->id_proc, gl->num_from);
        } else {
            printf("ERROR UNKNOWN TYPE MSG IN request_cs(), type = %d\n", lov_msg->s_header.s_type);
        }
    }
    

    return 0;
}
int release_cs(const void * self) {
    global* gl = (global*) self;
    // pop(gl);
    Message* reply_msg = new_cs_reply_msg(gl);
    for (int i = 0; i < 11; i++) {
        if (gl->my_dle[i] == 1) {
            send(gl, i, reply_msg);
            gl->my_dle[i] = 0;
        }
    }
    // printf("%d: PROC %d send multicast RELEASE \n", gl->time_now, gl->id_proc, gl->num_from);
    return 0;
}
