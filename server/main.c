#include "clargs.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define IP_ADDR "0.0.0.0"


void* handleConnection(void*);

int main(int argc, char *argv[])
{

    struct clArgs args;
    parseArgs(&args,argc,argv);

    if (chdir(args.dir)==-1)
    {
        perror("Chdir error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in srvaddr;
    srvaddr.sin_family=AF_INET;
    
    int p=atoi(args.port);
    if (p>=1 && p<=0xffff)
        srvaddr.sin_port=htons( (unsigned short) p);
    else
    {
        puts("Invalid port number");
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET,IP_ADDR,&srvaddr.sin_addr)==0)
    {
        puts("Invalid IPv4 address");
        exit(EXIT_FAILURE);
    }


    int skt;
    if ((skt=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    int optval=1;
    if (setsockopt(skt,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval))==-1)
    {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    if (bind(skt,(struct sockaddr *)&srvaddr,sizeof(srvaddr))==-1)
    {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    if (listen(skt,10)==-1)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }


    for (;;)
    {
        intptr_t cnt;
        if ((cnt=accept(skt,NULL,NULL))==-1)
        {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }
    
        pthread_t tid;
        int err;

        err=pthread_create(&tid,NULL,handleConnection,(void*)cnt);
        if (err!=0)
        {
            printf("Pthread_create error: %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
    
        if ((err=pthread_detach(tid))!=0)
        {
            printf("Pthread_detach error: %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
    
    }

    return 0;
}

