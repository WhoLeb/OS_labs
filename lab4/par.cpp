#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() 
{
    printf("\nПрограмма два начала работу\n");

    pid_t pid = fork();

    char* envp[] = {
        "LANG = LC_ALL\0",
        "HOME = /home",
        NULL
    };

    int retval;
    if (pid == 0) {
        execle("kid", "hello", "world", NULL, envp);
    }else if (pid > 0) {
        printf("Идентификатор программы-родителя: %d\n", getppid());
        printf("Идентификатор второй программы: %d\n", getpid()); 
        printf("Идентификатор дочерней программы: %d\n", pid); 
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
