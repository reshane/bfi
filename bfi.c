/* bfi.c
 * version 1.0.1
 * shane kenny
 * 7/30/2022
 */

#include "bfi.h"

#define STACK_SIZE 512

#define STACK_PUSH(A) (STACK[SP++] = A)
#define STACK_POP() (STACK[--SP])
#define STACK_EMPTY() (SP == 0)
#define STACK_FULL() (SP == STACK_SIZE)

static unsigned short STACK[STACK_SIZE];
static unsigned int SP = 0;

static struct instruction_t PROGRAM[PROGRAM_SIZE];

int compile(FILE* src)
{
	unsigned short pc = 0, jmp_pc;
	int c;
	while ((c = getc(src)) != EOF && pc < PROGRAM_SIZE)
	{
		switch (c)
		{
			case '>': PROGRAM[pc].operator = OP_INC_DP; break;
			case '<': PROGRAM[pc].operator = OP_DEC_DP; break;
			case '+': PROGRAM[pc].operator = OP_INC_VAL; break;
			case '-': PROGRAM[pc].operator = OP_DEC_VAL; break;
			case '.': PROGRAM[pc].operator = OP_OUT; break;
			case ',': PROGRAM[pc].operator = OP_IN; break;
			case '[':
				PROGRAM[pc].operator = OP_JMP_FWD;// push the current pc onto the stack
				if (STACK_FULL())
					return FAILURE;
				STACK_PUSH(pc);
				break;
			case ']':
				if (STACK_EMPTY())
					return FAILURE;
				jmp_pc = STACK_POP();// the pc at the top of the stack corresponds to this one
				PROGRAM[pc].operator = OP_JMP_BCK;// set operator
				PROGRAM[pc].operand = jmp_pc;// set operand
				PROGRAM[jmp_pc].operand = pc;// set operand of corresponding pc
				break;
			default: pc--; break;
		}
		pc++;// increment to next instruction
	}
	if (!STACK_EMPTY() || pc == PROGRAM_SIZE) { return FAILURE; } // if there are unresolved jmps or we didn't parse to the end of the file
	PROGRAM[pc].operator = OP_END;// make sure our compiled program stops
	return SUCCESS;
}

int execute() {
    unsigned short data[DATA_SIZE], pc = 0;
    unsigned int ptr = DATA_SIZE;
    while (--ptr) { data[ptr] = 0; }
    while (PROGRAM[pc].operator != OP_END && ptr < DATA_SIZE) {
        switch (PROGRAM[pc].operator) {
            case OP_INC_DP: ptr++; break;
            case OP_DEC_DP: ptr--; break;
            case OP_INC_VAL: data[ptr]++; break;
            case OP_DEC_VAL: data[ptr]--; break;
            case OP_OUT: putchar(data[ptr]); break;
            case OP_IN: data[ptr] = (unsigned int)getchar(); break;
            case OP_JMP_FWD: if(!data[ptr]) { pc = PROGRAM[pc].operand; } break;
            case OP_JMP_BCK: if(data[ptr]) { pc = PROGRAM[pc].operand; } break;
            default: return FAILURE;
        }
        pc++;
    }
    return ptr != DATA_SIZE ? SUCCESS : FAILURE;
}

int main(int argc, char *argv[])
{
	int status;
	FILE* src;
	if (argc < 2 || (src = fopen(argv[1], "r")) == NULL)// if issue with file, print error & exit
	{
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return FAILURE;
	}
	status = compile(src);// compile the source code
	fclose(src);
	if (status == SUCCESS) { status = execute(); }
	if (status == FAILURE) { fprintf(stderr, "Error!\n"); }
	return status;
}