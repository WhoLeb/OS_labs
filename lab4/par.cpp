#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() 
{
    printf("\nПрограмма два начала работу\n");

    pid_t pid = fork();

    printf("Идентификатор программы-родителя: %d\n", getppid());
    printf("Идентификатор второй программы: %d\n", getpid()); 
    printf("Идентификатор дочерней программы: %d\n", pid); 
    
    int retval;
    if (pid == 0) {
        execle("kid", "hello", "world", NULL, NULL);
    }else if (pid > 0) {
        while (waitpid(pid, &retval, WNOHANG) == 0) {
            printf("\nЖдем...\n");
            usleep(500000);
        }
    }
    else
        perror("fork");
    
    printf("Код завершения: %d", retval); 

    printf("\nПрограмма два закончила работу\n");
    return 0;
}
