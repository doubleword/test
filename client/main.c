#include "clargs.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/sendfile.h>


int main(int argc,char *argv[])
{

    struct clArgs args;
    parseArgs(&args,argc,argv);
    
    int fd=open(args.filepath,O_RDONLY);
    if (fd==-1)
    {
        perror("Open error");
        exit(EXIT_FAILURE);
    }

    if (args.filename==NULL)
        args.filename=extractFilename(args.filepath);
    
    //printf("Main: %s\n",args.filename);


    struct sockaddr_in srvaddr;
    srvaddr.sin_family=AF_INET;
    
    int p=atoi(args.port);
    if (p>=1 && p<=0xffff)
        srvaddr.sin_port=htons( (unsigned short) p);
    else
    {
        puts("Invalid port number");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET,args.ip,&srvaddr.sin_addr)==0)
    {
        puts("Invalid IPv4 address");
        close(fd);
        exit(EXIT_FAILURE);
    }


    int skt;
    if ((skt=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("Socket error");
        close(fd);
        exit(EXIT_FAILURE);
    }


    if (connect(skt,(struct sockaddr*)&srvaddr,sizeof(srvaddr))==-1)
    {
        perror("Connect error");
        close(fd);
        close(skt);
        exit(EXIT_FAILURE);
    }

    

    unsigned char header[256];
    header[0]=0;
    while (*(args.filename+header[0])!='\0')
        ++header[0];

    strncpy(header+1,args.filename,header[0]);

    if (write(skt,header,1+header[0])==-1)
    {
        perror("Write error");
        close(fd);
        close(skt);
        exit(EXIT_FAILURE);
    }




    ssize_t numWrote;
    while ((numWrote=sendfile(skt,fd,NULL,0x7ffff000))>0)
        continue;
    
    if (numWrote==-1)
    {
        perror("Sendfile error");
        close(fd);
        close(skt);
        exit(EXIT_FAILURE);
    }


    close(fd);
    close(skt);
    puts("Sent.");
    return 0;
}
