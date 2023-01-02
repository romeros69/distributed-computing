#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "message.h"

Message* new_started_msg(int id_proc) {
    char *data = (char *) malloc(sizeof(char) * 80);
    sprintf(data, log_started_fmt, id_proc, getpid(), getppid());
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
