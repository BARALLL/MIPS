#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_MEM 0xFFFFFFFF //addresse sur 32 bits => 0xFFFFFFFF

typedef struct mem
{
    int32_t data;
    unsigned long int address;
    struct mem* next;
} memory;


memory* createMem();

void freeMem(memory* mem);

void checkMem(unsigned long int address);

int32_t getElem(memory* mem, unsigned long int address);

void insertElem(memory* mem, unsigned long int address, int32_t data);

int32_t getValue(memory* mem, unsigned long int address);

void showMem(memory* mem);

void insertValue(memory* mem, unsigned long int address, int32_t data);

void deleteElem(memory* mem, unsigned long int address);