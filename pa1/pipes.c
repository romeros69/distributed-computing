#include <stdlib.h>
#include <malloc.h>
#include "pipes.h"

// Конструктор для создания структуры пайпов
// count_proc - count of all process in system
chpok** new_gen(size_t count_proc) {
    chpok** gen = (chpok**) malloc(sizeof(chpok*) * count_proc);
    for (size_t i = 0; i < count_proc; i++) {
        gen[i] = (chpok*) malloc(sizeof(chpok) * count_proc);
    }
    // test_add_elements_to_gen_pipes(count_proc, gen);
    return gen;
}

global* new_global(size_t count_proc) {
    chpok** gen = new_gen(count_proc);
    global* gl = (global*) malloc(sizeof(global));
    gl->count_proc = count_proc;
    gl->gen = gen;
    return gl;
}
