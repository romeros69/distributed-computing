//
// Created by romeros on 26.11.2022.
//

#ifndef DS_LAB_1_LOGGER_H
#define DS_LAB_1_LOGGER_H

#include "pipes.h"

void log_start(global* gl ,FILE * events, int id_proc);

void log_res_all_start(global* gl ,FILE * events, int id_proc);

void log_done_work(global* gl ,FILE * events, int id_proc);

void log_res_all_done(global* gl ,FILE * events, int id_proc);

#endif //DS_LAB_1_LOGGER_
