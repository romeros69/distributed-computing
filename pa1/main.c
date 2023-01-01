#include <stdio.h>
#include <stdlib.h>
#include "pipes.h"

void  print_global_pipes(global* gl) {
   size_t num_s = 0;
    for (size_t i = 0; i < gl->count_proc; i++) {
        for (size_t j = 0; j < gl->count_proc; j++) {
            if (j == 0) {
                printf("%zu| ", i);
            }
            printf("S%zu%zu-[%d;%d] [%d;%d]   ", i, j, gl->gen[i][j].s_to_b[0], gl->gen[i][j].s_to_b[1], gl->gen[i][j].b_to_s[0], gl->gen[i][j].b_to_s[1]);
            num_s++;
        }
        printf("\n");
    } 
}

void  print_gen_pipes(size_t count_proc, chpok** gen) {
    size_t num_s = 0;
    for (size_t i = 0; i < count_proc; i++) {
        for (size_t j = 0; j < count_proc; j++) {
            if (j == 0) {
                printf("%zu| ", i);
            }
            printf("S%zu%zu-[%d;%d] [%d;%d]   ", i, j, gen[i][j].s_to_b[0], gen[i][j].s_to_b[1], gen[i][j].b_to_s[0], gen[i][j].b_to_s[1]);
            num_s++;
        }
        printf("\n");
    }
}

int main() {
    global* gl = new_global(3);
    print_global_pipes(gl);
    return 0;
}
