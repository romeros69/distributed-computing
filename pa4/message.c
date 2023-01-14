#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mm_malloc.h>
#include "message.h"


Message* new_started_msg(global* gl, int id_proc) {
    char *data = (char *) malloc(sizeof(char) * 80);
    sprintf(data, log_started_fmt, gl->time_now, id_proc, getpid(), getppid(), 0);
    size_t len_str = strlen(data);
    MessageHeader* header = (MessageHeader*) malloc(sizeof(MessageHeader));
    header->s_magic = MESSAGE_MAGIC;
    header->s_payload_len = len_str;
    header->s_type = STARTED;
    Message* msg = (Message*) malloc(sizeof(Message));
    msg->s_header = *header;
    for (size_t i = 0; i < len_str; i++) {
        msg->s_payload[i] = data[i];
    }
    return msg;
}

Message* new_done_msg(global* gl, int id_proc) {
    char *data = (char *) malloc(sizeof(char) * 80);
    sprintf(data, log_done_fmt, gl->time_now, id_proc, 0);
    size_t len_str = strlen(data);
    MessageHeader* header = (MessageHeader*) malloc(sizeof(MessageHeader));
    header->s_magic = MESSAGE_MAGIC;
    header->s_payload_len = len_str;
    header->s_type = DONE;
    Message* msg = (Message*) malloc(sizeof(Message));
    msg->s_header = *header;
    for (size_t i = 0; i < len_str; i++) {
        msg->s_payload[i] = data[i];
    }
    return msg;
}


