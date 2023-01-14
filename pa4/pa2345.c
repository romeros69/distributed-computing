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
    push(gl, (my_queue){.id_proc = gl->id_proc, .time = gl->time_now});
    send_multicast(gl, msg);

    Message* lov_msg = (Message*) malloc(sizeof(Message));
    int count_reply = 0;
    while (1)
    {
        while (receive_any(gl, lov_msg) != 0) {}
        if (lov_msg->s_header.s_type == CS_REPLY) {
            gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
            my_get_lamport_time(gl);
            if (lov_msg->s_header.s_local_time >= gl->time_now) {
                count_reply++;
                if (count_reply == gl->count_proc - 2 && gl->mq[0].id_proc == gl->id_proc) {     // ВОТ ТУТ МБ НУЖНО ТАКЖЕ УЧИТЫВАТЬ ПРИЕМ ОТ РОДОКА
                    break;
                }
            }
        } else if (lov_msg->s_header.s_type == CS_REQUEST) {
            push(gl, (my_queue){.id_proc = gl->num_from, .time = lov_msg->s_header.s_local_time});
            gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
            my_get_lamport_time(gl);


            Message* reply_msg = new_cs_reply_msg(gl);
            reply_msg->s_header.s_local_time = my_get_lamport_time(gl);
            send(gl, gl->num_from, reply_msg);
            
        } else if (lov_msg->s_header.s_type == CS_RELEASE) {
            gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
            my_get_lamport_time(gl);
            pop(gl);
        } else {
            printf("ERROR UNKNOWN TYPE MSG IN request_cs(), type = %d\n", lov_msg->s_header.s_type);
        }
    }
    

    return 0;
}
int release_cs(const void * self) {
    global* gl = (global*) self;
    pop(gl);
    Message* release_msg = new_cs_release_msg(gl);
    release_msg->s_header.s_local_time = my_get_lamport_time(gl);
    send_multicast(gl, release_msg);
    return 0;
}
