#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main(int argc, char *argv[]) //аргументы командной строки
{
    int fd; //файловый дескриптор (номер файла, с которым происходит работа в программе, используемой ОС)
    fd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0644);
    dup2(fd, 1);
    close(fd);
    execvp(argv[2], &argv[2]);
    perror("main");
}