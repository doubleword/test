#pragma once

struct clArgs
{
    char *port;
    char *dir;
};


void parseArgs(struct clArgs*,int,char*[]);
