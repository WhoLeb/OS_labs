#include <queue>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using std::queue;

int list_sock;
int cli_sock;
queue<int> req_q;
int flag_acc = 1, flag_proc = 1, flag_wait = 1;
pthread_mutex_t mid1, mid2;
pthread_t m_recieve, m_proc;

void* recieve(void* a)
{
   while(flag_acc)
   {
       int* rcvbuf;
       int reccount = recv(list_sock, rcvbuf, sizeof(rcvbuf),0);
       if (reccount == -1) 
       {
           perror("recv");
           sleep(1);
       }
       else if (reccount == 0) 
       {
           //разъединение;
           shutdown(list_sock, SHUT_RDWR);
           sleep(1);
       }
       else
       {
           pthread_mutex_lock(&mid1);
           req_q.push(*rcvbuf);
           pthread_mutex_unlock(&mid1);
       }
   } 
}

void* proc(void* a)
{
    int i = 1;
    while(flag_proc)
    {
        pthread_mutex_lock(&mid1);
        if(!req_q.empty())
        {
            int num = req_q.back(); 
            req_q.pop();
            pthread_mutex_unlock(&mid1);

            struct addrinfo* result;
            char buf [NI_MAXHOST]; 
            getaddrinfo("www.example.com", NULL, NULL, &result);
            getnameinfo(result->ai_addr, result->ai_addrlen, buf, NI_MAXHOST, NULL, 0, 0);   
            
            int sentcount = send(list_sock, buf, NI_MAXHOST, 0);
            if(sentcount == -1)
            {
                perror("send");
            }
            else
            { 
                printf("hostname: %s, номер %d\n", buf,i);
                i++;
            }
        }
        else
        {
            pthread_mutex_unlock(&mid1);
            sleep(1);
        }
    }    
}

void* await(void* a)
{
    struct sockaddr_in addr;
    while(flag_wait)
    {
        socklen_t addrlen = 0;
        int list_sock = accept(cli_sock,(struct sockaddr*)&addr, &addrlen);
        if (list_sock == -1) {
            perror("accept");
            sleep(1);
        }else{
            pthread_create(&m_recieve, NULL, recieve, NULL); 
            pthread_create(&m_proc, NULL, proc, NULL);
            pthread_exit(NULL); 
        }
    }
    pthread_exit((void*)1);
} 

int main()
{

    printf("Сервер начал работу\n");
    cli_sock = socket(AF_INET, SOCK_STREAM, 0);
   
    struct sockaddr_in serverAddr;
    serverAddr.sin_family =		AF_INET;
    serverAddr.sin_port =		htons(7000);
    serverAddr.sin_addr.s_addr =	inet_addr("127.0.0.1");

    bind(cli_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(cli_sock, sizeof(int));
    int optval = 1;
    setsockopt(cli_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
   
    pthread_mutex_init(&mid1,NULL); 
    pthread_t m_await;
    pthread_create(&m_await, NULL, await, NULL);

    getchar();

    flag_acc = 0;
    flag_proc = 0;
    flag_wait = 0;
    pthread_join(m_await, NULL);
    pthread_join(m_recieve, NULL);
    pthread_join(m_proc, NULL);
    
    shutdown(list_sock, SHUT_RDWR); 
    shutdown(cli_sock, SHUT_RDWR);
    close(cli_sock);
    close(list_sock);
}
