#include "clargs.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static const char* helpStr=
"Usage: %s [OPTIONS]\n\n"
"Options:\n"
"-h\t\t Print help and exit\n"
"-p PORT\t\t Port to listen. Default: 8080\n"
"-d DIR\t\t Directory for saving files. Default: ./\n"
;



void parseArgs(struct clArgs* args, int argc, char *argv[])
{

    args->port=NULL;
    args->dir=NULL;

    int opt;
    while ((opt=getopt(argc,argv,":p:d:h"))!=-1)
    {
        switch (opt)
        {

            case 'p':

                if (args->port)
                {
                    puts("The option -p was specified more than once");
                    printf(helpStr,argv[0]);
                    exit(EXIT_FAILURE);
                }

                args->port=optarg;
                break;
            
            case 'd':

                if (args->dir)
                {
                    puts("The option -d was specified more than once");
                    printf(helpStr,argv[0]);
                    exit(EXIT_FAILURE);
                }

                args->dir=optarg;
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

    if (!args->port) args->port="8080";
    if (!args->dir) args->dir="./";

}
