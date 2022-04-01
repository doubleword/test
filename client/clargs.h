#pragma once


struct clArgs
{
    char *filename;
    char *ip;
    char *port;
    char *filepath;
};


void parseArgs(struct clArgs*,int,char *[]);
char *extractFilename(char*);


