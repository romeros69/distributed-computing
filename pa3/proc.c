#include <stdio.h>
#include "proc.h"
#include "ipc.h"
#include <stdlib.h>
#include <unistd.h>
#include "format.h"
#include <string.h>
#include <malloc.h>
#include "logger.h"
#include "banking.h"
#include "my_lamport.h"



void run(FILE * ev, FILE * ps, global* gl, int id_proc) {
    gl->time_now = 0;
    // timestamp_t cur_time = get_physical_time();
    //size_t dlina_history = gl->history.s_history_len;
    gl->history.s_history_len = 1;
    gl->history.s_history[0] = (BalanceState) {
    .s_balance = gl->dollar,
    .s_time = gl->time_now,
    .s_balance_pending_in = 0
    };
    // BalanceState balanceState = gl->history.s_history[dlina_history - 1];
    // for (int i = dlina_history; i < cur_time; i++) {
    //     gl->history.s_history[i] = balanceState;
    //     gl->history.s_history[i].s_time = i;
    // }


    close_nenuzh_pipes(ps, gl, gl->id_proc);
    close_ne_rw_pipes(ps,gl, gl->id_proc);
    // отправка всем сообщения о старте
    log_start(gl, ev, id_proc);
    Message* msg = new_started_msg(gl->id_proc);
    // msg->s_header.s_local_time = my_get_lamport_time(gl);
    send_multicast(gl, msg);

    // принятие от всех сообщения, о старте // МОЖЕТ ПОТОМ НАДО ФИКСАНУТЬ
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            while(receive(gl, j, msg) == -1) {}
            if (msg->s_header.s_type != STARTED) {
                j--;
            }
        }
    }   
    log_res_all_start(gl, ev, id_proc);

    // полезная работа
    Message* lov_msg = (Message*) malloc(sizeof(Message));
    TransferOrder* transfer = (TransferOrder*) malloc(sizeof(TransferOrder));
    int count_done = 0;
    int have_stop = 0;
    while (1)
    {
        while(receive_any(gl, lov_msg) == -1) {}
        if (lov_msg->s_header.s_type == TRANSFER) {
            memcpy(transfer, lov_msg->s_payload, lov_msg->s_header.s_payload_len);
            //printf("INFO: child proc %d, revieve transfer msg src = %d, dst = %d\n", gl->id_proc, transfer->s_src, transfer->s_dst);
            if (transfer->s_src == gl->id_proc) {
                gl->time_now = (gl->time_now > lov_msg->s_header.s_local_time) ? gl->time_now : lov_msg->s_header.s_local_time;

                timestamp_t cur_time = my_get_lamport_time(gl);
                gl->dollar = gl->dollar - transfer->s_amount;
                int dlina_history = gl->history.s_history_len;
                gl->history.s_history_len = cur_time + 1;
                gl->history.s_history[cur_time] = (BalanceState) {
                    .s_balance = gl->dollar,
                    .s_time = cur_time,
                    .s_balance_pending_in = 0
                };
                BalanceState balanceState = gl->history.s_history[dlina_history - 1];
                for (int i = dlina_history; i < cur_time; i++) {
                     balanceState.s_balance_pending_in = 0;
                    gl->history.s_history[i] = balanceState;
                    gl->history.s_history[i].s_time = i;
                    gl->history.s_history->s_balance_pending_in = 0;
                }

                Message* msg_to_dst = new_transfer_msg(transfer->s_src, transfer->s_dst, transfer->s_amount);
                msg_to_dst->s_header.s_local_time = my_get_lamport_time(gl);
                log_transfer_send(gl, ev, gl->id_proc, transfer->s_amount, transfer->s_dst);
                send(gl, transfer->s_dst, msg_to_dst);
                lov_msg->s_header.s_type = -1;
            } else if (transfer->s_dst == gl->id_proc) {
                
                timestamp_t receive_time = lov_msg->s_header.s_local_time;
                if (gl->time_now < lov_msg->s_header.s_local_time) {
                    gl->time_now = lov_msg->s_header.s_local_time;
                }
                my_get_lamport_time(gl);
                gl->dollar = gl->dollar + transfer->s_amount;
                int dlina_history = gl->history.s_history_len;
                gl->history.s_history_len = gl->time_now + 1;
                gl->history.s_history[gl->time_now] = (BalanceState) {
                    .s_balance = gl->dollar,
                    .s_time = gl->time_now,
                    .s_balance_pending_in = 0
                };
                BalanceState balanceState = gl->history.s_history[dlina_history - 1];


                // printf("proc %d, balance = %d, time = %d after receive bablo, bablo in balbance %d\n", gl->id_proc, gl->dollar, gl->time_now, gl->history.s_history[cur_time].s_balance);
                for (int i = dlina_history; i < gl->time_now; i++) {
                    gl->history.s_history[i] = balanceState;
                    gl->history.s_history[i].s_time = i;
                }

                for (int i = receive_time - 1; i < gl->time_now; i++) {
                    gl->history.s_history[i].s_balance_pending_in = transfer->s_amount;
                }

                Message* msg_ask_to_parent = new_ack_msg();
                log_transfer_receive(gl, ev, gl->id_proc, transfer->s_amount, transfer->s_src);
                send(gl, PARENT_ID, msg_ask_to_parent);
                lov_msg->s_header.s_type = -1;
            } else {
                printf("!!!ERRROR!!! PROC %d CHILDREN, type msg = %d, src = %d, dst = %d\n", gl->id_proc, lov_msg->s_header.s_type, transfer->s_src, transfer->s_dst);
            }
        } else if (lov_msg->s_header.s_type == STOP) {
            Message* done_msg = new_done_msg(gl->id_proc);
            send_multicast(gl, done_msg);
            log_done_work(gl, ev, id_proc);
            have_stop = 1;
            if (have_stop == 1 && count_done == gl->count_proc - 2) {
                log_res_all_done(gl, ev, id_proc);
                Message* history_msg = new_balance_history(gl);
                history_msg->s_header.s_local_time = gl->time_now;
                send(gl, PARENT_ID, history_msg);
                break;
            }
        } else if (lov_msg->s_header.s_type == DONE){
            count_done++;
            if (have_stop == 1 && count_done == gl->count_proc - 2) {
                log_res_all_done(gl, ev, id_proc);
                Message* history_msg = new_balance_history(gl);
                history_msg->s_header.s_local_time = gl->time_now;
                send(gl, PARENT_ID, history_msg);
                break;
            }
            //printf("proc %d receive msg unknown type = %d\n", gl->id_proc, lov_msg->s_header.s_type);
        } else {
            printf("proc %d res unknown type = %d", gl->id_proc, lov_msg->s_header.s_type);
        }
    }
    close_after_write(ps,gl);      
    close_after_read(ps,gl);
}

void run_parent(FILE * ev, FILE * ps, global* gl, int id_proc) {
    // принимаем сообщение о том, что все стартанули
    close_nenuzh_pipes(ps,gl, gl->id_proc);
    close_ne_rw_pipes(ps,gl, gl->id_proc);
    Message* msg = malloc(sizeof(Message));
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            while(receive(gl, j, msg) == -1) {}
            if (msg->s_header.s_type != STARTED) {
                j--;
            }
        }
    }
    log_res_all_start(gl, ev, gl->id_proc);

    // запускаем переводы
    int MAX_ID = gl->count_proc - 1;
    bank_robbery(gl, MAX_ID);

    // отправляем стоп всем
    Message* msg_stop = new_stop_msg();
    send_multicast(gl, msg_stop);

    
    // // принимаем сообщения о завершении
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            while(receive(gl, j, msg) == -1) {}
            if (msg->s_header.s_type != DONE) {
                printf("PRIVET type = %d", msg->s_header.s_type);
                j--;
            }
        }
    }
    log_res_all_done(gl, ev, gl->id_proc);


    // получаем history от всех дочерних процессов
    AllHistory all_history;
    all_history.s_history_len = 0;
    gl->time_now = 0;
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc && j != PARENT_ID) {
            while(receive(gl, j, msg) == -1) {}
            if (msg->s_header.s_type != BALANCE_HISTORY) {
                j--;
            } else {
                if (gl->time_now < msg->s_header.s_local_time) {
                    gl->time_now = msg->s_header.s_local_time;
                }
                all_history.s_history_len = all_history.s_history_len + 1;

                BalanceHistory history;
                memcpy(&history, msg->s_payload, sizeof(BalanceHistory));
                all_history.s_history[history.s_id - 1] = history;

                for (int k = 0; k < all_history.s_history_len; k++) {
                    BalanceHistory work_history = all_history.s_history[k];
                    BalanceState last_state = work_history.s_history[work_history.s_history_len - 1];
                        int old_history_len = work_history.s_history_len;
                        work_history.s_history_len = gl->time_now + 1;
                        for (int p = old_history_len; p <= gl->time_now; p++) {
                            work_history.s_history[p] = last_state;
                            work_history.s_history[p].s_time = p;
                        }
                    all_history.s_history[work_history.s_id - 1] = work_history;
                }
            }
        }
    }

    print_history(&all_history);

    close_after_write(ps,gl);
    close_after_read(ps,gl);
}
