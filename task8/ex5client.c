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
    int end_process;
    int stringlen;
    char readbuf[80];
    char end_str[5];

    printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
    fd = open(FIFO_FILE, O_CREAT|O_WRONLY);
    strcpy(end_str, "end");

    while (1) {
        //приглашение на ввод строки
        printf("Enter string: ");
        //чтение строки
        fgets(readbuf, sizeof(readbuf), stdin);

        //сохраняем длину строки
        stringlen = strlen(readbuf);

        //записываем в конец нуль символ - признак конца строки
        readbuf[stringlen - 1] = '\0';

        //копируем строку
        end_process = strcmp(readbuf, end_str);

        //printf("end_process is %d\n", end_process);
        if (end_process != 0) {
            //записываем сообщение
            write(fd, readbuf, strlen(readbuf));

            //печатаем на стандартный вывод строку и ее размер
            printf("Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
        } else {
            write(fd, readbuf, strlen(readbuf));
            printf("Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
            close(fd);
            break;
        }
    }

    return 0;
}