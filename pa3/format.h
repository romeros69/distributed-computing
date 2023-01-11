

#ifndef __ROMCHIK_FORMAT_H
#define __ROMCHIK_FORMAT_H

#include "stdlib.h"
#include "pipes.h"

void  print_global_pipes(global* gl, int cur_id_proc);

void  print_gen_pipes(size_t count_proc, chpok** gen);

#endif
