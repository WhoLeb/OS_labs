#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

typedef struct
{
    pthread_mutex_t *mid;
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
        if(!pthread_mutex_timedlock(args->mid, &tp))
        {
            for (int i = 0; i < 5; i++)
            {
                printf("%c", args->sym);
                fflush(stdout);
                sleep(1);
            }
            pthread_mutex_unlock(args->mid);
        }
        sleep(1);
    }
    printf("\nПоток 1 закончил свою работу\n");
    pthread_exit((void*)0);
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
        if(!pthread_mutex_timedlock(args->mid, &tp))
        {
            for (int i = 0; i < 5; i++)
            {
                printf("%c", args->sym);
                fflush(stdout);
                sleep(1);
            }
            pthread_mutex_unlock(args->mid);
        }
        sleep(1);
    }
    printf("\nПоток 2 закончил свою работу\n");
    pthread_exit((void*)0);
    return nullptr;
}

int main()
{
    printf("\nПрограмма начала работу\n");
    pthread_mutex_t mid1;
    pthread_mutex_init(&mid1, NULL);
    args_t arg1 = {&mid1, 0, '1'}, arg2 = {&mid1, 0, '2'};
    pthread_t id1;
    pthread_t id2;
    pthread_create(&id1, NULL, proc1, &arg1);
    pthread_create(&id2, NULL, proc2, &arg2);
    printf("\nПрограмма ожидает нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");
    arg1.flag = 1;
    arg2.flag = 1;
    int *exitcode1, *exitcode2;
    pthread_join(id1, (void**)&exitcode1);
    pthread_join(id2, (void**)&exitcode2);
    printf("Потоки вернули значения: %p, %p",  exitcode1,  exitcode2);
    pthread_mutex_destroy(&mid1);
    printf("\nПрограмма закончила работу\n");
    return 0;
}
