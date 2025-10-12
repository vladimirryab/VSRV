#include <stdio.h>
#include <stdlib.h>

int main() {
    int N = 0; int i = 0;
    int *array = NULL;
    
    // Запрос размера массива у пользователя
    printf("Введите размер массива N: ");
    scanf("%d", &N);
    
    // Динамическое выделение памяти для массива из N целых чисел
    array = (int*)calloc(N, sizeof(int));
    
    if (array == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }
    
    // Ввод элементов массива с клавиатуры
    printf("Введите %d элементов массива:\n", N);
    for (i = 0; i < N; i++) {
        printf("Элемент [%d]: ", i);
        scanf("%d", &array[i]);
    }
    
    // Вывод элементов массива и их адресов
    printf("\nЭлементы массива и их адреса:\n");
    for (i = 0; i < N; i++) {
        printf("array[%d] = %d, адрес = %p\n", i, array[i], &array[i]);
    }
    
    // Проанализируем адресы
    printf("Адрес начала массива: %p\n", array);
    printf("Адрес array[0]: %p\n", &array[0]);
    printf("Адрес array[1]: %p\n", &array[1]);
    
    // Освобождение выделенной памяти
    free(array);
    array = NULL; // Избегаем висячего указателя
    
    return 0;
}