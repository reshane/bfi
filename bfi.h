/* bfi.h
*/

#include <stdio.h>

#define OP_END 0
#define OP_INC_DP 1
#define OP_DEC_DP 2
#define OP_INC_VAL 3
#define OP_DEC_VAL 4
#define OP_OUT 5
#define OP_IN 6
#define OP_JMP_FWD 7
#define OP_JMP_BCK 8

#define SUCCESS	0
#define FAILURE	1

#define PROGRAM_SIZE 4096
#define DATA_SIZE 65535

struct instruction_t {
	unsigned short operator;
	unsigned short operand;
};

int compile(FILE*);
int execute();