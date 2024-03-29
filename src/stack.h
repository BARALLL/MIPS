#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_MEM 0xFFFFFFFF //addresse sur 32 bits => 0xFFFFFFFF

typedef struct mem
{
    int32_t data;
    uint32_t address;
    char type;
    struct mem* next;
} stack;


stack* createMem();

void freeMem(stack* mem);

void validMem(uint32_t address);

int32_t getElem(stack* mem, uint32_t address, char* type);

void insertElem(stack* mem, uint32_t address, int32_t data, char type);

void showMem(stack* mem);

void deleteElem(stack* mem, uint32_t address);