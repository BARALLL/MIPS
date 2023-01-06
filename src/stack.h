#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_MEM 0xFFFFFFFF //32 bits

typedef struct mem
{
    int32_t data;
    uint32_t address;
    struct mem* next;
} stack;


stack* createMem();

void freeMem(stack* mem);