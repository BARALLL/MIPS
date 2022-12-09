#pragma once
#include "tools.h"

uint8_t reg[];

void getOpCode(instr* inst)
{
	bool found = -1;
	char* strName = calloc(8, sizeof(char));
	char* instrNameTab[] = {"ADDI", "BEQ", "BGTZ", "BLEZ", "BNE", "J", "JAL", "LUI", "LW", "SW"};
	uint8_t* instrCodeTab[] = {0b001000, 0b000100, 0b000111, 0b000110, 0b000101, 0b000010, 0b000011, 0b001111, 0b100011, 0b101011};


	char* instrCodeRTab[] = { "ADD", "AND", "DIV", "JR", "MFHI", "MFLO", "MULT", "OR", "ROTR", "SLL", "SLT", "SRL", "SUB", "SYSCALL", "XOR"};
	uint8_t* instrNameRTab[] = { 0b100000, 0b100100, 0b011010, 0b001000, 0b010000, 
		0b010010, 0b011000, 0b100101, 0b000010, 0b000000,  0b101010, 0b000010, 0b100010, 0b001100, 0b100110 };



	if (!strName)
	{
		printf("MIPS Error 422: Memory was not allocated");
		exit(5);
	}

	for (int i = 0; inst->inst[i] != " "; i++)
		strName[i] = inst->inst[i];

	for (int i = 0; i < 10 && found != -1; i++)
	{
		if (strcmp(strName, instrNameTab[i]))
			found = i;
	}
	if (found == -1)
	{
		inst->format = 'R';
		for (int i = 0; i < 15 && found != -1; i++)
		{
			if (strcmp(strName, instrNameRTab[i]))
				found = i;
		}
	}

	if (found == -1)
	{
		printf("MIPS Error 450: Instruction was not found");
		exit(5);
	}
	else
	{
		if (instrNameRTab[found][0] == 'J')
			inst->format = 'J';
		else
			inst->format = 'I';

		if (inst->format == 'R')
			inst->opcode = (size_t)instrCodeRTab[found];
		else
			inst->opcode = (size_t)instrCodeTab[found];
	}
}


void makeInstr(instr* inst)						//change to big switch and supporting functions
{
	switch (inst->opcode)
	{
	case (int32_t)100000:

	default:
		break;
	}





	char* curr = callloc(2, sizeof(char));
	if (inst->format == 'R')
		inst->compInstr = ((size_t)"000000" << 26) | inst->compInstr;
	else	
		inst->compInstr = (inst->opcode << 26) | inst->compInstr;

	char* buffer = inst->inst;
	int i = 0, j = 0;

	for (i = 0, j = 0; inst->inst[i] != 0; i++)
	{
		if (buffer[i] == ' ' && buffer[i] != NULL)
		{
			for (j = i; j < inst->inst[i] != 0; j++)
				buffer[j] = buffer[j + 1];
		}
	}


	int i = 0;
	int j = 0;
	int posInstr = 26;
	while (inst->inst[i] == ' ')
		i++;

	while (inst->inst[i] == 0)
	{
		if (inst->inst[i] == '$' || inst->inst[i] != ',')
		{
			i++;
			while (inst->inst[i] != ',' && j < 3)
			{
				curr[j] = inst->inst[i];
				i++;
			}
				
			j = 0;
			inst->compInstr = ((size_t)curr << posInstr - 5) | inst->compInstr;		//200 or 002?	int or hex?
			posInstr -= 3;
		}
	}

	//inst->compInstr = (inst->opcode << 26) | inst->compInstr;
	/*
	for (int i = 0; i< 8; i++) {
		inst->compInstr = noOut | (int)binIn[i] << i;		//special
	}
	*/
}


void makeMIPSInstruction(instr inst, uint8_t rd, uint8_t rs, uint8_t rt)
{
	uint8_t opcode = inst.opcode;
	uint32_t compInst;

	char type = inst.format;
	
	switch (type) {
	
	case 'R':
		compInst = 0b000000;
		compInst = (compInst << 5) & rs;
		compInst = (compInst << 5) & rt;
		compInst = (compInst << 5) & rd;
		compInst = (compInst << 5) & opcode;


		// Etc. for other MIPS instructions...
	case 'J':    // SUB
		if (opcode = 0b000010)
		{

		}
		else
		{

		}
		break;
	}
	case 'I':
		break;
}

void getParam(uint32_t inst, uint8_t rs, uint8_t rd, uint8_t rt)
{
	// Get the instruction parameters by masking the irrelevant bits
	// and shifting them to the correct position.
	rs = (inst >> 21) & 0b11111;
	rt = (inst >> 16) & 0b11111;
	rd = (inst >> 11) & 0b11111;
}


void execMIPSInstruction(uint32_t inst) {
	uint8_t rs;
	uint8_t rd;
	uint8_t rt;

	// Get the opcode of the instruction by taking the first 6 bits.
	uint8_t opcode = inst >> 26;
	uint8_t special = (uint8_t)inst;

	switch (opcode) {
		// type R
	case 0b000000:
		switch (special) {
			getParam(inst, &rs, &rd, &rt);
		case 0b100000:    // ADD
			// Execute in registers
			reg[rd] = reg[rs] + reg[rt];
			break;
		case 0b100010:    // SUB
			reg[rd] = reg[rs] - reg[rt];
			break;
		case 0b100100:    // AND
			reg[rd] = reg[rs] & reg[rt];
			break;
		case 0b100101:    // OR
			reg[rd] = reg[rs] | reg[rt];
			break;
		case 0b011000:    // DIV
			reg[rd] = reg[rs] / reg[rt];
			break;
		case 0b100110:    // XOR
			reg[rd] = reg[rs] ^ reg[rt];
			break;
		}

		// Etc. for other MIPS instructions...
	case 0b000001:    // SUB
		break;
	}
}


int getFullInstruction(instr* inst) {
	// Déclaration de l'instruction ADD en langage assembleur MIPS.
	// Cette instruction ajoute le contenu des registres $s1 et $s2 et stocke le résultat dans $s3.
	char* instStr = inst->inst;

	// Extraction des arguments des registres de l'instruction en tant que chaîne de caractères.
	// Nous utilisons la fonction strtok pour découper la chaîne en différents tokens en utilisant l'espace comme séparateur.
	char* name = strtok(instStr, " ");  // Récupère "ADD"
	char* rs = strtok(NULL, " ");  // Récupère "$s1"
	char* rt = strtok(NULL, " ");  // Récupère "$s2"
	char* rd = strtok(NULL, " ");  // Récupère "$s3"

	// Conversion des arguments des registres en uint32_t.
	// Nous utilisons la fonction strtol pour convertir les chaînes de caractères en entiers.
	uint32_t rsValue = strtol(rs, NULL, 10);  // Convertit "$s1" en 21.
	uint32_t rtValue = strtol(rt, NULL, 10);  // Convertit "$s2" en 16.
	uint32_t rdValue = strtol(rd, NULL, 10);  // Convertit "$s3" en 11.

	// Affiche les valeurs des arguments des registres.
	printf("rsValue = %u\n", rsValue);
	printf("rtValue = %u\n", rtValue);
	printf("rdValue = %u\n", rdValue);

	
	execMIPSInstruction(inst, rs, rd, rt);
}