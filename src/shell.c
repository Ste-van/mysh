#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

static void print_prompt(void) {
    printf("-> ");
    fflush(stdout);

    return;
}

static char *read_line(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);

    if (nread < 0) {
        free(line);
        return NULL;
    }

    if (nread > 0 && line[nread - 1] == '\n') {
        line[nread - 1] = '\0';
    }

    return line;
}

int shell_run_repl(void) {
    int last_status = 0;

    while(1) {
        print_prompt();
        char *line = read_line();

        if (line == NULL) {
            free(line);
            break;
        }

        // Commands

        free(line);
    }

    return last_status;
}
