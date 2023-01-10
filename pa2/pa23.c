#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "pipes.h"
#include "ipc.h"
#include "proc.h"
#include "common.h"
#include "banking.h"

int main(int argc, char * argv[])
{

    int d1 = 10;
    int d2 = 10;

    if (argc != 3) {
        printf("invalid count arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "-p") != 0) {
        printf("invalid param");
        return 1;
    }

    // input_c - входной аргумент
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
        gl->dollar = d1;
        gl->history.s_id = 1;
        gl->history.s_history_len = 1;
        memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
        run(events, pipes, gl, 1);
        printf("proc %d balance = %d\n", gl->id_proc, gl->history.s_history[gl->history.s_history_len - 1].s_balance);
        return 0;
    } 
    if (count_proc > 2) {
        p2 = fork();
        if (p2 == 0) {
            gl->id_proc = 2;
            gl->history.s_id = 2;
            gl->history.s_history_len = 1;
            gl->dollar = d2;
            memset(gl->history.s_history, 0, sizeof(gl->history.s_history));
            run(events, pipes,gl, 2);
            printf("proc %d balance = %d\n", gl->id_proc, gl->history.s_history[gl->history.s_history_len - 1].s_balance);
            return 0;
        }
    }
    if (count_proc > 3) {
        p3 = fork();
        if (p3 == 0) {
            gl->id_proc = 3;
            run(events, pipes,gl, 3);
            return 0;
        }
    }
    if (count_proc > 4) {
        p4 = fork();
        if (p4 == 0) {
            gl->id_proc = 4;
            run(events, pipes,gl, 4);
            return 0;
        }
    }
    if (count_proc > 5) {
        p5 = fork();
        if (p5 == 0) {
            gl->id_proc = 5;
            run(events, pipes,gl, 5);
            return 0;
        }
    }
    if (count_proc > 6) {
        p6 = fork();
        if (p6 == 0) {
            gl->id_proc = 6;
            run(events, pipes,gl, 6);
            return 0;
        }
    }
    if (count_proc > 7) {
        p7 = fork();
        if (p7 == 0) {
            gl->id_proc = 7;
            run(events, pipes,gl, 7);
            return 0;
        }
    }
    if (count_proc > 8) {
        p8 = fork();
        if (p8 == 0) {
            gl->id_proc = 8;
            run(events, pipes,gl, 8);
            return 0;
        }
    }
    if (count_proc > 9) {
        p9 = fork();
        if (p9 == 0) {
            gl->id_proc = 9;
            run(events, pipes,gl, 9);
            return 0;
        }
    }
    if (count_proc > 10) {
        p10 = fork();
        if (p10 == 0) {
            gl->id_proc = 10;
            run(events, pipes,gl, 10);
            return 0;
        }
    }

    run_parent(events, pipes,gl, PARENT_ID);  // тут стопорится родительский
    for (size_t i = 0; i < count_proc; i++) {
        wait(NULL);
    }
    fclose(events);
    fclose(pipes);

    return 0;
}
