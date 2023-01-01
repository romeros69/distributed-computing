#include <stdio.h>
#include <stdlib.h>
#include "pipes.h"
#include "format.h"

int main() {
    
    size_t count_proc = 3;

    // инициализация структуры с пайпами
    global* gl = new_global(count_proc);

    // подготовка пайпов - мб это не правильно, вроде как нужно зеркалить 
    create_pipes(gl);

    print_global_pipes(gl);

    return 0;
}
