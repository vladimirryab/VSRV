#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    const char *fifo_path = "/tmp/my_fifo";
    mkfifo(fifo_path, 0666); // создаем конвейер FIFO файл

    // разветвляем родительский процесс
    if (fork() == 0) {
        // дочерний процесс
        int fd = open(fifo_path, O_RDONLY);
        char buffer[128];
        read(fd, buffer, sizeof(buffer));
        printf("Child received: ");
        printf("%s", buffer);
        close(fd);
    }
    else {
        // родительский процесс
        int fd = open(fifo_path, O_WRONLY);
        const char *msg = "Hello from parent!";
        write(fd, msg, strlen(msg) + 1);
        close(fd);
        wait(NULL); // ждем окончания дочернего процесса
    }

    unlink(fifo_path); // удаляем FIFO
    return 0;
}