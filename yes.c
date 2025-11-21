#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int ac, char **av)
{
    if (ac < 2)
        return 1;

    char *s = av[1];
    size_t ln = strlen(s);
    if (!ln)
        return 1;

    int *tbl = malloc(ln * sizeof(int));
    if (!tbl)
        exit(1);

    size_t j;
    for (j = 0; j < ln; j++)
        tbl[j] = s[j] - 'y';

    size_t sz = ln * 8;
    char *bp = malloc(sz);
    if (!bp)
        exit(1);

    int fd[2];
    if (pipe(fd) < 0)
        exit(1);

    pid_t pid = fork();
    if (pid < 0)
        exit(1);

    if (pid == 0)
    {
        close(fd[0]);
        if (dup2(fd[1], 1) < 0)
            _exit(1);
        execlp("yes", "yes", (char *)0);
        _exit(1);
    }

    close(fd[1]);

    size_t i = 0;
    for (;;)
    {
        ssize_t r = read(fd[0], bp, sz);
        if (r <= 0)
            break;
        for (j = 0; j < (size_t)r; j++)
        {
            if (bp[j] == '\n')
                continue;
            bp[j] += tbl[i];
            i = (i + 1) % ln;
        }
        if (write(1, bp, r) < 0)
            break;
    }

    close(fd[0]);
    free(tbl);
    free(bp);

    int st;
    waitpid(pid, &st, 0);
    return 0;
}
