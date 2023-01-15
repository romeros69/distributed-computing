#include <stdio.h>
#include "proc.h"
#include "ipc.h"
#include "logger.h"
#include "my_lamport.h"
#include "pa2345.h"



void run(FILE * ev, FILE * ps, global* gl, int id_proc) {
    gl->count_done_msg = 0;
    gl->mq = (my_queue*) malloc(sizeof(my_queue) * 9);
    gl->size_mq = 0;
    gl->id_proc = id_proc;
    gl->time_now = 0;
    close_nenuzh_pipes(ps, gl, gl->id_proc);
    close_ne_rw_pipes(ps,gl, gl->id_proc);
    log_start(gl, ev, id_proc);
    Message* msg = new_started_msg(gl, gl->id_proc);
    msg->s_header.s_local_time = my_get_lamport_time(gl);
    send_multicast(gl, msg);
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            while(receive(gl, j, msg) == -1) {}
            if (msg->s_header.s_type != STARTED) {
                j--;
            } else {
                gl->time_now = (gl->time_now > msg->s_header.s_local_time) ? gl->time_now : msg->s_header.s_local_time;
                my_get_lamport_time(gl);
            }
        }
    }   
    log_res_all_start(gl, ev, id_proc);
    // полезная работа

    char data[200];
    for (int i = 1; i <= gl->id_proc * 5; i++) {
        if (gl->mutex_flag == 1) {
            request_cs(gl);
        }
        
        sprintf(data, log_loop_operation_fmt, gl->id_proc, i, gl->id_proc*5);
        print(data);
        
        if (gl->mutex_flag == 1) {
            release_cs(gl);
        }
    }

    //free(data);


    log_done_work(gl, ev, id_proc);
    msg = new_done_msg(gl, gl->id_proc);
    msg->s_header.s_local_time = my_get_lamport_time(gl);
    send_multicast(gl, msg);

    if (gl->count_done_msg == gl->count_proc -2) {

    } else {
        Message* lov_msg = (Message*) malloc(sizeof(Message));
        while (1)
        {
            while(receive_any(gl, lov_msg) != 0) {}
            if (lov_msg->s_header.s_type == DONE) {
                gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
                my_get_lamport_time(gl);
                //printf("%d: PROC %d receive DONE msg from %d\n", gl->time_now, gl->id_proc, gl->num_from);
                gl->count_done_msg++;
                if (gl->count_done_msg == gl->count_proc - 2) {
                    break;
                }
            } else if (lov_msg->s_header.s_type == CS_REQUEST) {
                gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;
                my_get_lamport_time(gl);
                //printf("%d: PROC %d receive REQUEST from %d\n", gl->time_now, gl->id_proc, gl->num_from);
                Message* reply_msg = new_cs_reply_msg(gl);
                reply_msg->s_header.s_local_time = my_get_lamport_time(gl);
                send(gl, gl->num_from, reply_msg);
                //printf("%d: PROC %d send REPLY to %d\n", gl->time_now, gl->id_proc, gl->num_from);
            }
        }
    }
    
    close_after_write(ps,gl);  
    close_after_read(ps,gl);
    log_res_all_done(gl, ev, id_proc);
}

void run_parent(FILE * ev, FILE * ps, global* gl, int id_proc) {
    gl->time_now = 0;
    gl->count_done_msg = 0;
    gl->id_proc = 0;
    close_nenuzh_pipes(ps,gl, gl->id_proc);
    close_ne_rw_pipes(ps,gl, gl->id_proc);
    Message* msg = malloc(sizeof(Message));
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            while(receive(gl, j, msg) == -1) {}
            if (msg->s_header.s_type != STARTED) {
                j--;
            } else {
                gl->time_now = (gl->time_now > msg->s_header.s_local_time) ? gl->time_now : msg->s_header.s_local_time;
                my_get_lamport_time(gl);
            }
        }
    }
    log_res_all_start(gl, ev, gl->id_proc);

    // полезная работа






    Message* receive_done_msg = (Message*) malloc(sizeof(Message));
    while (1)
    {
        while (receive_any(gl, receive_done_msg) == -1) {}
        if (receive_done_msg->s_header.s_type == DONE) {
            gl->time_now = (gl->time_now > receive_done_msg->s_header.s_local_time) ? gl->time_now : receive_done_msg->s_header.s_local_time;
            my_get_lamport_time(gl);
            //printf("%d: proc %d receive DONE msg from %d\n", gl->time_now, gl->id_proc, gl->num_from);
            gl->count_done_msg++;
            if (gl->count_done_msg == gl->count_proc -1) {
                break;
            }
        } else if (receive_done_msg->s_header.s_type == CS_REQUEST) {
            gl->time_now = (gl->time_now > receive_done_msg->s_header.s_local_time) ? gl->time_now : receive_done_msg->s_header.s_local_time;
            my_get_lamport_time(gl);
            Message* reply_msg = new_cs_reply_msg(gl);
            reply_msg->s_header.s_local_time = my_get_lamport_time(gl);
            send(gl, gl->num_from, reply_msg);
            //printf("%d: PROC %d send REPLY to %d\n", gl->time_now, gl->id_proc, gl->num_from);
        }
        // мб добавить ответ от родителя реплай
    }
    
    log_res_all_done(gl, ev, gl->id_proc);
    close_after_write(ps,gl);
    close_after_read(ps,gl);
}
