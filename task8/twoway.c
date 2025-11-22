#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int parent_to_child[2]; // канал родитель - дочерний
    int child_to_parent[2]; // канал дочерний - родитель

    pipe(parent_to_child);
    pipe(child_to_parent);

    if (fork() == 0) {
        // Дочерний процесс
        close(parent_to_child[1]); // закрываем запись в первый канал
        close(child_to_parent[0]); // закрываем чтение из второго канала

        char buffer[BUFFER_SIZE];
        int count = 0;

        while (count < 3) {
            // Читаем сообщение от родителя
            int bytes = read(parent_to_child[0], buffer, BUFFER_SIZE);
            buffer[bytes] = '\0';
            printf("CHILD received: %s", buffer);

            // Отправляем ответ родителю
            char response[BUFFER_SIZE];
            sprintf(response, "Response %d from child\n", ++count);
            write(child_to_parent[1], response, strlen(response));

            sleep(1);
        }

        // Завершаем диалог
        write(child_to_parent[1], "BYE\n", 4);

        close(parent_to_child[0]);
        close(child_to_parent[1]);
        exit(0);
    }
    else {
        // Родительский процесс
        close(parent_to_child[0]); // закрываем чтение из первого канала
        close(child_to_parent[1]); // закрываем запись во второй канал

        char *questions[] = {
            "Question 1: How are you?\n",
            "Question 2: What's your name?\n",
            "Question 3: What time is it?\n"
        };

        char buffer[BUFFER_SIZE];

        for (int i = 0; i < 3; i++) {
            // Отправляем вопрос дочернему процессу
            printf("PARENT sending: %s", questions[i]);
            write(parent_to_child[1], questions[i], strlen(questions[i]));

            // Ждем ответ
            int bytes = read(child_to_parent[0], buffer, BUFFER_SIZE);
            buffer[bytes] = '\0';
            printf("PARENT received: %s", buffer);
        }

        // Ждем завершающее сообщение
        int bytes = read(child_to_parent[0], buffer, BUFFER_SIZE);
        buffer[bytes] = '\0';
        printf("PARENT received final: %s", buffer);

        close(parent_to_child[1]);
        close(child_to_parent[0]);
        wait(NULL); // ждем завершения дочернего процесса

        return 0;
    }
}