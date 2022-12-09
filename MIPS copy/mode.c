#include "mode.h"

void modeInteractif() {
    instr instExec;
    char* currInstr = calloc(100, sizeof(char));
    if (!currInstr) {
        printf("MIPS Error 422: Memory was not allocated");
        exit(5);
    }
    printf("Veuillez entrez une instruction");
    while (strcmp(currInstr, "EXIT")) {
        scanf("%s", currInstr);
        instExec = translate(currInstr);
    }
    free(currInstr);
}

void modeFichier(FILE* file, FILE* outputFile) {
    int count = 0;
    instr instExec;
    char currInstr[30];
    while (fscanf(file, "%[^\n] ", currInstr) != EOF) {
        printf("Line %d is %s\n", count, currInstr);
        instExec = translate(currInstr);
        printf("Binary instruction is %s\n", instExec.compInstr);
        printf("Hexadecimal instruction is %x\n", instExec.compInstr);
        snprintf(currInstr, sizeof(currInstr), "%zu", instExec.compInstr); //size_t to str
        fputs(currInstr, outputFile); //write in output file
        count++;
    }
}