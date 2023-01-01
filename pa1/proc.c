
#include "proc.h"


void run(global* gl, int id_proc) {
    close_nenuzh_pipes(gl, id_proc);
    
}

void run_parent(global* gl, int id_proc) {
    close_nenuzh_pipes(gl, id_proc);

}
