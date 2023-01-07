
#ifndef __ROMCHIK_PIPES_H
#define __ROMCHIK_PIPES_H

#include <unistd.h>

typedef struct
{
    int s_to_b[2];
    int b_to_s[2]; 
}__attribute__((packed)) chpok;

typedef struct
{
    chpok** gen;
    size_t count_proc;
    int id_proc;
}__attribute__((packed)) global;

global* new_global(size_t count_proc);

void create_pipes(global* gl);

void close_nenuzh_pipes(global* gl, int cur_id_proc);

void close_ne_rw_pipes(global* gl, int cur_id_proc);

void close_after_write(global* gl);

void close_after_read(global* gl);

#endif
