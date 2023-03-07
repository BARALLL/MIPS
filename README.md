# MIPS Emulator

A MIPS assembly emulator that executes code provided via command line or file input, featuring a memory simulation of the stack.

It supports three modes based on the number of arguments passed to the program:

Interactive mode is launched when no arguments are provided.
Automatic step-by-step mode is launched when two arguments are provided: the name of the assembly file, and the second is the '-pas'.
Automatic mode is launched when three arguments are provided: the name of the assembly file, the name of the file to save the assembled code, and the name of the file to save the final state of the program after execution.
When in automatic mode, the program reads the assembler program from the file specified in the first argument and produces the two output files. 
The provided Makefile includes a "test-cli" command to verify that automatic mode works correctly and generates the output files.

Mnemonics can be written in uppercase and lowercase, and operands only need one space but can include more between each.
The file can include line breaks and comments using the "#" symbol. 
Register names can be define with or without "$" but immediates should be written in decimal.
