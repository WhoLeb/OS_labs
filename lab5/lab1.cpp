#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

typedef struct 
{
    int flag;
    char sym;
}t_args;

void* proc1(void *arg) 
{  
    printf("\nПоток 1 начал свою работу\n");
    t_args* args = (t_args*) arg; 
   
    

    printf("\nПоток 1 закончил свою работу\n");
    pthread_exit((void*)0);
}

void* proc2(void* arg) 
{    
    printf("\nПоток 2 начал свою работу\n");
    t_args* args = (t_args*) arg; 
    
    
    
    printf("\nПоток 2 закончил свою работу\n");
    pthread_exit((void*)14);
}

int main() 
{
    printf("\nПрограмма начала работу\n");
    t_args arg1 = {0, '1'};
    t_args arg2 = {0, '2'};
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
   
    printf("\nПрограмма закончила работу\n");
    return 0;
}
