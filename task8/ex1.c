#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    FILE *fp = fopen("file1", "r");
    dup2(fileno(fp), fileno(stdin));
    fclose(fp);

    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        // Дочерний процесс
        dup2(fd[1], fileno(stdout));
        close(fd[0]);
        close(fd[1]);
        execl("/usr/bin/sort", "sort", (char *)0);
        exit(2);
    }
    else {
        // Родительский процесс
        dup2(fd[0], fileno(stdin));
        close(fd[0]);
        close(fd[1]);
        execl("/usr/bin/uniq", "uniq", (char *)0);
        exit(3);
    }

    return 0;
}