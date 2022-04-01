#include "clargs.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

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


    




    return 0;
}
