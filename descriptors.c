// Рябенко В.В. КВБО-05-23.

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int fd1, fd2, fd3; // инициализация переменных
	
	fd1 = open("output.txt", O_CREAT | O_RDWR | O_TRUNC); // создаем/открываем файл
	printf("fd1 = %d\n", fd1);
	write(fd1, "Hello,", 6); // записываем Hello,

	fd2 = dup(fd1); // дублируем дескриптор в fd2
	printf("fd2 = %d\n", fd2);
	write(fd2, " World", 6); // записываем " World"

	close(0); // после закрытия нулевого дескриптора, он станет доступен для дублирования
	
	fd3 = dup(fd1); // дублируем fd1 в fd3, dup возьмет первый свободный (0)
	printf("fd3 = %d\n", fd3);
	write(0, "!", 1); // запишем ! в output.txt так как 0 теперь не stdin а указыавет на файл

	dup2(0, 2); // явно перенаправим stderr (2) в output.txt
	write(2, "\n", 1); // запишем знак окончания строки, так как 2 теперь не stderr а указывает на файл
} 
