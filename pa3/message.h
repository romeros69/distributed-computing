

#ifndef __ROMCHIK_MESSAGE_H
#define __ROMCHIK_MESSAGE_H
#include "ipc.h"
#include "pa2345.h"
#include "banking.h"
#include "pipes.h"

Message* new_started_msg(int id_proc);

Message* new_done_msg(global* gl, int id_proc);

Message* new_transfer_msg(local_id src, local_id dst, balance_t amount);

Message* new_ack_msg();

Message* new_stop_msg();

Message* new_balance_history(global* gl);

#endif
