#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <string.h>
#include <ncurses.h>
#include <netdb.h>


int main() 
{
    printf("\nПрограмма начала работу\n");
   
    int fd = shm_open("/shared", O_CREAT|O_RDWR, 0644);
    ftruncate(fd, NI_MAXHOST);
    char* addr = (char*)mmap(NULL, NI_MAXHOST, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    if(addr == MAP_FAILED)
    {
        printf("MAP_FAILED!\n");
        return 1;
    }
    sem_t* read_sem = sem_open("/read_sem", O_CREAT, 0644, 1);
    sem_t* write_sem =  sem_open("/write_sem", O_CREAT, 0644, 1); 
    
    fcntl(0, F_SETFL, O_NONBLOCK);
    while(getchar()!=10)
    { 
        sem_wait(write_sem);
        char buf[NI_MAXHOST];
        strcpy(buf, addr);
        printf("hostname: %s\n", buf);
        sem_post(read_sem);
        sleep(1);
    }
   
    close(fd);
    munmap(addr, NI_MAXHOST);
    shm_unlink("/shared"); 
    sem_close(read_sem);
    sem_close(write_sem);
    sem_unlink("/read_sem"); 
    sem_unlink("/write_sem"); 
    printf("\nПрограмма закончила работу\n");
    return 0;
}
