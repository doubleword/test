#pragma once

#include <pthread.h>



extern int activeConnections;

extern pthread_mutex_t connMtx;
extern pthread_cond_t connCond;

void* handleConnection(void*);