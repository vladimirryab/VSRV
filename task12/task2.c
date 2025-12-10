#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_HANDLER_THREADS 3

/* ДВА РАЗНЫХ МЬЮТЕКСА */
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;  /* Только для очереди */
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;   /* Только для условной переменной */

pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

int num_requests = 0;

/* Структура запроса */
struct request {
    int number;
    struct request* next;
};

struct request* requests = NULL;
struct request* last_request = NULL;

/* Прототипы функций */
void add_request(int request_num);
struct request* get_request();
void handle_request(struct request* a_request, int thread_id);
void* handle_requests_loop(void* data);

/* Функция добавления запроса */
void add_request(int request_num)
{
    struct request* a_request = (struct request*)malloc(sizeof(struct request));
    if (!a_request) {
        fprintf(stderr, "add_request: out of memory\n");
        exit(1);
    }
    a_request->number = request_num;
    a_request->next = NULL;

    /* Блокируем queue_mutex для работы с очередью */
    pthread_mutex_lock(&queue_mutex);

    if (num_requests == 0) {
        requests = a_request;
        last_request = a_request;
    }
    else {
        last_request->next = a_request;
        last_request = a_request;
    }
    num_requests++;

    printf("Добавлен запрос %d. Всего в очереди: %d\n", request_num, num_requests);
    
    /* Разблокируем queue_mutex */
    pthread_mutex_unlock(&queue_mutex);

    /* Блокируем cond_mutex для отправки сигнала */
    pthread_mutex_lock(&cond_mutex);
    pthread_cond_signal(&got_request);
    pthread_mutex_unlock(&cond_mutex);
}

/* Функция получения запроса из очереди */
struct request* get_request()
{
    struct request* a_request = NULL;

    /* Блокируем queue_mutex для работы с очередью */
    pthread_mutex_lock(&queue_mutex);

    if (num_requests > 0) {
        a_request = requests;
        requests = a_request->next;
        if (requests == NULL) {
            last_request = NULL;
        }
        num_requests--;
    }

    /* Разблокируем queue_mutex */
    pthread_mutex_unlock(&queue_mutex);

    return a_request;
}

/* Обработка запроса */
void handle_request(struct request* a_request, int thread_id)
{
    if (a_request) {
        printf("Поток %d обработал запрос %d\n", thread_id, a_request->number);
    }
}

/* Основной цикл потока-обработчика */
void* handle_requests_loop(void* data)
{
    int thread_id = *((int*)data);
    struct request* a_request = NULL;
    
    printf("Запущен поток-обработчик %d\n", thread_id);

    while (1) {
        int should_wait = 0;
        
        /* 1. Проверяем, есть ли запросы в очереди */
        pthread_mutex_lock(&queue_mutex);
        if (num_requests == 0) {
            should_wait = 1; /* Очередь пуста - нужно ждать */
        }
        pthread_mutex_unlock(&queue_mutex);
        
        if (!should_wait) {
            /* Есть запросы - обрабатываем */
            a_request = get_request();
            if (a_request) {
                handle_request(a_request, thread_id);
                free(a_request);
            }
            /* Короткая пауза между обработками */
            usleep(100000); /* 100 мс */
        }
        else {
            /* 2. Очередь пуста - ждём на условной переменной */
            pthread_mutex_lock(&cond_mutex);
            
            /* 3. Ещё раз проверяем очередь (чтобы избежать гонки) */
            pthread_mutex_lock(&queue_mutex);
            if (num_requests == 0) {
                /* Очередь всё ещё пуста - можем ждать */
                pthread_mutex_unlock(&queue_mutex);
                printf("Поток %d: нет задач, жду...\n", thread_id);
                pthread_cond_wait(&got_request, &cond_mutex);
                printf("Поток %d: получен сигнал, просыпаюсь\n", thread_id);
            }
            else {
                /* Пока мы блокировали cond_mutex, появилась задача */
                pthread_mutex_unlock(&queue_mutex);
            }
            
            pthread_mutex_unlock(&cond_mutex);
        }
    }
    
    return NULL;
}

int main()
{
    pthread_t handler_threads[NUM_HANDLER_THREADS];
    int thread_ids[NUM_HANDLER_THREADS];
    int i;
    
    srand(time(NULL));

    /* Создаём потоки-обработчики */
    for (i = 0; i < NUM_HANDLER_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&handler_threads[i], NULL, 
                      handle_requests_loop, &thread_ids[i]);
    }
    
    /* Даём потокам время запуститься */
    sleep(2);
    
    /* Генерируем запросы */
    printf("\n=== Начинаю генерировать запросы ===\n");
    for (i = 1; i <= 20; i++) {
        add_request(i);
        
        /* Случайная задержка между запросами */
        int delay = rand() % 3 + 1; /* 1-3 секунды */
        sleep(delay);
    }
    
    /* Ждём обработки всех запросов */
    sleep(5);
    
    printf("\n=== Все запросы обработаны ===\n");
    printf("Программа завершена (нажмите Ctrl+C для выхода)\n");
    
    /* В реальной программе здесь был бы код для корректного завершения потоков */
    while (1) {
        sleep(1);
    }
    
    return 0;
}