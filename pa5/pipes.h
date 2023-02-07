
#ifndef __ROMCHIK_PIPES_H
#define __ROMCHIK_PIPES_H

#include <unistd.h>
#include <stdio.h>
#include "ipc.h"

static int my_flag = 0;

typedef struct
{
    int s_to_b[2];
    int b_to_s[2]; 
}__attribute__((packed)) chpok;


typedef struct
{
    timestamp_t time;
    int id_proc;
}__attribute__((packed)) my_queue;


typedef struct
{
    chpok** gen;
    size_t count_proc;
    int id_proc;
    timestamp_t time_now;
    my_queue* mq;
    int size_mq;
    int num_from;
    int count_done_msg;
    int mutex_flag;
    int my_dle[11];
    timestamp_t my_metka;
}__attribute__((packed)) global;

global* new_global(size_t count_proc);

void create_pipes(global* gl);

void close_nenuzh_pipes(FILE * ps, global* gl, int cur_id_proc);

void close_ne_rw_pipes(FILE * ps, global* gl, int cur_id_proc);

void close_after_write(FILE * ps, global* gl);

void close_after_read(FILE * ps, global* gl);

my_queue pop(global* gl);

void push(global* gl, my_queue new_element);

void print_mq(global* gl);

#endif
