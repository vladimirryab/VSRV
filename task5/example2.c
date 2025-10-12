#include<stdio.h>
int main()
{
 // объявляем переменную и инициализируем ее
 int score = 300;
// объявление и инициализация переменной указателя 'intPtr' адресом 'score'
 int *intPtr = &score;
 // variable value
 printf("Value of score: %d\n", score);
 // получить адрес переменной 'score'
 printf("Адрес score: %p\n", &score);
 // значение переменной указателя 'intPtr'
 printf("Значение intPtr(указателя) :%p\n", intPtr);
 // Адрес переменной указателя 'intPtr'
 printf("Адрес intPtr(указателя): %p\n", &intPtr);
 return 0;
}