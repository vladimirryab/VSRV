#include <stdio.h>
#include <stdlib.h>

// Функция возвращает несколько результатов через указатели
void count_chars(const char *str, int *vowels, int *consonants, int *delimiters) {
    *vowels = 0;
    *consonants = 0;
    *delimiters = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            (*vowels)++;  // проверяем гласные
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            (*consonants)++; // блок сработает если не гласная, проверим согласные
        }
        else if (c == ' ' || c == ',' || c == '.' || c == '!' || c == '?') {
            (*delimiters)++; // блок сработает если ни гласная, ни согласная, но разделитель из указанных выше
        }
    }
}

int main() {
    char *text = NULL;
    size_t len = 0; // размер строки
    ssize_t read; // знаковый тип, результат чтения строки
    int v, c, d; // переменные с результатом
    
    printf("Введите строку: ");
    read = getline(&text, &len, stdin); // getline сама перераспределит память
    
    if (read == -1) {
        printf("Ошибка ввода!\n");
        return 1;
    }
    
    // Указатель на функцию
    void (*func_ptr)(const char*, int*, int*, int*);
    func_ptr = &count_chars;
    
    // Вызов функции через указатель
    func_ptr(text, &v, &c, &d); // передали адреса переменных
    
    printf("Гласные: %d\n", v);
    printf("Согласные: %d\n", c);
    printf("Разделители: %d\n", d);
    
    // Освобождение памяти
    free(text);
    
    return 0;
}