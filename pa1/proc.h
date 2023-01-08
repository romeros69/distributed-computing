

#ifndef __ROMCHIK_PROC_H
#define __ROMCHIK_PROC_H
#include <stdio.h>
#include "pipes.h"
#include "message.h"
#include "stdlib.h"

void run(FILE * ev, FILE * ps, global* gl, int id_proc);

void run_parent(FILE * ev, FILE * ps, global* gl, int id_proc);

#endif
