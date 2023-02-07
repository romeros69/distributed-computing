

#ifndef __ROMCHIK_MESSAGE_H
#define __ROMCHIK_MESSAGE_H
#include "ipc.h"
#include "pa2345.h"
#include "pipes.h"

Message* new_started_msg(global* gl, int id_proc);

Message* new_done_msg(global* gl, int id_proc);

Message* new_cs_request_msg(global* gl);

Message* new_cs_reply_msg(global* gl);

Message* new_cs_release_msg(global* gl);

#endif
