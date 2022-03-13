#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <ncurses.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>

bool flag = true;

void sig_handler(int signo)
{
    printf("Считывающая программа закрылась раньше записывающей!\n");
    sleep(1);
}

int main() 
{
    printf("\nЗаписывающая программа начала работу\n");
    fcntl(0, F_SETFL, O_NONBLOCK);
    signal(SIGPIPE, sig_handler); 
    mkfifo("/tmp/named_pype", 0644);
    int fd = -1;
    while(!(fd+1)&&getchar()!=10)
    {
        fd = open("/tmp/named_pype", O_WRONLY|O_NONBLOCK);     
        if(fd == -1)
            printf("An error has occured: %s\n", strerror(errno));
        sleep(1);
    }
    int i = 1;
    while(fd!=-1&&getchar()!=10)
    { 
        struct addrinfo* result;
        char buf [NI_MAXHOST]; 
        getaddrinfo("www.example.com", NULL, NULL, &result);
        getnameinfo(result->ai_addr, result->ai_addrlen, buf, NI_MAXHOST, NULL, 0, 0);   
        write(fd, buf, NI_MAXHOST); 
        printf("hostname: %s, номер %d\n", buf,i);
        i++;
        sleep(1);
    }

    close(fd);
    unlink("/tmp/named_pype"); 
    printf("\nПрограмма закончила работу\n");
    return 0;
}
