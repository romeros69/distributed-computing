#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "pipes.h"
#include "ipc.h"
#include "proc.h"
#include "common.h"
#include "banking.h"
#include "format.h"

int main(int argc, char * argv[])
{
    int * init_balance = (int*) malloc(sizeof(int)*10);
    for (size_t i = 3; i < argc; i++) {
        sscanf(argv[i], "%d", &init_balance[i -3]);
    }
    if (strcmp(argv[1], "-p") != 0) {
        printf("invalid param");
        return 1;
    }

    //input_c - входной аргумент
    size_t input_c;
    sscanf(argv[2], "%zu", &input_c);
    size_t count_proc = input_c + 1;

    FILE *events;
    FILE *pipes;
    events = fopen(events_log, "w");
    pipes = fopen(pipes_log, "w");
    // инициализация структуры с пайпами
    global* gl = new_global(count_proc);
    // подготовка пайпов
    create_pipes(gl);
    // print_global_pipes(gl, 0);
    int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
    p1 = fork();
    if (p1 == 0) {
        gl->id_proc = 1;
        gl->dollar = init_balance[0];
        gl->history.s_id = 1;
        gl->history.s_history_len = 1;
        memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
        run(events, pipes, gl, 1);
        //  printf("proc %d balance = %d\n", gl->id_proc, gl->history.s_history[gl->history.s_history_len - 1].s_balance);
        return 0;
    } 
    if (count_proc > 2) {
        p2 = fork();
        if (p2 == 0) {
            gl->id_proc = 2;
            gl->history.s_id = 2;
            gl->history.s_history_len = 1;
            gl->dollar = init_balance[1];
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 2);
            // printf("proc %d balance = %d\n", gl->id_proc, gl->history.s_history[gl->history.s_history_len - 1].s_balance);
            return 0;
        }
    }
    if (count_proc > 3) {
        p3 = fork();
        if (p3 == 0) {
            gl->id_proc = 3;
            gl->dollar = init_balance[2];
            gl->history.s_id = 3;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 3);
            return 0;
        }
    }
    if (count_proc > 4) {
        p4 = fork();
        if (p4 == 0) {
            gl->id_proc = 4;
            gl->dollar = init_balance[3];
            gl->history.s_id = 4;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 4);
            return 0;
        }
    }
    if (count_proc > 5) {
        p5 = fork();
        if (p5 == 0) {
            gl->id_proc = 5;
            gl->dollar = init_balance[4];
            gl->history.s_id = 5;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 5);
            return 0;
        }
    }
    if (count_proc > 6) {
        p6 = fork();
        if (p6 == 0) {
            gl->id_proc = 6;
            gl->dollar = init_balance[5];
            gl->history.s_id = 6;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 6);
            return 0;
        }
    }
    if (count_proc > 7) {
        p7 = fork();
        if (p7 == 0) {
            gl->id_proc = 7;
            gl->dollar = init_balance[6];
            gl->history.s_id = 7;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 7);
            return 0;
        }
    }
    if (count_proc > 8) {
        p8 = fork();
        if (p8 == 0) {
            gl->id_proc = 8;
            gl->dollar = init_balance[7];
            gl->history.s_id = 8;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 8);
            return 0;
        }
    }
    if (count_proc > 9) {
        p9 = fork();
        if (p9 == 0) {
            gl->id_proc = 9;
            gl->dollar = init_balance[8];
            gl->history.s_id = 9;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 9);
            return 0;
        }
    }
    if (count_proc > 10) {
        p10 = fork();
        if (p10 == 0) {
            gl->id_proc = 10;
            gl->dollar = init_balance[9];
            gl->history.s_id = 10;
            gl->history.s_history_len = 1;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 10);
            return 0;
        }
    }

    run_parent(events, pipes, gl, PARENT_ID);  // тут стопорится родительский
    for (size_t i = 0; i < count_proc - 1; i++) {
        wait(NULL);
    }
    fclose(events);
    fclose(pipes);

    return 0;
}
