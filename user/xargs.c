#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int readLine(int fd, char *argv[], int argc);

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "xargs usage: xargs command (args...)");
        exit(1);
    }
    char *command = malloc(strlen(argv[1]) + 1);
    strcpy(command, argv[1]);
    char *new_argv[MAXARG];
    for (int i = 1; i < argc; i++) {
        new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i - 1], argv[i]);
    }
    while (readLine(0, new_argv, argc - 1)) {
        if (fork() == 0) {
            exec(command, new_argv);
        } else {
            wait(0);
        }
    }
    exit(0);
}

int readLine(int fd, char *argv[], int argc) {
    char buf[1024];
    char *p = buf;
    while(read(0, p, 1)) {
        if (*p == '\n') {
            *p = '\0';
            break;
        }
        p ++;
        if (p >= buf + 1024) {
            break;
        }
    }

    if (p == buf) {
        return 0;
    }

    char *c = buf;
    while(c < p) {
        argv[argc] = c;
        while(*c != ' ' && c < p) {
            c ++;
        }
        while(*c == ' ' && c < p) {
            *c = '\0';
        }
    }

    return 1;
}
