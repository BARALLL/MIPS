#include "tools.h"

instr translate(char* sInstr)
{
	instr inst;
	inst.inst = sInstr;
	getOpcode(&inst);
	makeInstr(&inst);
	return inst;
}

void exec(size_t opcode)
{
	//mem
}