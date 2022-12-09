#include "tools.h"
#include "translate.c"

instr translate(char* sInstr)
{
	instr inst;
	inst.inst = sInstr;
	getOpcode(&inst);
	makeInstr(&inst);
	return inst;
}