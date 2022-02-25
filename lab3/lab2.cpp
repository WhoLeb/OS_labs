#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct 
{
    int flag;
    char sym;
    int fd;
}t_args;

void* proc1(void *arg) 
{  
    printf("\nПоток 1 начал свою работу\n");
    t_args* args = (t_args*) arg; 

    char buf[NI_MAXHOST] = "\0";

    while (args->flag == 0)
    {
        struct addrinfo* result;
        struct addrinfo* res;
        int error;

        /* resolve the domain name into a list of addresses */
        error = getaddrinfo("www.example.com", NULL, NULL, &result);
        if (error != 0) {   
            if (error == EAI_SYSTEM) {
                perror("getaddrinfo");
            } else {
                fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
            }   
            exit(EXIT_FAILURE);
        }   

        error = getnameinfo(
                result->ai_addr, 
                result->ai_addrlen, 
                buf, 
                NI_MAXHOST, 
                NULL,
                0,
                0
                ); 
        if (error != 0) {
            fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
        }
        freeaddrinfo(result);
        
        if(write(args->fd, buf, NI_MAXHOST) == -1)
        {
            printf("Невозможно записать, ошибка: %s\n", strerror(errno));
        }
        sleep(1);
    }
    printf("\nПоток 1 закончил свою работу\n");
    pthread_exit((void*)0);
}

void* proc2(void* arg) 
{    
    printf("\nПоток 2 начал свою работу\n");
    t_args* args = (t_args*) arg; 
    
    char buf[NI_MAXHOST];

    while(!args->flag)
    {
        char buf[NI_MAXHOST];
        if(read(args->fd, buf, NI_MAXHOST)==-1)
        {
            printf("Невозможно считать, ошибка: %s\n",strerror(errno)); 
        }
        else
            printf("hostname: %s\n", buf);
        sleep(1); 
    }
    
    printf("\nПоток 2 закончил свою работу\n");
    pthread_exit((void*)0);
}

int main() 
{
    printf("\nПрограмма начала работу\n");
    int filedes[2]; 
    pipe2(filedes, O_NONBLOCK);
    t_args arg1 = {0, '1', filedes[1]};
    t_args arg2 = {0, '2', filedes[0]};
    pthread_t id1, id2;
  
    pthread_create(&id1, NULL, proc1, &arg1);
    pthread_create(&id2, NULL, proc2, &arg2);
  
    printf("\nПрограмма ожидает нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");
  
    arg1.flag = 1;
    arg2.flag = 1;
  
    int* exitcode1, *exitcode2;
    pthread_join(id1, (void**)&exitcode1);
    printf("Код первого потока: %p\n", exitcode1);
    pthread_join(id2, (void**)&exitcode2);
    printf("Код второго потока: %p\n",exitcode2);
    
    close(filedes[0]);
    close(filedes[1]);
   
    printf("\nПрограмма закончила работу\n");
    return 0;
}
