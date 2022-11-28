#pragma once
#include <stdlib.h>

typedef struct
{
	char* inst;
	int format;
	size_t opcode;
	size_t compInstr;

} instr;

instr translate(char* sInstr);

void exec(size_t opcode);

//instr* listInstr =
//{"ADDI",}
