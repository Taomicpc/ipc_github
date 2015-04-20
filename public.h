//main.h各文件公用的头文件
//
#ifndef _MAIN_H
#define _MAIN_H

#define NUM 10

#include "stdio.h"
#include "stdlib.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include "memory.h"
#include "stdbool.h"
#include "assert.h"
#include <sys/stat.h>

typedef struct
{
    int size;
    int dataUsed;
    int data[NUM];
    bool endflag;
    bool dealfinish;
}Data_Type;

extern bool chartoint(char* str, int* result);

#endif
