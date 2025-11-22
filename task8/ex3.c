#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd); // создаем pipe

    if (fork() != 0) {
        // Родительский процесс - ПИШЕТ в pipe
        close(fd[READ]);    // закрываем ненужный конец для чтения
        dup2(fd[WRITE], 1);    // перенаправляем stdout в канал
        close(fd[WRITE]);    // закрываем конец для записи
        execlp(argv[1], argv[1], NULL);    // запускаем первую команду
        perror("connect");    // сюда попадем только при ошибке execlp
        exit(1);
    } else {
        // Дочерний процесс - ЧИТАЕТ из pipe
        close(fd[WRITE]);    // закрываем ненужный конец для записи
        dup2(fd[READ], 0);    // перенаправляем stdin из канала
        close(fd[READ]);    // закрываем конец для чтения
        execlp(argv[2], argv[2], NULL);    // запускаем вторую команду
        perror("connect");    // сюда попадем только при ошибке execlp
        exit(1);
    }

    return 0;
}