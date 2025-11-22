#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FIFO_FILE "MYFIFO"

int main() {
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;
    int read_bytes;

    /* Создаем FIFO файл, если он не существует*/
    mknod(FIFO_FILE, S_IFIFO|0640, 0);
    strcpy(end, "end");

    //организуем непрерывный цикл для обмена сообщениями
    while(1) {
        //открываем FIFO файл для чтения
        fd = open(FIFO_FILE, O_RDONLY);
        read_bytes = read(fd, readbuf, sizeof(readbuf));
        readbuf[read_bytes] = '\0';
        printf("Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

        //копируем строку из буфера
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            close(fd);
            break;
        }
    }

    // Удаляем FIFO файл после завершения
    unlink(FIFO_FILE);
    return 0;
}