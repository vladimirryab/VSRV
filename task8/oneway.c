#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        // Дочерний процесс - получатель
        close(fd[1]); // закрываем запись

        char buffer[100];
        int bytes_read;

        while ((bytes_read = read(fd[0], buffer, sizeof(buffer))) > 0) {
            buffer[bytes_read] = '\0';
            printf("CHILD received: %s", buffer);
        }

        close(fd[0]);
        exit(0);
    } else {
        // Родительский процесс - отправитель
        close(fd[0]); // закрываем чтение

        char *messages[] = {
            "Hello from parent!\n",
            "This is message 2\n",
            "Message number three\n",
            "END\n"
        };

        for (int i = 0; i < 4; i++) {
            printf("PARENT sending: %s", messages[i]);
            write(fd[1], messages[i], strlen(messages[i]));
            sleep(1); // небольшая задержка для наглядности
        }

        close(fd[1]);
        wait(NULL); // ждем завершения дочернего процесса
    }

    return 0;
}