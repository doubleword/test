#pragma once

struct clArgs
{
    char *port;
    char *dir;
    char *threads;
};


void parseArgs(struct clArgs*,int,char*[]);
