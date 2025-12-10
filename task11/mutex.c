#include <stdio.h>        /* стандартный ввод-вывод */
#include <pthread.h>      /* функции и структуры pthread */
#include <string.h>       /* для strcpy */
#include <stdlib.h>       /* для exit */

#define NUM_EMPLOYEES 2   /* количество сотрудников */

/* глобальный мьютекс для защиты данных */
pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;

struct employee {
    int number;
    int id;
    char first_name[20];
    char last_name[30];
    char department[30];
    int room_number;
};

/* глобальный массив сотрудников */
struct employee employees[] = {
    { 1, 12345678, "danny", "coresh", "Accounting", 101 },
    { 2, 87654321, "misha", "levyn", "Programmers", 202 }
};

/* глобальная переменная — сотрудник дня */
struct employee employee_of_the_day;

/* копирование структуры employee с защитой мьютексом */
void copy_employee(struct employee* from, struct employee* to) {
    int rc;
    rc = pthread_mutex_lock(&a_mutex);   /* блокировка мьютекса */

    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name, from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;

    rc = pthread_mutex_unlock(&a_mutex); /* разблокировка мьютекса */
}

/* функция потока: постоянно копирует одного сотрудника в employee_of_the_day */
void* do_loop(void* data) {
    int my_num = *((int*)data); /* номер сотрудника (1 или 2) */
    while (1) {
        copy_employee(&employees[my_num-1], &employee_of_the_day);
    }
}

int main(int argc, char* argv[]) {
    int i;
    pthread_t p_thread1, p_thread2;
    int num1 = 1, num2 = 2;
    struct employee eotd;
    struct employee* worker;

    /* инициализация сотрудника дня первым сотрудником */
    copy_employee(&employees[0], &employee_of_the_day);

    /* создание потоков */
    pthread_create(&p_thread1, NULL, do_loop, (void*)&num1);
    pthread_create(&p_thread2, NULL, do_loop, (void*)&num2);

    /* проверка целостности данных в основном потоке */
    for (i = 0; i < 60000; i++) {
        copy_employee(&employee_of_the_day, &eotd);
        worker = &employees[eotd.number - 1];

        if (eotd.id != worker->id) {
            printf("mismatching 'id', %d != %d (loop '%d')\n", eotd.id, worker->id, i);
            exit(0);
        }
        if (strcmp(eotd.first_name, worker->first_name) != 0) {
            printf("mismatching 'first_name', %s != %s (loop '%d')\n", eotd.first_name, worker->first_name, i);
            exit(0);
        }
        if (strcmp(eotd.last_name, worker->last_name) != 0) {
            printf("mismatching 'last_name', %s != %s (loop '%d')\n", eotd.last_name, worker->last_name, i);
            exit(0);
        }
        if (strcmp(eotd.department, worker->department) != 0) {
            printf("mismatching 'department', %s != %s (loop '%d')\n", eotd.department, worker->department, i);
            exit(0);
        }
        if (eotd.room_number != worker->room_number) {
            printf("mismatching 'room_number', %d != %d (loop '%d')\n", eotd.room_number, worker->room_number, i);
            exit(0);
        }
    }

    printf("Glory, employees contents was always consistent\n");
    return 0;
}