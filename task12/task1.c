#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/* Максимальное количество задач, которые могут накопиться */
#define MAX_TASKS 10
/* Количество потоков-генераторов */
#define NUM_GENERATORS 3

/* Глобальные переменные для синхронизации */
pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  task_cond  = PTHREAD_COND_INITIALIZER;

/* Общий ресурс — количество задач, ожидающих обработки */
int pending_tasks = 0;

/* Функция потока-генератора: создаёт задачи через случайные интервалы */
void* generator_thread(void* arg) {
    int thread_id = *((int*)arg);
    srand(time(NULL) + thread_id); /* Инициализация генератора случайных чисел */

    while (1) {
        /* Случайная задержка между 1 и 3 секундами */
        sleep(rand() % 3 + 1);

        /* Захватываем мьютекс для изменения общего счётчика */
        pthread_mutex_lock(&task_mutex);

        if (pending_tasks < MAX_TASKS) {
            pending_tasks++;
            printf("Генератор %d: Добавлена задача. Всего задач: %d\n", 
                   thread_id, pending_tasks);
            /* Сигнализируем обработчику, что появилась новая задача */
            pthread_cond_signal(&task_cond);
        } else {
            printf("Генератор %d: Очередь переполнена, задача отброшена.\n", 
                   thread_id);
        }

        pthread_mutex_unlock(&task_mutex);
    }

    return NULL;
}

/* Функция потока-обработчика: обрабатывает задачи по мере их появления */
void* processor_thread(void* arg) {
    while (1) {
        /* Захватываем мьютекс перед проверкой условия */
        pthread_mutex_lock(&task_mutex);

        /* Если задач нет — ждём сигнала от генератора */
        while (pending_tasks == 0) {
            printf("Обработчик: задач нет, жду...\n");
            pthread_cond_wait(&task_cond, &task_mutex);
        }

        /* Обрабатываем одну задачу */
        pending_tasks--;
        printf("Обработчик: обработал задачу. Осталось задач: %d\n", 
               pending_tasks);

        pthread_mutex_unlock(&task_mutex);

        /* Имитация времени обработки задачи */
        sleep(2);
    }

    return NULL;
}

int main() {
    pthread_t generator_threads[NUM_GENERATORS];
    pthread_t processor;
    int generator_ids[NUM_GENERATORS];
    int i;

    /* Создаём поток-обработчик */
    pthread_create(&processor, NULL, processor_thread, NULL);

    /* Создаём потоки-генераторы */
    for (i = 0; i < NUM_GENERATORS; i++) {
        generator_ids[i] = i + 1;
        pthread_create(&generator_threads[i], NULL, 
                       generator_thread, &generator_ids[i]);
    }

    /* Ждём завершения потоков (хотя в данном случае они бесконечные) */
    pthread_join(processor, NULL);
    for (i = 0; i < NUM_GENERATORS; i++) {
        pthread_join(generator_threads[i], NULL);
    }

    /* Уничтожаем мьютекс и условную переменную (сюда программа не дойдёт) */
    pthread_mutex_destroy(&task_mutex);
    pthread_cond_destroy(&task_cond);

    return 0;
}