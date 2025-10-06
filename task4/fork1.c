#include <stdio.h>
#include <unistd.h>
int main() {
    int pid;
    printf("I`m the original process with %d and ppid %d\n", getpid(), getppid());
    pid = fork();
    if(pid!=0){
            printf("I`m the parent process with pid %d and ppid %d\n", getpid(), getppid());
            printf("My child's pid is %d\n", pid);
    }else{     /*esli pid==0, this is child process*/
            sleep(5);
            printf("I'm the child process with pid %d and ppid %d\n", getpid(), getppid());
            printf("\n was deserted");
        }
    printf("\n pid %d terminates\n", getpid());
    
    return 0;
}
