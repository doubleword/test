#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>

#define BUF_SZ 1024

/*

Message format:

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  filename length (char: 1-255)  | filename (array of chars: 1-255)  | file binary data  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/


void* handleConnection(void *arg)
{

    int cnt=(int)arg;
    unsigned char filenameLength;
    
    // Read filename length from the socket
    ssize_t numRead;
    if ((numRead=read(cnt,&filenameLength,1))<=0)
    {
        if (numRead==-1)
            perror("Read error");
        close(cnt);
        return NULL;
    }

    if (filenameLength==0)
    {
        write(cnt,"Invalid filename length!",25);
        close(cnt);
        return NULL;
    }

    // Read filename from the socket
    char filename[256];
    filename[filenameLength]='\0';

    unsigned char bytesRead=0;
    while (bytesRead<filenameLength)
    {
        numRead=read(cnt,filename+bytesRead,filenameLength-bytesRead);
        
        if (numRead<=0)
            break;

        bytesRead+=numRead;

    }

    if (numRead==-1 || bytesRead<filenameLength)
    {
        if (numRead==-1)
            perror("Read error");
        close(cnt);
        return NULL;
    }

    // Check if filename contains forbidden characters: "/","\0"
    for (int i=0;i<filenameLength;++i)
        if (filename[i]=='/' || filename[i]=='\0')
        {
            write(cnt,"Forbidden characters in the filename!",38);
            close(cnt);
            return NULL;
        }


    // Read data from the socket and save it to a file 
    int fd=open(filename,O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR);
    if (fd==-1)
    {
        perror("Open error");
        close(cnt);
        return NULL;
    }

    // Forbid clients to write to the same file simultaneously
    if (flock(fd,LOCK_EX)==-1)
    {
        perror("Flock error");
        close(cnt);
        close(fd);
        return NULL;
    }

    char *buf=malloc(BUF_SZ);
    if (buf==NULL)
    {
        puts("Malloc error");
        close(cnt);
        close(fd);
        return NULL;
    }


    // Replace file contents if it already exists
    if (ftruncate(fd,0)==-1)
    {
        perror("Ftruncate error");
        close(cnt);
        close(fd);
        free(buf);
        return NULL;
    }

    while ((numRead=read(cnt,buf,BUF_SZ))>0)
    {
        if (write(fd,buf,numRead)==-1)
        {
            perror("Write error");
            close(cnt);
            close(fd);
            free(buf);
            return NULL;
        }
    }

    if (numRead==-1)
    {
        perror("Read error");
        close(cnt);
        close(fd);
        free(buf);
        return NULL;
    }

    
    close(fd); //Releases the file lock


    close(cnt);
    free(buf);
    return NULL;
}