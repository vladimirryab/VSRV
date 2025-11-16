#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void ctrl_c_handler(int sig);
void alarm_handler(int sig);

// Счетчик для демонстрации работы
int work_counter = 0;

// Обработчик alarm (квант времени)
void alarm_handler(int sig) {
    printf("Process switched now...\n");
    // В реальной системе здесь переключились бы на другой процесс
    alarm(3);  // Устанавливаем следующий квант на 3 секунды
}

void ctrl_c_handler(int sig) {
    printf("\nCtrl+C detected! Process stopped.\n");
    printf("Control returned to shell.\n");
    exit(0);
}

int main() {
    // Устанавливаем обработчик для SIGINT (Ctrl+C)
    signal(SIGINT, ctrl_c_handler);
    
    // Устанавливаем обработчик для ALARM (квант времени)
    signal(SIGALRM, alarm_handler);
    
    printf("Program is running. Press Ctrl+C to stop.\n");
    
    // Устанавливаем первый "квант времени" на 2 секунды
    alarm(2);
    
    // Выводим процесс работы
    while(1) {
        printf("Working... cycle %d\n", work_counter);
	work_counter++;
        sleep(1);
    }
    
    return 0;
}