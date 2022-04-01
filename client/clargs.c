#include "clargs.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const char* helpStr=
"Usage: %s [OPTIONS] [--] IP PORT FILEPATH\n\n"
"Options:\n"
"-h\t\t Print help and exit\n"
"-n FILENAME\t The name of the file under which it will be created on the server\n\n"
"Positional arguments:\n"
"IP\t\t Server IP\n"
"PORT\t\t Port to connect to\n"
"FILEPATH\t Path of the file to send\n"
;

char *extractFilename(char* filepath)
{
    //Extracts filename from filepath. Asserts that filepath is correct ( open() succeeds).

    char *filename=filepath;
    while (*filepath!='\0')
    {
        if (*filepath=='/')
            filename=filepath+1;
        ++filepath;
    }

    return filename;
}

static void validateFilename(char *filename)
{
    
    if (filename[0]=='\0')
    {
        puts("Empty argument for the option -n");
        exit(EXIT_FAILURE);
    }
    
    int k=0;
    while (*filename!='\0')
    {
        
        if (*filename=='/')
        {
            puts("Forbidden character '/' in the filename");
            exit(EXIT_FAILURE);
        }
        ++k;
        ++filename;
    }

    if (k>255)
    {
        puts("Filename can't be longer than 255 characters");
        exit(EXIT_FAILURE);
    }
    
}

void parseArgs(struct clArgs* args,int argc, char *argv[])
{

    args->filename=NULL;
    args->ip=NULL;
    args->port=NULL;
    args->filepath=NULL;

    int opt;
    while ((opt=getopt(argc,argv,":n:h"))!=-1)
    {

        switch (opt)
        {

            case 'n':

                if (args->filename)
                {
                    puts("The option -n was specified more than once");
                    printf(helpStr,argv[0]);
                    exit(EXIT_FAILURE);
                }

                validateFilename(optarg);
                args->filename=optarg;
                break;

            case 'h': 
                printf(helpStr,argv[0]);
                exit(EXIT_SUCCESS);

            case '?':
                printf("Unrecognized option: -%c\n",optopt);
                printf(helpStr,argv[0]);
                exit(EXIT_FAILURE);

            case ':':
                printf("Missing an argument for the option: -%c\n",optopt);
                printf(helpStr,argv[0]);
                exit(EXIT_FAILURE);


        }
    
    }


    if (argv[optind]!=NULL)
        args->ip=argv[optind++];
    else
    {
        puts("Missing positional argument IP");
        printf(helpStr,argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if (argv[optind]!=NULL)
        args->port=argv[optind++];
    else
    {
        puts("Missing positional argument PORT");
        printf(helpStr,argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argv[optind]!=NULL)
        args->filepath=argv[optind++];
    else
    {
        puts("Missing positional argument FILEPATH");
        printf(helpStr,argv[0]);
        exit(EXIT_FAILURE);
    }


    // if (args->filename==NULL)
    // {
    //     puts("[*] Filename is empty");
    // }

    // puts("Arguments:");
    // if (args->filename) puts(args->filename);
    // puts(args->ip);
    // puts(args->port);
    // puts(args->filepath);


}