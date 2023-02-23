#include "stack.h"


stack* createMem() {
    stack* elem = malloc(sizeof(stack));
    if (elem)
    {
        elem->next = NULL;
        elem->data = 0;
        elem->address = (int32_t)(rand() % 0xFF);;
    }
    else
        printf("MIPS Error 401: Memory not allocated");
    return elem;
}

void showMem(stack* mem)
{
    stack* elem = mem;
    while (elem->next != NULL)
    {
        elem = elem->next;
        printf("0x%08u: 0x%08u\n", (uint32_t)elem->address, elem->data);
    }
}

void freeMem(stack* mem)
{
    stack* nextElem = mem;
    stack* current = mem;
    while (current != NULL)
    {
        nextElem = current->next;
        free(current);
        current = nextElem;
    }
}

void validMem(uint32_t address) {
    if (address > MAX_MEM) {
        printf("MIPS Error 420: Memory Overflow\n");
        exit(3);
    }
    else if (address % sizeof(uint32_t) != 0) {
        printf("MIPS Error 421: stack alignment not valid\n");
        exit(4);
    }
}

int32_t getElem(stack* mem, uint32_t address, char* type)
{
    validMem(address);
    bool done = false;
    stack* elem = mem;
    int32_t value;

    while (!done && elem->next != NULL)
    {
        elem = elem->next;
        if (elem->address == address)
            done = true;
    }

    if (done)
    {
        value = elem->data;
        *type = elem->type;
    }
    else
    {
        value = (int32_t)(rand() % 0xFF);   //simulation acces memoire si la donnee n'est pas presente
        *type = 'N';
    }
    return value;
}


void insertElem(stack* mem, uint32_t address, int32_t data, char type)
{
    validMem(address);
    stack* elemAdd = mem;
    if (elemAdd)
    {
        while (elemAdd->next != NULL && (elemAdd->next)->address < address)
            elemAdd = elemAdd->next;

        if (elemAdd->next != NULL && (elemAdd->next)->address == address)
        {
            (elemAdd->next)->data = data;
            (elemAdd->next)->type = type;
        }
        else
        {
            stack* newElem = (stack*)malloc(sizeof(stack));
            if (newElem)
            {
                newElem->data = data;
                newElem->address = address;
                newElem->type = type;
                newElem->next = elemAdd->next;
                elemAdd->next = newElem;
            }
            else
                printf("MIPS Error 401: Memory not allocated\n");
        }
    }
    else
        printf("MIPS Error 401: Memory not allocated\n");
}




void deleteElem(stack* mem, uint32_t address)
{
    stack* bLastElem = mem;
    stack* elem;
    if (bLastElem)
    {
        while (bLastElem->next != NULL && (bLastElem->next)->address < address)
            bLastElem = bLastElem->next;
        if ((bLastElem->next)->address == address)
        {
            elem = bLastElem->next;
            bLastElem->next = elem->next;
            free(elem);
        }
    }
    else
        printf("MIPS Error 401: Memory not allocated\n");
}