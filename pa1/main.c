#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "pipes.h"
#include "format.h"
#include "ipc.h"
#include "proc.h"

int main() {
    
    size_t count_proc = 3;

    // инициализация структуры с пайпами
    global* gl = new_global(count_proc);

    // подготовка пайпов
    create_pipes(gl);

    int p1, p2;
    p1 = fork();
    if (p1 == 0) {
        int id_proc = 1;
        run(gl, id_proc);
        return 0;
    } else {
        if (count_proc > 2) {
            p2 = fork();
            if (p2 == 0) {
                int id_proc = 2;
               
                run(gl, id_proc);
                
                return 0;
            }
        }
    }
    run_parent(gl, PARENT_ID);  // тут стопорится родительский
    wait(NULL);
    if (count_proc > 2) {
        wait(NULL);
    }

    return 0;
}
