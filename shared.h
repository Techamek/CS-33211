#ifndef SHARED_H_
#define SHARED_H_

#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct table {
    int buffer[2];
    int in;
    int out;
};

static const int SIZE = sizeof(struct table);
static const char *NAME = "ProdConsumnProb";
static const char *semFullName = "full";
static const char *semEmptyName = "empty";
static const char *semMutexName = "mutex";

#endif //SHARED_H_
