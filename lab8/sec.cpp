#include <stdio.h>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::queue;

queue<char*> res_q;
int ser_sock;
int flag_con = 1, flag_send = 1, flag_rec = 1;
pthread_t m_con, m_send, m_rec;
pthread_mutex_t mid1;

void* sen(void* a)
{
    int i = 1;
    while(flag_send)
    {
        send(ser_sock, &i, sizeof(i), 0);
        printf("Запрос №%d сформирован", i);
        sleep(1);
    }
}

void* rec(void* a)
{
    int i = 1;
    while(flag_rec)
    {
        char rcvbuf[NI_MAXHOST];
        int reccount = recv(ser_sock, rcvbuf, sizeof(rcvbuf),0);
        if (reccount == -1) 
        {
            perror("recv");
            sleep(1);
        }
        else if (reccount == 0) 
        {
            //разъединение;
            shutdown(ser_sock, SHUT_RDWR);
            sleep(1);
        }
        else
        {
            printf("Запрос №%d сформирован", i);
            i++;    
        }
    } 
}

void* conn(void*)
{
    while(flag_con)
    {
        struct sockaddr_in serverAddr;
        serverAddr.sin_family =		AF_INET;
        serverAddr.sin_port =		htons(7000);
        serverAddr.sin_addr.s_addr =	inet_addr("127.0.0.1");

        int res = connect(ser_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if(res == 0)
        {
            pthread_create(&m_send, NULL, sen, NULL);
            pthread_create(&m_rec, NULL, rec, NULL);
            pthread_exit(NULL);
        }
    }
    pthread_exit((void*)1);
}

int main()
{
    printf("Программа-клиент начала работу"); 
    
    ser_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    pthread_mutex_init(&mid1,NULL); 
    pthread_t m_await;
    pthread_create(&m_con, NULL, conn, NULL);
    
    getchar();

    flag_con = 0;
    flag_rec = 0;
    flag_send = 0;
    pthread_join(m_send, NULL);
    pthread_join(m_rec, NULL);
    pthread_join(m_con, NULL);

    shutdown(ser_sock, SHUT_RDWR); 
    close(ser_sock);
} 
