#pragma once
#include "tools.h"

size_t getOpCode(instr inst)
{
	size_t opCode;
	char* strCode = calloc(6, sizeof(char));
	for (int i = 0; inst.inst[i] != ' '; i++)
		strCode = inst.inst[i];

	/*
	switch (strCode)
	{
	default:
		break;
	}
	*/

	return opCode;
}

char getType(instr inst)
{
	char type;
	return type;
}

char makeInstr(instr inst)
{
	size_t compInstr;
	return compInstr;
}