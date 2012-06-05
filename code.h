#ifndef GUARD_CODE
#define GUARD_CODE

typedef struct Operand_ Operand;
typedef struct InterCode_ InterCode;
typedef struct InterCodes_ InterCodes;

struct Operand_{
	enum{
		VARIABLE, CONSTANT, LABEL, FUNC}kind;
	union{
		char* var_name;
		int value;
		int label;
		char* func_name;
	}u;
};

struct InterCode_{
	enum{
		LABEL_DEF, ASSIGN, ADD, SUB, MUL, _DIV, RM, MR, UNCON_JUMP, CON_JUMP, _RETURN, MEM_DEC, ARG, CALL, PARAM, READ, WRITE, FUNCTION
	}kind;
	struct{
		Operand *first, *second, *third;
		char* relop;
	}u;
};

struct InterCodes_{
	InterCode* code;
	InterCodes *prev, *next;
};

InterCodes* code_table;

extern InterCodes* newcode(char* kind, Operand* first, Operand* second, Operand* third, char* relop);

extern Operand* newop(char* kind, char* u);
extern Operand* newop_const(int value);

extern char* newvar();

extern InterCodes* link(InterCodes* first, InterCodes* second);//Link the second codes to the tail of the first one

extern void print_code(char* filename);


#endif
