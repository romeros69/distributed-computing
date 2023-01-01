#include <stdlib.h>
#include "test.h"

// добавление рандомных элементов в gen для проверки корректного создания gen
void test_add_elements_to_gen_pipes(size_t count_proc, chpok** gen) {
    for (size_t i = 0; i < count_proc; i++) {
        for (size_t j = 0; j < count_proc; j++) {
            gen[i][j].s_to_b[0] = i;
            gen[i][j].s_to_b[1] = i;
            gen[i][j].b_to_s[0] = j;
            gen[i][j].b_to_s[1] = j;

        }
    }
}
