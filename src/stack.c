#include "stack.h"
                                                            //a finir...

stack* createMem() {
    stack* elem = malloc(sizeof(stack));
    if (elem)
    {
        elem->next = NULL;
        elem->data = 0;
        elem->address = -1;
    }
    else
        printf("MIPS Error 401: Memory not allocated");
    return elem;
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