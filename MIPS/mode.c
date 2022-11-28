#include "tools.h"
#include "stack.c"



void modeInteractif()
{
	instr instExec;
	char* currInstr = " ";
	printf("Veuillez entrez une currInstruction");
	while (!strcmp(currInstr, "EXIT"))
	{
		scanf("%s", &(*currInstr));
		instExec = translate(currInstr);
	}
}


void modeFichier(FILE* file, FILE* outputFile)
{
	int count = 0;
	instr instExec;
	char* currInstr = calloc(30, sizeof(char));

	while (fscanf(file, "%[^\n] ", currInstr) != EOF)
	{
		printf("Line %d is %s\n", count, currInstr);
		instExec = translate(currInstr);
		printf("Binary instruction is %x\n", instExec.compInstr);
		snprintf(currInstr, sizeof(currInstr), "%zu", instExec.compInstr);		//size_t to str
		fputs(&currInstr, outputFile);										//write in output file
		count++;
	}
}