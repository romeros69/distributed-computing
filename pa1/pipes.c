#include <stdlib.h>
#include <malloc.h>
#include "pipes.h"

// Конструктор для создания двумерного массива для будущих пайпов
// count_proc - count of all process in system
chpok** new_gen(size_t count_proc) {
    chpok** gen = (chpok**) malloc(sizeof(chpok*) * count_proc);
    for (size_t i = 0; i < count_proc; i++) {
        gen[i] = (chpok*) malloc(sizeof(chpok) * count_proc);
    }
    return gen;
}

// Конструктор для создания главной структуры с пайпами и количетсвом процессов в системе
global* new_global(size_t count_proc) {
    chpok** gen = new_gen(count_proc);
    global* gl = (global*) malloc(sizeof(global));
    gl->count_proc = count_proc;
    gl->gen = gen;
    return gl;
}

// Функция создания пайпов
void create_pipes(global* gl) {
    for (size_t i = 0; i < gl->count_proc; i++) {
        for (size_t j = 0; j < gl->count_proc; j++) {
            if (i != j) {
                if (j > i) {
                    int err = pipe(gl->gen[i][j].s_to_b);
                    if (err != 0) {
                        printf("ERROR: creating pipe s_to_b");
                    }
                    err = pipe(gl->gen[i][j].b_to_s);
                    if (err != 0) {
                        printf("ERROR: creating pipe b_to_s");
                    }
                } else {
                    gl->gen[i][j] = gl->gen[j][i];
                }
            }
        }
    }
}

// закрывает все дескрипторы в chpok-е
void close_all_chpok(chpok ch) {
    close(ch.s_to_b[0]);
    close(ch.s_to_b[1]);
    close(ch.b_to_s[0]);
    close(ch.b_to_s[1]);
}

// закрывает все пайпы не относящиеся никак к текущему процессу - проверка №1 - ок
void close_nenuzh_pipes(global* gl, int cur_id_proc) {
    for (size_t i = 0; i < gl->count_proc; i++) {
        for (size_t j = i; j < gl->count_proc; j++) {
            if (i != cur_id_proc && j != cur_id_proc && i != j) {
                close_all_chpok(gl->gen[i][j]);
            }
            
        }
    }
}


// в будущем может быть такое, что эти функции надо будет разделить на r и w !!!
// Закрывает ненужные пайпы чтения и запимси, в нужных chpok-ах --- ДО РАБОТЫ ПАЙПОВ - ok 1
void close_ne_rw_pipes(global* gl, int cur_id_proc) {
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != cur_id_proc) {
            if (j > cur_id_proc) {
                close(gl->gen[cur_id_proc][j].s_to_b[0]);
                close(gl->gen[cur_id_proc][j].b_to_s[1]);
            } else {
                close(gl->gen[cur_id_proc][j].s_to_b[1]);
                close(gl->gen[cur_id_proc][j].b_to_s[0]);
            }
        }
    }
}

// Закрывает НУЖНЫЕ пайпы чтения и запимси, в нужных chpok-ах --- ПОСЛЕ РАБОТЫ ПАЙПОВ - ok 1
void close_ok_rw_pipes(global* gl, int cur_id_proc) {
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != cur_id_proc) {
            if (j > cur_id_proc) {
                close(gl->gen[cur_id_proc][j].s_to_b[1]);
                close(gl->gen[cur_id_proc][j].b_to_s[0]);
            } else {
                close(gl->gen[cur_id_proc][j].s_to_b[0]);
                close(gl->gen[cur_id_proc][j].b_to_s[1]);
            }
        }
    }
}
