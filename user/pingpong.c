#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
        // child
        char ping[4];
        if (read(p[0], ping, 4))
        {
            fprintf(1, "%d: received %s\n", getpid(), ping);
            write(p[1], "pong", 4);
            close(p[0]);
            close(p[1]);
            exit(0);
        }
        close(p[0]);
        close(p[1]);
        exit(-1);
    }
    else
    {
        write(p[1], "ping", 4);
        char pong[4];
        if (read(p[0], pong, 4))
        {
            fprintf(1, "%d: received %s\n", getpid(), pong);
            close(p[0]);
            close(p[1]);
            exit(0);
        }
        close(p[0]);
        close(p[1]);
        exit(-1);
    }
}
