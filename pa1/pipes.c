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

// Функция создания пайпов - правильно ли это? Нужно зеркалить скорее всего
void create_pipes(global* gl) {
    for (size_t i = 0; i < gl->count_proc; i++) {
        for (size_t j = 0; j < gl->count_proc; j++) {
            if (i != j) {
                int err = pipe(gl->gen[i][j].s_to_b);
                if (err != 0) {
                    printf("ERROR: creating pipe s_to_b");
                }
                err = pipe(gl->gen[i][j].b_to_s);
                if (err != 0) {
                    printf("ERROR: creating pipe b_to_s");
                }
            }
        }
    }
}
