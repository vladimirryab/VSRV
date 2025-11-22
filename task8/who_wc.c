#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        // Дочерний процесс - Who
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("who", "who", NULL);
        perror("execlp who failed");
        exit(1);
    }

    if (fork() == 0) {
        // Дочерний процесс - WC
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc failed");
        exit(1);
    }

    // Родительский процесс
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}