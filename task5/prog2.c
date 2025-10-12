#include <stdio.h>
#include <stdlib.h>

int main() {
    int N, M; int i, j;
    int **array;
    
    // Запрос размеров массива у пользователя
    printf("Введите количество строк N: ");
    scanf("%d", &N);
    printf("Введите количество столбцов M: ");
    scanf("%d", &M);
    
    // Динамическое выделение памяти для двумерного массива NxM
    array = (int**)malloc(N * sizeof(int*));
    
    if (array == NULL) {
        printf("Ошибка выделения памяти для строк!\n");
        return 1;
    }
    
    for (i = 0; i < N; i++) {
        array[i] = (int*)malloc(M * sizeof(int));
        if (array[i] == NULL) {
            printf("Ошибка выделения памяти для столбцов!\n");
            return 1;
        }
    }
    
    // Ввод элементов массива с клавиатуры
    printf("Введите элементы массива %dx%d:\n", N, M);
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("Элемент [%d][%d]: ", i, j);
            scanf("%d", &array[i][j]);
        }
    }
    
    // Вывод элементов массива и их адресов
    printf("\nЭлементы массива и их адреса:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("array[%d][%d] = %d, адрес = %p\n", i, j, array[i][j], &array[i][j]);
        }
    }
    
    // Проанализируем адреса
    printf("Адрес массива указателей: %p\n", array);
    for (i = 0; i < N; i++) {
        printf("Адрес строки %d: %p\n", i, array[i]);
    }
    
    // Освобождение выделенной памяти
    for (i = 0; i < N; i++) {
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;
    
    return 0;
}