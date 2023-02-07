//
// Created by romeros on 26.11.2022.
//

#ifndef DS_LAB_1_LOGGER_H
#define DS_LAB_1_LOGGER_H

void log_start(FILE * events, int id_proc);

void log_res_all_start(FILE * events, int id_proc);

void log_done_work(FILE * events, int id_proc);

void log_res_all_done(FILE * events, int id_proc);

#endif //DS_LAB_1_LOGGER_H
