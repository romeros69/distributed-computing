#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "mm_malloc.h"
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
                    fcntl(gl->gen[i][j].s_to_b[0], F_SETFL, fcntl(gl->gen[i][j].s_to_b[0], F_GETFL, 0) | O_NONBLOCK);
                    fcntl(gl->gen[i][j].s_to_b[1], F_SETFL, fcntl(gl->gen[i][j].s_to_b[1], F_GETFL, 0) | O_NONBLOCK);
                    if (err != 0) {
//                        printf("ERROR: creating pipe s_to_b");
                    }
                    err = pipe(gl->gen[i][j].b_to_s);
                    fcntl(gl->gen[i][j].b_to_s[0], F_SETFL, fcntl(gl->gen[i][j].b_to_s[0], F_GETFL, 0) | O_NONBLOCK);
                    fcntl(gl->gen[i][j].b_to_s[1], F_SETFL, fcntl(gl->gen[i][j].b_to_s[1], F_GETFL, 0) | O_NONBLOCK);
                    if (err != 0) {
//                        printf("ERROR: creating pipe b_to_s");
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
void close_nenuzh_pipes(FILE * pipes, global* gl, int cur_id_proc) {
    for (size_t i = 0; i < gl->count_proc; i++) {
        for (size_t j = i; j < gl->count_proc; j++) {
            if (i != cur_id_proc && j != cur_id_proc && i != j) {
                close_all_chpok(gl->gen[i][j]);
                fprintf(pipes, "INFO_PIPES: type=nenuzhn, proc %d close %d\n", gl->id_proc, gl->gen[i][j].s_to_b[0]);
                fprintf(pipes, "INFO_PIPES: type=nenuzhn, proc %d close %d\n", gl->id_proc, gl->gen[i][j].s_to_b[1]);
                fprintf(pipes, "INFO_PIPES: type=nenuzhn, proc %d close %d\n", gl->id_proc, gl->gen[i][j].b_to_s[0]);
                fprintf(pipes, "INFO_PIPES: type=nenuzhn, proc %d close %d\n", gl->id_proc, gl->gen[i][j].b_to_s[1]);
            }
        }
    }
}


// в будущем может быть такое, что эти функции надо будет разделить на r и w !!!
// Закрывает ненужные пайпы чтения и запимси, в нужных chpok-ах --- ДО РАБОТЫ ПАЙПОВ - ok 1
void close_ne_rw_pipes(FILE * pipes, global* gl, int cur_id_proc) {
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != cur_id_proc) {
            if (j > cur_id_proc) {
                close(gl->gen[cur_id_proc][j].s_to_b[0]);
                close(gl->gen[cur_id_proc][j].b_to_s[1]);
                fprintf(pipes, "INFO_PIPES: type=ne_otnos_rw, proc %d close %d\n", gl->id_proc, gl->gen[cur_id_proc][j].s_to_b[0]);
                fprintf(pipes, "INFO_PIPES: type=ne_otnos_rw, proc %d close %d\n", gl->id_proc, gl->gen[cur_id_proc][j].b_to_s[1]);
            
            } else {
                close(gl->gen[cur_id_proc][j].s_to_b[1]);
                close(gl->gen[cur_id_proc][j].b_to_s[0]);
                fprintf(pipes, "INFO_PIPES: type=ne_otnos_rw, proc %d close %d\n", gl->id_proc, gl->gen[cur_id_proc][j].s_to_b[1]);
                fprintf(pipes, "INFO_PIPES: type=ne_otnos_rw, proc %d close %d\n", gl->id_proc, gl->gen[cur_id_proc][j].b_to_s[0]);
            }
        }
    }
}

void close_after_write(FILE * pipes, global* gl) {
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc) {
            if (j > gl->id_proc) {
                close(gl->gen[gl->id_proc][j].s_to_b[1]);
                fprintf(pipes, "INFO_PIPES: type=close_after_write, proc %d close %d\n", gl->id_proc, gl->gen[gl->id_proc][j].s_to_b[1]);
            } else {
                close(gl->gen[gl->id_proc][j].b_to_s[1]);
                fprintf(pipes, "INFO_PIPES: type=close_after_write, proc %d close %d\n", gl->id_proc, gl->gen[gl->id_proc][j].b_to_s[1]);
            }
        }
    }
}

void close_after_read(FILE * pipes, global* gl) {
    for (size_t j = 0; j < gl->count_proc; j++) {
        if (j != gl->id_proc) {
            if (j > gl->id_proc) {
                close(gl->gen[gl->id_proc][j].b_to_s[0]);
                fprintf(pipes, "INFO_PIPES: type=close_after_read, proc %d close %d\n", gl->id_proc, gl->gen[gl->id_proc][j].b_to_s[0]);
            } else {
                close(gl->gen[gl->id_proc][j].s_to_b[0]);
                fprintf(pipes, "INFO_PIPES: type=close_after_read, proc %d close %d\n", gl->id_proc, gl->gen[gl->id_proc][j].s_to_b[0]);
            }
        }
    }
}

int my_compare(const my_queue* mq1, const my_queue* mq2) {
    if (mq1->time < mq2->time) {
        return -1;
    } else if (mq1->time > mq2->time) {
        return 1;
    } else {
        if (mq1->id_proc < mq2->id_proc) {
            return -1;
        } else if (mq1->id_proc > mq2->id_proc){
            return 1;
        }
    }
    return 0;
}

void my_sort(global * gl) {
    qsort(gl->mq, gl->size_mq, sizeof(my_queue), (int(*) (const void *, const void *)) my_compare);
}

my_queue pop(global * gl) {
    my_queue result = gl->mq[0];
    for (int i = 0; i < gl->size_mq - 1;i++) {
        gl->mq[i] = gl->mq[i + 1];
    }
    gl->size_mq--;
    return result;
}

// ok
void push(global * gl, my_queue new_element) {
    if (gl->size_mq == 9) {
        printf("ERROR");
    }
    gl->mq[gl->size_mq] = new_element;
    gl->size_mq++;
    my_sort(gl);
}

void print_mq(global * gl) {
    for (int i = 0; i < gl->size_mq; i++) {
        printf("(%d, %d) ", gl->mq[i].time, gl->mq[i].id_proc);
    }
}
