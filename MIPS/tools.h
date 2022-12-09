#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct
{
	char* inst;
	char format;
	size_t opcode;
	int32_t compInstr;

} instr;

instr translate(char* sInstr);

void exec(size_t opcode);

//instr* listInstr =
//{"ADDI",}
