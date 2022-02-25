#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) 
{
    printf("\nПрограмма один начала работу\n");
    printf("Идентификатор программы один: %d\n", getpid()); 
    printf("Идентификатор программы-родителя: %d\n", getppid()); 
    for(int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
        fflush(stdout);
        sleep(1);
    }

    printf("\nПрограмма один закончила работу\n");
    int res = 0;
    for(int i = 0; i < argc-1; i++)
        res += (int)argv[i][0];

    return res;
}
