#pragma once
#include "tools.h"

instr translate(char* sInstr)
{
	instr inst;
	inst.inst = sInstr;
	inst.format = getType(inst);
	inst.opcode = getOpcode(inst);
	inst.compInstr = makeInstr(inst);
	return inst;
}

void exec(size_t opcode)
{
	//mem
}