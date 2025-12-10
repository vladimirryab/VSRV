#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_CATS 5
#define MAX_FOOD 1000
#define MIN_FOOD 100
#define CAT_PORTION 100
#define OWNER_PORTION 200

/* Глобальные переменные для синхронизации */
pthread_mutex_t food_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t food_cond = PTHREAD_COND_INITIALIZER;

/* Общие данные */
int food_amount = 500; /* Начальное количество корма */
int program_running = 1;
int cats_eating[NUM_CATS] = {0}; /* Счётчик съеденного каждым котом */

/* Прототипы функций */
void* owner_thread(void* arg);
void* cat_thread(void* arg);

/* Функция потока-владельца */
void* owner_thread(void* arg) {
    srand(time(NULL));
    
    printf("Владелец: начинаю кормить %d котов\n", NUM_CATS);
    
    while (program_running) {
        /* Случайная задержка между 2 и 5 секундами */
        sleep(rand() % 4 + 2);
        
        pthread_mutex_lock(&food_mutex);
        
        /* Проверяем, нужно ли добавлять корм */
        if (food_amount < MAX_FOOD) {
            int to_add = OWNER_PORTION;
            
            /* Не превышаем максимум */
            if (food_amount + to_add > MAX_FOOD) {
                to_add = MAX_FOOD - food_amount;
            }
            
            food_amount += to_add;
            printf("Владелец: добавил %d грамм корма. Всего в кормушке: %d грамм\n", 
                   to_add, food_amount);
            
            /* Будим всех котов */
            pthread_cond_broadcast(&food_cond);
        } else {
            printf("Владелец: кормушка полна (%d грамм), не добавляю\n", food_amount);
        }
        
        pthread_mutex_unlock(&food_mutex);
    }
    
    printf("Владелец: завершаю работу\n");
    return NULL;
}

/* Функция потока-кота */
void* cat_thread(void* arg) {
    int cat_id = *((int*)arg);
    srand(time(NULL) + cat_id);
    
    printf("Кот %d: пришёл кушать\n", cat_id);
    
    while (program_running) {
        /* Кот голоден через случайный интервал */
        sleep(rand() % 3 + 1);
        
        pthread_mutex_lock(&food_mutex);
        
        /* Ждём, пока будет достаточно корма */
        while (food_amount < CAT_PORTION && program_running) {
            printf("Кот %d: жду, когда появится корм (сейчас %d грамм)\n", 
                   cat_id, food_amount);
            pthread_cond_wait(&food_cond, &food_mutex);
        }
        
        if (!program_running) {
            pthread_mutex_unlock(&food_mutex);
            break;
        }
        
        /* Кот ест */
        if (food_amount >= CAT_PORTION) {
            food_amount -= CAT_PORTION;
            cats_eating[cat_id - 1]++;
            
            printf("Кот %d: съел %d грамм корма. Осталось: %d грамм. Всего съедено: %d порций\n", 
                   cat_id, CAT_PORTION, food_amount, cats_eating[cat_id - 1]);
            
            /* Если корма стало мало, будим владельца */
            if (food_amount < MIN_FOOD) {
                pthread_cond_signal(&food_cond);
            }
        }
        
        pthread_mutex_unlock(&food_mutex);
        
        /* Имитация времени еды */
        usleep(500000); /* 0.5 секунды */
    }
    
    printf("Кот %d: наелся, ухожу. Всего съел: %d порций\n", 
           cat_id, cats_eating[cat_id - 1]);
    return NULL;
}

int main() {
    pthread_t owner;
    pthread_t cats[NUM_CATS];
    int cat_ids[NUM_CATS];
    int i;
    
    printf("=== ПРОГРАММА 'ПОКОРМИ КОТА' ===\n");
    printf("Владелец кормит %d котов\n", NUM_CATS);
    printf("Максимум в кормушке: %d грамм\n", MAX_FOOD);
    printf("Минимум в кормушке: %d грамм\n", MIN_FOOD);
    printf("Порция кота: %d грамм\n", CAT_PORTION);
    printf("Порция владельца: %d грамм\n", OWNER_PORTION);
    printf("Начальный корм: %d грамм\n\n", food_amount);
    
    /* Создаём поток-владельца */
    pthread_create(&owner, NULL, owner_thread, NULL);
    
    /* Создаём потоки-котов */
    for (i = 0; i < NUM_CATS; i++) {
        cat_ids[i] = i + 1;
        pthread_create(&cats[i], NULL, cat_thread, &cat_ids[i]);
    }
    
    /* Даём программе поработать 30 секунд */
    printf("\nПрограмма работает 30 секунд...\n\n");
    sleep(30);
    
    /* Корректно завершаем программу */
    printf("\nЗавершаем программу...\n");
    pthread_mutex_lock(&food_mutex);
    program_running = 0;
    pthread_cond_broadcast(&food_cond); /* Будим всех */
    pthread_mutex_unlock(&food_mutex);
    
    /* Ждём завершения всех потоков */
    pthread_join(owner, NULL);
    for (i = 0; i < NUM_CATS; i++) {
        pthread_join(cats[i], NULL);
    }
    
    /* Выводим статистику */
    printf("\n=== СТАТИСТИКА ===\n");
    printf("Осталось корма: %d грамм\n", food_amount);
    int total_eaten = 0;
    for (i = 0; i < NUM_CATS; i++) {
        printf("Кот %d съел: %d порций (%d грамм)\n", 
               i + 1, cats_eating[i], cats_eating[i] * CAT_PORTION);
        total_eaten += cats_eating[i];
    }
    printf("Всего съедено: %d порций (%d грамм)\n", 
           total_eaten, total_eaten * CAT_PORTION);
    
    /* Освобождаем ресурсы */
    pthread_mutex_destroy(&food_mutex);
    pthread_cond_destroy(&food_cond);
    
    printf("\nПрограмма успешно завершена!\n");
    return 0;
}