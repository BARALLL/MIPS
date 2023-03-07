#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#pragma warning(disable : 4996)


int32_t R[32];
uint32_t PC = 0;
uint32_t PS = 0;
uint32_t SP = 0;
uint32_t HI, LO;


void execute(uint32_t instruction, char type, stack* mem, bool* delay) {
    uint32_t opcode, immediate, shamt, special, target;
    int32_t rs, rt, rd;
    int32_t tmp;

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
                printf("MIPS Error 201: Integer Overflow\n");
            else
                R[rd] = tmp;
        }
        else if (special == 0x22) {         //SUB
            tmp = R[rs] - R[rt];
            if (((tmp >> 30) & 1) != ((tmp >> 31) & 1))
                printf("MIPS Error 201: Integer Overflow\n");
            else
                R[rd] = tmp;
        }
        else if (special == 0x24) R[rd] = R[rs] & R[rt];     // AND
        else if (special == 0x24) R[rd] = R[rs] | R[rt];     // OR
        else if (special == 0x1A) {                         // DIV
            if (R[rt] == 0)
                printf("MIPS Error 202: division by 0\n");
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
        //printf("Resultat : %d\n", R[rd]);
    }

    else if (type == 'J')
    {
        opcode = (instruction & 0xFC000000) >> 26;
        target = instruction & 0x3FFFFFF;
        
        //if (opcode == 0x02) PC = (PC & 0xF0000000) | (target * 4);  si valeur relative : J 4 jump a 4 plus loin qu'actuel
        if (opcode == 0x02)                                     //valeur absolue : J 4 jump a la 4eme instruction du prog
        {                                                       //tres simple si mem en tab, plus penible en liste chainee
            if (*delay)
            {
                PC = PS + (target * 4);         // J
                *delay = false;
                PC -= 4;
            }
            else
                *delay = true;
        }                                                                   
        else if (opcode == 0x03) {              // JAL
            if (*delay)
            {
                R[31] = PC + 8;
                PC = PS + (target * 4);
                *delay = false;
                PC -= 4;
            }
            else
                *delay = true;

        }
    }

    else if (type == 'I')
    {
        opcode = (instruction & 0xFC000000) >> 26;
        rd = (instruction & 0x0000F800) >> 11;
        rs = (instruction & 0x03E00000) >> 21;
        rt = (instruction & 0x001F0000) >> 16;
        immediate = instruction & 0x0000FFFF;


        
        if (opcode != 0x23 && opcode != 0x2B && opcode != 0x0F)
        {
            if ((immediate >> 15) & 1) // ie is signed
                immediate = immediate | 0xFFFF0000;
            else
                immediate = immediate;
        }
        

        if (opcode == 0x08 || opcode == 0x09) { // ADDI/ADDIU
            tmp = R[rs] + immediate;
            if (((tmp >> 30) & 1) != ((tmp >> 31) & 1))
                printf("MIPS Error 201: Integer Overflow\n");
            else
                R[rt] = tmp;
        }

        else if (opcode == 0x04) { // BEQ       //conditional jump (branch)
            if (R[rs] == R[rt]) 
            {
                if (*delay)
                {
                    PC += immediate * 4;    //mult par 4 au lieu de <<2 pour préserver le signe
                    *delay = false;
                }
                else
                    *delay = true;
            }
          }
        
        else if (opcode == 0x05) { // BNE
            if (R[rs] != R[rt])
            {
                if (*delay)
                {
                    PC += immediate * 4;
                    *delay = false;
                }
                else
                    *delay = true;
            }
        }
        
        else if (opcode == 0x06) { // BLEZ
            if (R[rs] <= 0)
            {
                if (*delay)
                {
                    PC += immediate * 4;
                    *delay = false;
                }
                else
                    *delay = true;
            }
        }

        else if (opcode == 0x07) { // BGTZ
            if (R[rs] > 0)
            {
                if (*delay)
                {
                    PC += immediate * 4;
                    *delay = false;
                }
                else
                    *delay = true;
            }
        }

        else if (opcode == 0x23) { // LW
            R[rt] = (getElem(mem, (uint32_t)(R[rs] + immediate), &type) << 24) | ((getElem(mem, (uint32_t)(R[rs] + immediate + 4), &type)) << 16)
                | (getElem(mem, (uint32_t)(R[rs] + immediate + 8), &type) << 8) | getElem(mem, (uint32_t)(R[rs] + immediate + 12), &type);
        }

        else if (opcode == 0x2B) { // SW
            insertElem(mem, (uint32_t)(R[rs] + immediate), R[rt] >> 24, 'D');           //type is data
            insertElem(mem, (uint32_t)(R[rs] + immediate + 4), (R[rt] >> 16) & 0xFF, 'D');
            insertElem(mem, (uint32_t)(R[rs] + immediate + 8), (R[rt] >> 8) & 0xFF, 'D');
            insertElem(mem, (uint32_t)(R[rs] + immediate + 12), (R[rt]) & 0xFF, 'D');
        }

        else if (opcode == 0x2A) { // SLT
            R[rd] = (R[rs] < R[rt]) ? 1 : 0;
        }

        else if (opcode == 0x0F) { // LUI
            R[rt] = immediate << 16;
        }
    }
}


bool arg(char* instruction, char* tmp, int* index)
{
    int i = *index;
    bool isarg = true;
    int immneg = 0;
    int tmpsize = 0;
    while (instruction[i] != '\0' && instruction[i] != '\n' && instruction[i] != '$' && instruction[i] != '-'
        && !(instruction[i] >= '0' && instruction[i] <= '9'))
        i++;

    if (instruction[i] == '-') 
    {
        immneg = 1;
        i++;
    }
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
        strncpy(tmp, &instruction[i - tmpsize - immneg], tmpsize + immneg);
        tmp[tmpsize+immneg] = '\0';
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
    {
        if (instruction[i] >= 'a' && instruction[i] <= 'z')
            name[i] = instruction[i] - 'a' + 'A';
        else
            name[i] = instruction[i];
    }

    name[i] = '\0';

    printf("%s\n", name);

    if (strcmp(name, "NOP") == 0) return 0;

    uint16_t arg1 = 0;
    uint16_t arg2 = 0;
    uint16_t arg3 = 0;

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

        if ((strcmp(name, "SYSCALL") == 0))
            compInstr = (0x00 << 26) | (arg1 << 6) | opcode;
        else if ((strcmp(name, "SLL") == 0))
            compInstr = (0x00 << 26) | (arg2 << 16) | (arg1 << 11) | (arg3 << 6) | opcode;
        else if ((!isarg3 && isarg2) || strcmp(name, "JR") == 0)
            compInstr = (0x00 << 26) | (arg1 << 21) | (arg2 << 16) | (arg3 << 11) | opcode;
        else
            compInstr = (0x00 << 26) | (arg2 << 21) | (arg3 << 16) | (arg1 << 11) | opcode;
    }
    
    if (strcmp(name, "NOP") == 0) { compInstr = 0; }
    if (strcmp(name, "ROTR") == 0 || strcmp(name, "SRL") == 0)
    {
        compInstr = (0x00 << 26) | (arg2 << 16) | (arg1 << 11) | (arg3 << 6) | opcode;      //special cases
        if (strcmp(name, "ROTR") == 0)
            compInstr = compInstr | (1 << 21);
    }

    if (*type != 'J' && *type != 'R')
    {     
        if (strcmp(name, "LUI") == 0)
            compInstr = (opcode << 26) | (arg1 << 16) | arg2;
        if (strcmp(name, "SW") == 0 || strcmp(name, "LW") == 0)
            compInstr = (opcode << 26) | (arg3 << 21) | (arg1 << 16) | arg2;
        else if (strcmp(name, "BEQ") == 0 || strcmp(name, "BGTZ") == 0 || strcmp(name, "BLEZ") == 0 || strcmp(name, "BNE") == 0)
        {
            if (strcmp(name, "BGTZ") == 0 || strcmp(name, "BLEZ") == 0)
            {
                arg3 = arg2;
                arg2 = 0;
            }
            compInstr = (opcode << 26) | (arg1 << 21) | (arg2 << 16) | arg3;
        }
        else
            compInstr = (opcode << 26) | (arg2 << 21) | (arg1 << 16) | arg3;
        *type = 'I';
    }

    printf("\nMIPS Hexadecimal instruction is %08x\n\n", compInstr);
    return compInstr;
}

void strRm(char* str)
{
    int i = 0, k = 0;
    while ((!(str[i] >= 'a' && str[i] <= 'z')) &&        //while it's not between a and z or A and Z and isn't a #
        (!(str[i] >= 'A' && str[i] <= 'Z')) && str[i] != '#' && str[i] != '\0')

        i++;

    if (i > 0)
    {
        for (int j = i; str[j] != '\0'; j++)
        {
            str[k] = str[j];
            k++;
        }
        if (str[0] != '\n')
            str[k] = '\0';
    }
    else
    {
        i = 0;
        while (str[i] != '#' && str[i] != '\0')         //on coupe au min entre \0 et #
            i++;
        if (str[0] != '\n' && str[0] != '#')
        {
            if (str[i] == '#')
                str[strcspn(str, "#")] = '\0';
            else
                str[strcspn(str, "\n")] = '\0';
        }
    }
}


int interactive_mode(stack* mem) {
    char* instr_str = calloc(255, sizeof(char));
    uint32_t compInstr = 0;
    char type;
    bool delay = false;

    if (instr_str)
    {
        instr_str[0] = '#';
        
        while (strcmp(instr_str, "EXIT")) { //stop if EXIT
            if (instr_str[0] != '#' && instr_str[0] != '\n') { //skip line if commentary or empty after removing leading waste
                compInstr = strToInt32(instr_str, &type);
                if (compInstr != (uint32_t)-1)
                    execute(compInstr, type, mem, &delay);
                else
                    printf("MIPS Error 500: Parsing Error\n");
            }
            type = ' ';
            printf("Enter Instruction : ");
            fgets(instr_str, 255 * sizeof(char), stdin);
            fflush(stdout);

            strRm(instr_str);             // remove potential leading waste
        }
        free(instr_str);
    }
    else
        printf("MIPS Error 401: Memory not allocated\n");

    return 0;
}


int file_mode(FILE* file, FILE* outputFile, bool pas, stack* mem) {
    char* instr_str = calloc(255, sizeof(char));
    uint32_t compInstr = 0;
    char type;
    int wait = 0;
    int countHex = 0;
    int countExec = 0;
    bool delay = false;


    if (instr_str)
    {
        PC = SP;
        PS = SP;
        while (fgets(instr_str, 255 * sizeof(char), file) != NULL) { //fgets(instr_str, 255 * sizeof(char), stdin)){//
            strRm(instr_str);
            //if (strcmp(instr_str, "EXIT") == 0) break;
            if (pas)
            {
                scanf("Waiting for user... %d\n", &wait);
            }
            if (instr_str[0] != '#' && instr_str[0] != '\n') 
            {
                type = ' ';
                compInstr = strToInt32(instr_str, &type);   // Parse the MIPS instruction from the string
                printf("MIPS Line %d is '%s' parsed in %x\n", countHex, instr_str, compInstr);
                fflush(stdout);
                
                insertElem(mem, SP, compInstr, type);
                if(!pas)
                    fprintf(outputFile, "%08x\n", compInstr);
                SP += 4;

                countHex++;
            }
        }

        while (PC <= SP - 4)
        {
            compInstr = getElem(mem, PC, &type);
            if (type == 'R' || type == 'I' || type == 'J') {
                execute(compInstr, type, mem, &delay);
                countExec++;
            }
            else
                printf("MIPS Error 403: Data at adress is not instruction\n");

            fflush(stdout);
            printf("\nPC: %d    Instruction: %08x   type %c\n", PC, compInstr, type);
            fflush(stdout);

            if (delay)
            {
                PC += 4;
                compInstr = getElem(mem, PC, &type);                    //execute next instruction before going back
                if (type == 'R' || type == 'I' || type == 'J')          //to the previous one (conditional branch)
                    execute(compInstr, type, mem, &delay);
                else
                    printf("MIPS Error 403: Data at adress is not instruction\n");
                printf("\nPC: %d    Instruction: %08x     delay %d\n", PC, compInstr, delay);
                fflush(stdout);
                PC -= 4;
            }
            else
                PC += 4;
        }
        free(instr_str);
        printf("MIPS : %d instructions parsed and %d executed\n", countHex, countExec);
    }
    else
        printf("MIPS Error 401: Memory not allocated\n");

    return 0;
}



int file_mode_nofile(FILE* outputFile, bool pas, stack* mem) {
    char* instr_str = calloc(255, sizeof(char));
    uint32_t compInstr = 0;
    char type;
    int wait = 0;
    int countHex = 0;
    int countExec = 0;
    bool delay = false;


    if (instr_str)
    {
        instr_str[0] = '#';
        PC = SP;
        PS = SP;

        while (strcmp(instr_str, "EXIT")) { //EXIT...
            if (instr_str[0] != '#' && instr_str[0] != '\n') {
                compInstr = strToInt32(instr_str, &type);
                
                insertElem(mem, SP, compInstr, type);
                SP += 4;

                countHex++;
            }
            type = ' ';
            printf("Enter Instruction : ");
            fgets(instr_str, 255 * sizeof(char), stdin);
            fflush(stdout);

            strRm(instr_str);             // remove potential leading waste
        }

        while (PC <= SP - 4)
        {
            compInstr = getElem(mem, PC, &type);
            if (type == 'R' || type == 'I' || type == 'J') {
                execute(compInstr, type, mem, &delay);
                countExec++;
            }
            else
                printf("MIPS Error 403: Data at adress is not instruction\n");

            //printf("\nPC: %d    Instruction: %08x     delay %d\n", PC, compInstr, delay);
            //fflush(stdout);

            if (delay)
            {
                PC += 4;
                compInstr = getElem(mem, PC, &type);                    //execute next instruction before going back
                //fprintf(outputFile, "\n\nPC: %d    Instruction: %08x\n\n", PC, compInstr);
                //printf("\nPC: %d    Instruction: %08x     delay %d\n", PC, compInstr, delay);
                //fflush(stdout);
                if (type == 'R' || type == 'I' || type == 'J')          //to the previous one (conditional branch)
                    execute(compInstr, type, mem, &delay);
                else
                    printf("MIPS Error 403: Data at adress is not instruction\n");
                PC -= 4;
            }
            else
                PC += 4;
        }
        free(instr_str);
        printf("MIPS : %d instructions parsed and %d executed\n", countHex, countExec);
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
    

    stack* mem = createMem();
    if (!mem)               //print error deja dans le fonction createMem
        return 1;

    if(argc >= 3 && !strcmp(argv[2], "-pas"))
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
        file_mode(file, fileDone, false, mem);
        fflush(stdout);
        //fprintf(fileOut, "\n\nHI: %d\n", HI);
        //fprintf(fileOut, "LO: %d\n", LO);
        for (int i = 0; i < 32; i++)                //argc == 4
        {
            if (R[i] != 0)
                fprintf(fileOut, "$%02d: %d\n", i, R[i]); //write registers after execution to output file
                //printf("$%02d: %d\n", i, R[i]);
        }

    }
    else if (fileName != NULL && pas)
    {
        file_mode(file, fileDone, pas, mem); //argc == 3
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