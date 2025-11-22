#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        // Дочерний процесс - LS
        close(fd[0]);
        // закрываем чтение
        dup2(fd[1], 1);
        // stdout -> канал
        close(fd[1]);
        execlp("ls", "ls", NULL);
        // запускаем ls
        perror("execlp ls failed");
        exit(1);
    }

    if (fork() == 0) {
        // Дочерний процесс - WC
        close(fd[1]);
        // закрываем запись
        dup2(fd[0], 0);
        // stdin <- канал
        close(fd[0]);
        execlp("wc", "wc", "-l", NULL);
        // запускаем wc -l
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