#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void worker(int *p);

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
        close(p[1]);
        worker(p);
    }
    else
    {
        close(p[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, 4);
        }
        close(p[1]);
        wait((int*)0);
    }
    exit(0);
}

void worker(int * p)
{
    int prime, n;
    if (!read(p[0], &prime, 4))
    {
        close(p[0]);
        exit(0);
    }
    fprintf(1, "prime %d\n", prime);

    if (read(p[0], &n, 4)) {
        int newp[2];
        pipe(newp);
        if (fork() == 0 ) {
            close(newp[1]);
            worker(newp);
        } else {
            close(newp[0]);
            if (n % prime != 0) {
                write(newp[1], &n, 4);
            }
            while (read(p[0], &n, 4)) {
                if (n % prime != 0) write(newp[1], &n, 4);
            }
            close(newp[1]);
            wait(0);
        }
    }
    exit(0);
}
