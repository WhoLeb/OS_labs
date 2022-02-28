#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ncurses.h>

int main() 
{
    printf("\nПрограмма начала работу\n");
 
    sem_t* main_sem;
    if ((main_sem = sem_open("/main_sem", O_CREAT, 0600, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        return -1;
    }
    std::ofstream output_file("output_file", std::fstream::app); 
    fcntl(0, F_SETFL, O_NONBLOCK);
    while(getchar()!=10)
    {  
        sem_wait(main_sem);
        for(int i = 0; i < 10; i++) 
        {
            output_file << '2';
            output_file.flush();
            printf("2");
            fflush(stdout);
            sleep(1);
        }
        sem_post(main_sem);
        sleep(1);
    }
    
    output_file.close();
    sem_close(main_sem);
    sem_unlink("/main_sem"); 
    printf("\nПрограмма закончила работу\n");
    return 0;
}
