#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/* Максимальное количество чисел, которые могут накопиться */
#define MAX_NUMBERS 10

/* Глобальные переменные для синхронизации */
pthread_mutex_t number_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t number_cond = PTHREAD_COND_INITIALIZER;

/* Общие данные */
int generated_number = 0;
int number_available = 0; /* 0 - число не готово, 1 - готово чётное, 2 - готово нечётное */

/* Флаги для корректного завершения */
int program_running = 1;

/* Функция потока-генератора */
void* generator_thread(void* arg) {
    srand(time(NULL));
    
    while (program_running) {
        /* Задержка 1 секунда */
        sleep(1);
        
        /* Генерируем случайное число от 1 до 100 */
        int num = rand() % 100 + 1;
        
        /* Захватываем мьютекс для изменения общего числа */
        pthread_mutex_lock(&number_mutex);
        
        /* Ждём, пока предыдущее число не будет обработано */
        while (number_available != 0 && program_running) {
            pthread_cond_wait(&number_cond, &number_mutex);
        }
        
        if (!program_running) {
            pthread_mutex_unlock(&number_mutex);
            break;
        }
        
        /* Записываем новое число */
        generated_number = num;
        
        /* Определяем тип числа */
        if (num % 2 == 0) {
            number_available = 1; /* Чётное */
            printf("Генератор: создал чётное число %d\n", num);
        } else {
            number_available = 2; /* Нечётное */
            printf("Генератор: создал нечётное число %d\n", num);
        }
        
        /* Сигнализируем обработчикам */
        pthread_cond_broadcast(&number_cond);
        
        pthread_mutex_unlock(&number_mutex);
    }
    
    printf("Генератор завершает работу\n");
    return NULL;
}

/* Функция потока для обработки чётных чисел */
void* even_processor_thread(void* arg) {
    while (program_running) {
        /* Захватываем мьютекс перед проверкой условия */
        pthread_mutex_lock(&number_mutex);
        
        /* Ждём, пока не появится чётное число */
        while (number_available != 1 && program_running) {
            pthread_cond_wait(&number_cond, &number_mutex);
        }
        
        if (!program_running) {
            pthread_mutex_unlock(&number_mutex);
            break;
        }
        
        /* Обрабатываем чётное число */
        if (number_available == 1) {
            int num = generated_number;
            int square = num * num;
            printf("Поток для чётных: квадрат числа %d = %d\n", num, square);
            
            /* Помечаем число как обработанное */
            number_available = 0;
            pthread_cond_signal(&number_cond); /* Будим генератор */
        }
        
        pthread_mutex_unlock(&number_mutex);
    }
    
    printf("Поток для чётных чисел завершает работу\n");
    return NULL;
}

/* Функция потока для обработки нечётных чисел */
void* odd_processor_thread(void* arg) {
    while (program_running) {
        /* Захватываем мьютекс перед проверкой условия */
        pthread_mutex_lock(&number_mutex);
        
        /* Ждём, пока не появится нечётное число */
        while (number_available != 2 && program_running) {
            pthread_cond_wait(&number_cond, &number_mutex);
        }
        
        if (!program_running) {
            pthread_mutex_unlock(&number_mutex);
            break;
        }
        
        /* Обрабатываем нечётное число */
        if (number_available == 2) {
            int num = generated_number;
            int cube = num * num * num;
            printf("Поток для нечётных: куб числа %d = %d\n", num, cube);
            
            /* Помечаем число как обработанное */
            number_available = 0;
            pthread_cond_signal(&number_cond); /* Будим генератор */
        }
        
        pthread_mutex_unlock(&number_mutex);
    }
    
    printf("Поток для нечётных чисел завершает работу\n");
    return NULL;
}

int main() {
    pthread_t generator, even_processor, odd_processor;
    
    printf("Начало работы...\n");
    
    /* Создаём три потока */
    pthread_create(&generator, NULL, generator_thread, NULL);
    pthread_create(&even_processor, NULL, even_processor_thread, NULL);
    pthread_create(&odd_processor, NULL, odd_processor_thread, NULL);
    
    /* Даём программе поработать 10 секунд */
    sleep(10);
    
    /* Корректно завершаем программу */
    printf("\nЗавершаем работу программы...\n");
    pthread_mutex_lock(&number_mutex);
    program_running = 0;
    pthread_cond_broadcast(&number_cond); /* Будим все спящие потоки */
    pthread_mutex_unlock(&number_mutex);
    
    /* Ждём завершения всех потоков */
    pthread_join(generator, NULL);
    pthread_join(even_processor, NULL);
    pthread_join(odd_processor, NULL);
    
    /* Уничтожаем мьютекс и условную переменную */
    pthread_mutex_destroy(&number_mutex);
    pthread_cond_destroy(&number_cond);
    
    printf("Программа успешно завершена!\n");
    return 0;
}