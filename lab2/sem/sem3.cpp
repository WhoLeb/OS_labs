#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

typedef struct
{
    sem_t *sid;
    int flag;
    char sym;
} args_t;

void *proc1(void *arg)
{
    printf("\nПоток 1 начал свою работу\n");
    args_t *args = (args_t *)arg;
    while (args->flag == 0)
    {
        struct timespec tp;
        clock_gettime(CLOCK_REALTIME, &tp);
        tp.tv_sec += 1;
        if(!sem_timedwait(args->sid, &tp))
        {
            for (int i = 0; i < 5; i++)
            {
                printf("%c", args->sym);
                fflush(stdout);
                sleep(1);
            }
            sem_post(args->sid);
        }
        sleep(1);
    }
    printf("\nПоток 1 закончил свою работу\n");
    return nullptr;
}

void *proc2(void *arg)
{
    printf("\nПоток 2 начал свою работу\n");
    args_t *args = (args_t *)arg;
    while (args->flag == 0)
    {
        struct timespec tp;
        clock_gettime(CLOCK_REALTIME, &tp);
        tp.tv_sec += 1;
        if (sem_trywait(args->sid) != -1)
        {
            for (int i = 0; i < 5; i++)
            {
                printf("%c", args->sym);
                fflush(stdout);
                sleep(1);
            }
            sem_post(args->sid);
        }
        sleep(1);
    }
    printf("\nПоток 2 закончил свою работу\n");
    return nullptr;
}

int main()
{
    printf("\nПрограмма начала работу\n");
    sem_t sid1;
    sem_init(&sid1, 0, 1);
    args_t arg1 = {&sid1, 0, '1'}, arg2 = {&sid1, 0, '2'};
    pthread_t id1, id2;
    pthread_create(&id1, NULL, proc1, &arg1);
    pthread_create(&id2, NULL, proc2, &arg2);
    printf("\nПрограмма ожидает нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");
    arg1.flag = 1;
    arg2.flag = 1;
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    sem_destroy(&sid1);
    printf("\nПрограмма закончила работу\n");
    return 0;
}
