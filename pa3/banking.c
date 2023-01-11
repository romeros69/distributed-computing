#include "banking.h"
#include "ipc.h"
#include "pipes.h"
#include "message.h"
#include "malloc.h"
#include "logger.h"


void transfer(void * parent_data, local_id src, local_id dst, balance_t amount) {
    global* gl = (global*) parent_data;
    Message* msg = new_transfer_msg(src, dst, amount);
    msg->s_header.s_local_time = 0;
    send(gl, src, msg);
    Message* response_msg = (Message*) malloc(sizeof(Message));
    while (1)
    {
        while(receive(gl, dst, response_msg) == -1) {}
        if (response_msg->s_header.s_type == ACK) {
            break;
        }
    }
}
