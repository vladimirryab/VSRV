#include <stdio.h>
#include <unistd.h>

#define MAX_COUNT 200

void ChildProcess(void);
void ParentProcess(void);

int main()
{
    pid_t pid;
    
    pid = fork();
    
    if (pid == 0) {
        ChildProcess();
    } else {
        ParentProcess();
    }
    
    return 0;
}

void ChildProcess(void)
{
    for (int i = 1; i <= MAX_COUNT; i++) {
        printf("This line is from child, value = %d\n", i);
    }
    printf("*** Child process is done ***\n");
}

void ParentProcess(void)
{
    for (int i = 1; i <= MAX_COUNT; i++) {
        printf("This line is from parent, value = %d\n", i);
    }
    printf("*** Parent process is done ***\n");
}