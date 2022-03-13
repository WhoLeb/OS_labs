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

int main() 
{
    printf("\nСчитывающая программа начала работу\n");
    mkfifo("/tmp/named_pype", 0644);
    int fd = open("/tmp/named_pype", O_RDONLY|O_NONBLOCK);
    int i = 1; 
    fcntl(0, F_SETFL, O_NONBLOCK);
    while(getchar()!=10)
    { 
        char buf [NI_MAXHOST] = "\0"; 
        int ret = read(fd, buf, NI_MAXHOST); 
        if(ret==-1)
            printf("error: %s\n", strerror(errno));
        printf("hostname: %s, номер %d\n", buf, i);
        if(buf[0]!='\0')
            i++;
        sleep(1);
    }

    close(fd);
    unlink("/tmp/named_pype"); 
    printf("\nПрограмма закончила работу\n");
    return 0;
}
