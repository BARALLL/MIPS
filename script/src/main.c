#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#pragma warning(disable : 4996)


int R[32];
uint32_t PC;
uint32_t HI, LO;

void execute(uint32_t instruction, char type, stack* mem) {
    uint32_t opcode, rs, rt, immediate, rd, shamt, special, target;
    int incr = 1;
    uint32_t tmp;

    if (type == 'R')
    {
        rd = (instruction & 0x0000F800) >> 11;
        rs = (instruction & 0x03E00000) >> 21; 
        rt = (instruction & 0x001F0000) >> 16;
        immediate = instruction & 0x0000FFFF;
        special = instruction & 0x000003F;
        shamt = (instruction & 0x000007C0) >> 6;


        if (special == 0x20) {              // ADD
            tmp = R[rs] + R[rt];
            if (((tmp >> 30) & 1) != ((tmp >> 31) & 1))
                printf("MIPS Error 201: Integer Overflow");
            else
                R[rd] = tmp;
        }
        else if (special == 0x22) {         //SUB
            tmp = R[rs] - R[rt];
            if (((tmp >> 30) & 1) != ((tmp >> 31) & 1))
                printf("MIPS Error 201: Integer Overflow");
            else
                R[rd] = tmp;
        }
        else if (special == 0x24) R[rd] = R[rs] & R[rt];     // AND
        else if (special == 0x24) R[rd] = R[rs] | R[rt];     // OR
        else if (special == 0x1A) {
            R[rt] = R[rs] / R[rd];         // DIV
            if (R[rd] == 0) 
                printf("MIPS Error 202: division by 0");
            else {
                LO = R[rs] / R[rt];
                HI = R[rs] % R[rt];
            }
        }
        else if (special == 0x18) {
            tmp = R[rs] * R[rt];
            LO = tmp & 0x0000FFFF;      //MULT
            HI = tmp & 0xFFFF0000;
        }
        else if (special == 0x26) R[rd] = R[rs] ^ R[rt];         // XOR
        else if (special == 0x02) {
            if(instruction & (1 << 21)) //is 21th bit set to 1?
                R[rd] = (R[rt] << shamt) | (R[rt] >> (32 - shamt)); //ROTR
            else
                R[rd] = R[rt] >> shamt;        // SRL
        }
        else if (special == 0x00) R[rd] = R[rt] << shamt;        // SLL
        else if (special == 0x08) PC = R[rs];                     // JR
        else if (special == 0x10) PC = HI;
        else if (special == 0x12) PC = LO;
        printf("Resultat : %d\n", R[rd]);
    }

    else if (type == 'J')
    {
        opcode = (instruction & 0xFC000000) >> 26;
        target = instruction & 0x3FFFFFF;
        
        if (opcode == 0x02) PC = (PC & 0xF0000000) | (target << 2); // J
        
        else if (opcode == 0x03) {                                  // JAL
            R[31] = PC;
            PC = (PC & 0xF0000000) | (target << 2);
        }
        incr = 0;
    }

    else if (type == 'I')
    {
        opcode = (instruction & 0xFC000000) >> 26;
        rd = (instruction & 0x0000F800) >> 11;
        rs = (instruction & 0x03E00000) >> 21;
        rt = (instruction & 0x001F0000) >> 16;
        immediate = instruction & 0x0000FFFF;


        if (opcode == 0x08 || opcode == 0x09) { // ADDI/ADDIU
            tmp = R[rs] + immediate;
            if (((tmp >> 30) & 1) != ((tmp >> 31) & 1))
                printf("MIPS Error 201: Integer Overflow");
            else
                R[rt] = tmp;
        }

        else if (opcode == 0x04) { // BEQ
            if (R[rs] == R[rt]) {
                PC += immediate * 4;    //mult par 4 au lieu de <<2 pour préserver le signe
                incr = 0;
            }
        }
        
        else if (opcode == 0x05) { // BNE
            if (R[rs] != R[rt]) {
                PC += immediate * 4;
                incr = 0;
            }
        }
        
        else if (opcode == 0x06) { // BLEZ
            if (R[rs] <= 0) {
                PC += immediate * 4;
                incr = 0;
            }
        }

        else if (opcode == 0x07) { // BGTZ
            if (R[rs] > 0) {
                PC += immediate * 4;
                incr = 0;
            }
        }

        else if (opcode == 0x23) { // LW
            /*      stack non finie...
            R[rt] = (getElem(mem, (uint32_t)(R[rs] + immediate) << 24)) | (getElem(mem, (uint32_t)(R[rs] + immediate + 1) << 16))
                | (getElem(mem, (uint32_t)(R[rs] + immediate + 2) << 8)) | getElem(mem, (uint32_t)(R[rs] + immediate + 3));
                */
        }

        else if (opcode == 0x2B) { // SW
            /*      stack non finie...
            insertElem(mem, (uint32_t)(R[rs] + immediate), R[rt] >> 24);
            insertElem(mem, (uint32_t)(R[rs] + immediate + 1), (R[rt] >> 16) & 0xFF);
            insertElem(mem, (uint32_t)(R[rs] + immediate + 2), (R[rt] >> 8) & 0xFF);
            insertElem(mem, (uint32_t)(R[rs] + immediate + 3), (R[rt]) & 0xFF);
            */
        }

        else if (opcode == 0x2A) { // SLT
            R[rd] = (R[rs] < R[rt]) ? 1 : 0;
        }

        else if (opcode == 0x0F) { // LUI
            R[rt] = immediate << 16;
        }
    }

    PC += 4 * incr; //+4 si pas de saut, 0 sinon
}


bool arg(char* instruction, char* tmp, int* index)
{
    int i = *index;
    bool isarg = true;
    int tmpsize = 0;
    while (instruction[i] != '\0' && instruction[i] != '\n' && instruction[i] != '$' && !(instruction[i] > '0' && instruction[i] < '9'))
        i++;

    if (instruction[i] == '\0' || instruction[i] == '\n')
        isarg = false;
    else 
    {
        if (instruction[i] == '$')
            i++;
        while (instruction[i] >= '0' && instruction[i] <= '9')
        {
            tmpsize++;
            i++;
        }
        strncpy(tmp, &instruction[i-tmpsize], tmpsize);
        tmp[tmpsize] = '\0';
    }

    *index = i;
    return isarg;
}

uint32_t strToInt32(char* instruction, char* type) {

    // Parse the instruction and its arguments
    char* name = (char*)calloc(sizeof(instruction), sizeof(char));
    int i = 0;
    bool isarg1 = true;
    bool isarg2 = true;
    bool isarg3 = true;
    
    char* tmp = calloc(100, sizeof(char));

    for (i = 0; instruction[i] != '\0' && instruction[i] != '\n' && instruction[i] != ' '; i++)
        name[i] = instruction[i];
    name[i] = '\0';

    uint32_t arg1 = 0;
    uint32_t arg2 = 0;
    uint32_t arg3 = 0;

    isarg1 = arg(instruction, tmp, &i);
    if(isarg1) arg1 = atoi(tmp);

    isarg2 = arg(instruction, tmp, &i);
    if (isarg2) arg2 = atoi(tmp);

    isarg3 = arg(instruction, tmp, &i);
    if (isarg3) arg3 = atoi(tmp);

    uint32_t opcode = 0;
    uint32_t compInstr = 0;

    if (!name)
    {
        printf("MIPS Error 504: Incorrect instruction\n");
        return -1;
    }

    // Set the opcode in the instruction
    if (strcmp(name, "J") == 0)
    {
        opcode = 0x02;
        *type = 'J';
        if (!isarg1)
        {
            printf("MIPS Error 506: Incorrect arguments for function : %x\n", opcode);
            return -1;
        }
        compInstr = (opcode << 26) | arg1;
    }
    else if (strcmp(name, "JAL") == 0)
    {
        opcode = 0x03;
        *type = 'J';
        if (!isarg1)
        {
            printf("MIPS Error 506: Incorrect arguments for function : %x\n", opcode);
            return -1;
        }
        compInstr = (opcode << 26) | arg1;
    }
    else if (strcmp(name, "ADDI") == 0) opcode = 0x08;
    else if (strcmp(name, "ADDIU") == 0) opcode = 0x09;
    else if (strcmp(name, "BEQ") == 0) opcode = 0x04;
    else if (strcmp(name, "BGTZ") == 0) opcode = 0x07;
    else if (strcmp(name, "BLEZ") == 0) opcode = 0x06;
    else if (strcmp(name, "BNE") == 0) opcode = 0x05;
    else if (strcmp(name, "LUI") == 0) opcode = 0x0F;
    else if (strcmp(name, "LW") == 0) opcode = 0x23;
    else if (strcmp(name, "SW") == 0) opcode = 0x2B;
    else
    {
        *type = 'R';
        if (strcmp(name, "ADD") == 0) opcode = 0x20;
        else if (strcmp(name, "SUB") == 0) opcode = 0x22;
        else if (strcmp(name, "AND") == 0) opcode = 0x24;
        else if (strcmp(name, "OR") == 0) opcode = 0x25;
        else if (strcmp(name, "DIV") == 0) opcode = 0x1A;
        else if (strcmp(name, "MULT") == 0) opcode = 0x18;
        else if (strcmp(name, "MFHI") == 0) opcode = 0x10;
        else if (strcmp(name, "MFLO") == 0) opcode = 0x12;
        else if (strcmp(name, "XOR") == 0) opcode = 0x26;
        else if (strcmp(name, "SLL") == 0 || (strcmp(name, "NOP") == 0)) opcode = 0x00;         //NOP is SLL W/ only 0
        else if (strcmp(name, "SRL") == 0 || strcmp(name, "ROTR") == 0) opcode = 0x02;          //ROTR is SRL W/ 21th bit = 1
        else if (strcmp(name, "SLT") == 0) opcode = 0x2A;
        else if (strcmp(name, "JR") == 0) opcode = 0x08;
        else if (strcmp(name, "SYSCALL") == 0) opcode = 0x0C;
        else
        {
            printf("MIPS Error 501: Instruction not found\n");
            return -1;
        }

        /*
        if (!isarg1 || !isarg2 || !isarg3)
        {
            printf("MIPS Error 506: Incorrect arguments for function : %x", opcode);
            return -1;
        }
        */
        if ((strcmp(name, "SYSCALL") == 0))
            compInstr = (0x00 << 26) | (arg1 << 6) | opcode;
        else if((strcmp(name, "SLL") == 0))
            compInstr = (0x00 << 26) | (arg2 << 16) | (arg1 << 11) | (arg3 << 6) | opcode;
        else if(!isarg3 && isarg2 || strcmp(name, "JR") == 0)
            compInstr = (0x00 << 26) | (arg1 << 21) | (arg2 << 16) | (arg3 << 11) | opcode;
        else
            compInstr = (0x00 << 26) | (arg2 << 21) | (arg3 << 16) | (arg1 << 11) | opcode;
    }
    
    if (strcmp(name, "NOP") == 0) { compInstr = 0; }
    if (strcmp(name, "ROTR") == 0 || strcmp(name, "SRL") == 0) 
    {
        /*
        if (!isarg1 || !isarg2 || !isarg3)
        {
            printf("MIPS Error 506: Incorrect arguments for function : %x\n", opcode);
            return -1;
        }*/
        compInstr = (0x00 << 26) | (arg2 << 16) | (arg1 << 11) | (arg3 << 6) | opcode;      //special cases
        if(strcmp(name, "ROTR") == 0)
            compInstr = compInstr & ~(1 << 21);
    }

    if (*type != 'J' && *type != 'R')
    {
        /*
        if (!isarg1 || !isarg2)
        {
            printf("MIPS Error 506: Incorrect arguments for function : %x", opcode);
            return -1;
        } */
        
        if (strcmp(name, "LUI") == 0)
            compInstr = (opcode << 26) | (arg2 << 21) | (arg1 << 16) | arg2;
        else if (strcmp(name, "BEQ") == 0 || strcmp(name, "BGTZ") == 0 || strcmp(name, "BLEZ") == 0 || strcmp(name, "BNE") == 0)
        {
            compInstr = (opcode << 26) | (arg1 << 21) | (arg2 << 16) | arg3;
        }
        else
            compInstr = (opcode << 26) | (arg2 << 21) | (arg1 << 16) | arg3;
        *type = 'I';
    }

    printf("MIPS Hexadecimal instruction is %08x\n\n", compInstr);
    return compInstr;
}

int interactive_mode(stack* mem) {
    char* instr_str = calloc(64, sizeof(char));
    uint32_t compInstr = 0;
    char type;

    if (instr_str)
    {
        printf("Enter Instruction : ");
        fgets(instr_str, 64*sizeof(char), stdin);
        //instr_str[strcspn(instr_str, "\n")] = '\0';
        while (strcmp(instr_str, "EXIT")) { //EXIT...
            if (!strncmp(instr_str, "#", 1) == 0) {
                compInstr = strToInt32(instr_str, &type);
                if(compInstr != (uint32_t)-1)
                    //execute(compInstr, type, mem);
                    print(" ");
                else
                    printf("MIPS Error 500: Parsing Error\n");
            }
            type = ' ';
            printf("Enter Instruction : ");
            fgets(instr_str, 64 * sizeof(char), stdin);
            instr_str[strcspn(instr_str, "\n")] = '\0';
        }
        free(instr_str);
    }
    else
        printf("MIPS Error 401: Memory not allocated\n");

    return 0;
}


int file_mode(FILE* file, FILE* outputFile, bool pas, stack* mem) {
    char* instr_str = calloc(64, sizeof(char));
    uint32_t compInstr = 0;
    char type;
    int wait = 0;
    int count = 0;

    if (file == NULL)
        return 1;

    if (instr_str)
    {
        while (fgets(instr_str, 64 * sizeof(char), file) != NULL) {
            if (pas)
                scanf("Waiting for user... %d\n", &wait);
            if (!strncmp(instr_str, "#", 1) == 0) {
                printf("MIPS Line %d is %s", count, instr_str);
                
                compInstr = strToInt32(instr_str, &type);   // Parse the MIPS instruction from the string
                //if (compInstr != -1)
                    //execute(compInstr, type, mem);
                fprintf(outputFile, "%08x\n", compInstr);
                //fwrite(&compInstr, sizeof(compInstr), 1, outputFile);
                //fwrite("\n", sizeof(char), 1, outputFile);
                type = ' ';
                count++;
            }
        }
        free(instr_str);
        printf("MIPS %d instructions completed\n", count);
    }
    else
        printf("MIPS Error 401: Memory not allocated\n");

    return 0;
}


int main(int argc, char* argv[]) {
    bool pas = false;
    char* fileName = NULL;
    char* fileNameDone = NULL;
    char* fileNameOut = NULL;
    
    
    for (int i = 0; i < 32; i++)        //test registre
        R[i] = 0;
   

    stack* mem = createMem();
    if (!mem)               //print error deja dans le fonction createMem
        return 1;

    if(argc > 3 && !strcmp(argv[2], "-pas"))
        pas = true;
    
    if (argc == 3 && pas)
        fileName = argv[1];
    else if (argc == 4)
    {
        fileName = argv[1];
        fileNameDone = argv[2];
        fileNameOut = argv[3];
    }
    else if (argc != 1 || (argc == 3 && pas))
    {
        printf("MIPS Error 100: Entry argument not valid : %d && %s\n", argc, argv[2]);
        exit(2);
    }


    FILE* file = NULL;
    FILE* fileDone = NULL;
    FILE* fileOut = NULL;

    if (fileName != NULL)
        file = fopen(fileName, "r");

    if (fileNameDone)
        fileDone = fopen(fileNameDone, "w");

    if (fileNameOut)
        fileOut = fopen(fileNameOut, "w");



    if (file != NULL && fileNameDone != NULL && fileOut != NULL)
    {
        file_mode(file, fileDone, 1, mem);
        for (int i = 0; i < 32; i++)                //argc == 3
            fprintf(fileOut, "$02%d: %d\n", i, R[i]); //write registers after execution to output file
    }
    else if (fileName != NULL)
    {
        file_mode(file, fileDone, 0, mem); //argc == 3
    }
    else
    {
        interactive_mode(mem);     //argc == 1
    }


    if (file != NULL)
        fclose(file);

    if (fileDone)
        fclose(fileDone);

    if (fileOut)
        fclose(fileOut);

    freeMem(mem);

    return 0;
}