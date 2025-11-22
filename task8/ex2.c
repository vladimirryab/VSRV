#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

char *msg = "Hello from parent!";

int main() {
    int fd[2], bytesRead;
    char buffer[128];

    pipe(fd);

    if (fork() == 0) {
        // Дочерний процесс
        close(fd[READ]);
        write(fd[WRITE], msg, strlen(msg) + 1);
        close(fd[WRITE]);
    } else {
        // Родительский процесс
        close(fd[WRITE]);
        bytesRead = read(fd[READ], buffer, sizeof(buffer));
        printf("Read %d bytes: \"%s\"\n", bytesRead, buffer);
        close(fd[READ]);
    }

    return 0;
}