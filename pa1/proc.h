

#ifndef __ROMCHIK_PROC_H
#define __ROMCHIK_PROC_H

#include "pipes.h"
#include "message.h"

void run(global* gl, int id_proc);

void run_parent(global* gl, int id_proc);

#endif
