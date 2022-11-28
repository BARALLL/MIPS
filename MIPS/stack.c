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

memory* createMem() {
    memory* elem = malloc(sizeof(memory));
    elem->next = NULL;
    elem->data = 0;
    elem->address = -1;
    return elem;
}

void freeMem(memory* mem)
{
    memory* nextElem = mem;
    memory* current = mem;
    while (current != NULL)
    {
        nextElem = current->next;
        free(current);
        current = nextElem;
    }
}

void checkMem(unsigned long int address) {
    if (address > MAX_MEM) {
        printf("MIPS Error 420: Memory Overflow");
        exit(3);
    }
    else if (address % sizeof(int32_t) != 0) {
        printf("MIPS Error 421: Memory alignment not valid");
        exit(4);
    }
}

int32_t getElem(memory* mem, unsigned long int address)
{
    bool done = false;
    memory* elem = mem;
    int32_t value;

    while (!done && elem->next != NULL)
    {
        elem = elem->next;
        if (elem->address == address)
            done = true;
    }

    if (done)
        value = elem->data;
    else
        value = (int32_t)(rand() % 0xFF);
    return value;
}

void insertElem(memory* mem, unsigned long int address, int32_t data)
{
    memory* elemAdd = mem;
    while (elemAdd->next != NULL && (elemAdd->next)->address < address)
        elemAdd = elemAdd->next;

    if (elemAdd->next != NULL && (elemAdd->next)->address == address)
        (elemAdd->next)->data = data;
    else 
    {
        memory* newElem = (memory*)malloc(sizeof(memory));
        newElem->data = data;
        newElem->address = address;
        newElem->next = elemAdd->next;
        elemAdd->next = newElem;
    }
}


int32_t getValue(memory* mem, unsigned long int address) 
{
    checkMem(address);
    return getElem(mem, address);
}


void showMem(memory* mem)
{
    memory* elem = mem;
    while (elem->next != NULL)
    {
        elem = elem->next;
        printf("0x%08lX=>0x%08u\n", elem->address, elem->data);
    }
}


void insertValue(memory* mem, unsigned long int address, int32_t data) 
{
    checkMem(address);
    insertElem(mem, address, data);
}


void deleteElem(memory* mem, unsigned long int address)
{
    memory* bLastElem = mem;
    while (bLastElem->next != NULL && (bLastElem->next)->address < address)
        bLastElem = bLastElem->next;
    if ((bLastElem->next)->address == address)
    {
        memory* elem = bLastElem->next;
        bLastElem->next = elem->next;
        free(elem);
    }

}