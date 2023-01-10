#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mm_malloc.h>
#include "message.h"
#include "banking.h"
#include "ipc.h"

Message* new_started_msg(int id_proc) {
    char *data = (char *) malloc(sizeof(char) * 80);
    int time = 1;
    int current_balance = 10;
    sprintf(data, log_started_fmt, time, id_proc, getpid(), getppid(), current_balance);
    size_t len_str = strlen(data);
    MessageHeader* header = (MessageHeader*) malloc(sizeof(MessageHeader));
    header->s_magic = MESSAGE_MAGIC;
    header->s_payload_len = len_str;
    header->s_type = STARTED;
    header->s_local_time = get_physical_time();
    Message* msg = (Message*) malloc(sizeof(Message));
    msg->s_header = *header;
    for (size_t i = 0; i < len_str; i++) {
        msg->s_payload[i] = data[i];
    }
    return msg;
}

Message* new_done_msg(int id_proc) {
    char *data = (char *) malloc(sizeof(char) * 80);
    int time = 1;
    int current_balance = 10;
    sprintf(data, log_done_fmt, time, id_proc, current_balance);
    size_t len_str = strlen(data);
    MessageHeader* header = (MessageHeader*) malloc(sizeof(MessageHeader));
    header->s_magic = MESSAGE_MAGIC;
    header->s_payload_len = len_str;
    header->s_type = DONE;
    header->s_local_time = get_physical_time();
    Message* msg = (Message*) malloc(sizeof(Message));
    msg->s_header = *header;
    for (size_t i = 0; i < len_str; i++) {
        msg->s_payload[i] = data[i];
    }
    return msg;
}

Message* new_transfer_msg(local_id src, local_id dst, balance_t amount) {
    TransferOrder transfer;
    transfer.s_amount = amount;
    transfer.s_dst = dst;
    transfer.s_src = src;
    MessageHeader* header = (MessageHeader*) malloc(sizeof(MessageHeader));
    header->s_magic = MESSAGE_MAGIC;
    header->s_type = TRANSFER;
    header->s_local_time = get_physical_time();
    header->s_payload_len = sizeof(transfer);
    Message* msg = (Message*) malloc(sizeof(Message));
    msg->s_header = *header;
    memcpy(msg->s_payload, &transfer, sizeof(transfer));
    return msg;
}

Message* new_ack_msg() {
    MessageHeader* header = (MessageHeader*) malloc(sizeof(MessageHeader));
    header->s_magic = MESSAGE_MAGIC;
    header->s_payload_len = 0;
    header->s_type = ACK;
    header->s_local_time = get_physical_time();
    Message* msg = (Message*) malloc(sizeof(Message));
    msg->s_header = *header;
    return msg;
}

Message* new_stop_msg() {
    MessageHeader* header = (MessageHeader*) malloc(sizeof(MessageHeader));
    header->s_magic = MESSAGE_MAGIC;
    header->s_payload_len = 0;
    header->s_type = ACK;
    header->s_local_time = get_physical_time();
    Message* msg = (Message*) malloc(sizeof(Message));
    msg->s_header = *header;
    return msg;
}

