#include "code.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int LABEL_NUM = 1;
int TEMP_VAR_NUM = 1;


InterCodes* newcode(char* kind, Operand* first, Operand* second, Operand* third, char* relop){
	InterCodes* code_node = (InterCodes*)malloc(sizeof(InterCodes));
	code_node->code = (InterCode*)malloc(sizeof(InterCode));
	if(strcmp(kind, "LABEL_DEF") == 0){
		code_node->code->kind == LABEL_DEF;
	}
	else if(strcmp(kind, "ASSIGN") == 0){
		code_node->code->kind = ASSIGN;
	}
	else if(strcmp(kind, "ADD") == 0){
		code_node->code->kind = ADD;
	}
	else if(strcmp(kind, "SUB") == 0){
		code_node->code->kind = SUB;
	}
	else if(strcmp(kind, "MUL") == 0){
		code_node->code->kind = MUL;
	}
	else if(strcmp(kind, "DIV") == 0){
		code_node->code->kind = _DIV;
	}
	else if(strcmp(kind, "RM") == 0){
		code_node->code->kind = RM;
	}
	else if(strcmp(kind, "MR") == 0){
		code_node->code->kind = MR;
	}
	else if(strcmp(kind, "UNCON_JUMP") == 0){
		code_node->code->kind = UNCON_JUMP;
	}
	else if(strcmp(kind, "CON_JUMP") == 0){
		code_node->code->kind = CON_JUMP;
		code_node->code->u.relop = (char*)malloc(strlen(relop) + 1);
		strcpy(code_node->code->u.relop, relop);
	}
	else if(strcmp(kind, "RETURN") == 0){
		code_node->code->kind = _RETURN;
	}
	else if(strcmp(kind, "MEM_DEC") == 0){
		code_node->code->kind = MEM_DEC;
	}
	else if(strcmp(kind, "ARG") == 0){
		code_node->code->kind = ARG;
	}
	else if(strcmp(kind, "CALL") == 0){
		code_node->code->kind = CALL;
	}
	else if(strcmp(kind, "PARAM") == 0){
		code_node->code->kind = PARAM;
	}
	else if(strcmp(kind, "READ") == 0){
		code_node->code->kind = READ;
	}
	else if(strcmp(kind, "WRITE") == 0){
		code_node->code->kind = WRITE;
	}
	else if(strcmp(kind, "FUNCTION") == 0){
		code_node->code->kind = FUNCTION;
	}
	else{
		return 0;
	}
	code_node->code->u.first = first;
	code_node->code->u.second = second;
	code_node->code->u.third = third;

	code_node->prev = code_node;
	code_node->next = code_node;

	return code_node;
}

Operand* newop(char* kind, char* u){
	Operand* op = (Operand*)malloc(sizeof(Operand));
	if(strcmp(kind, "VARIABLE") == 0){
		op->kind = VARIABLE;
		op->u.var_name = (char*)malloc(strlen(u) + 1);
		strcpy(op->u.var_name, u);
	}
	else if(strcmp(kind, "FUNC") == 0){
		op->kind = FUNC;
		op->u.func_name = (char*)malloc(strlen(u) + 1);
		strcpy(op->u.func_name, u);
	}
	else if(strcmp(kind, "LABEL") == 0){
		op->kind = LABEL;
		op->u.label = LABEL_NUM++;
	}
	else{
		return 0;
	}
	return op;
}

Operand* newop_const(int value){
	Operand* op = (Operand*)malloc(sizeof(Operand));
	op->kind = CONSTANT;
	op->u.value = value;
	return op;
}

char* newvar(){
	char* t = (char*)malloc(10);
	int num = TEMP_VAR_NUM++;
	int i = 1, j = 0;
	char c[10];

	t[0] = 't';
	while(num != 0){
		int a = num % 10;
		num = num / 10;
		c[j++] = '0' + a;
	}
	while(j > 0){
		t[i++] = c[--j];
	}
	t[i] = 0;
	return t;

}

InterCodes* link(InterCodes* first, InterCodes* second){
	if(second == 0)
		return first;
	if(first == 0)
		return second;
	InterCodes* tail1 = first->prev, *tail2 = second->prev;
	tail1->next = second;
	tail2->next = first;
	second->prev = tail1;
	first->prev = tail2;
	return first;
}

void print_op(FILE* file, Operand* op){
	if(op == 0)
		return;
	if(op->kind == VARIABLE){
		fprintf(file, "%s", op->u.var_name);
		return;
	}
	if(op->kind == CONSTANT){
		fprintf(file, "#%d", op->u.value);
		return;
	}
	if(op->kind == LABEL){
		fprintf(file, "label%d", op->u.label);
		return;
	}
	if(op->kind == FUNC){
		fprintf(file, "%s", op->u.func_name);
	}
}

void print_code(char* filename){
	FILE* out = fopen(filename, "w");
	InterCodes* pCode = code_table;
	if(out == 0){
		printf("Fail to open file: %s\n", filename);
		return;
	}
	if(code_table == 0)
		return;
	do{
		if(pCode->code->kind == LABEL_DEF){
			fprintf(out, "LABEL ");
			print_op(out, pCode->code->u.first);
			fprintf(out, " :");
		}
		else if(pCode->code->kind == ASSIGN){
			print_op(out, pCode->code->u.first);
			fprintf(out, " := ");
			print_op(out, pCode->code->u.second);
		}
		else if(pCode->code->kind == ADD){
			print_op(out, pCode->code->u.first);
			fprintf(out, " := ");
			print_op(out, pCode->code->u.second);
			fprintf(out, " + ");
			print_op(out, pCode->code->u.third);
		}
		else if(pCode->code->kind == SUB){
			print_op(out, pCode->code->u.first);
			fprintf(out, " := ");
			print_op(out, pCode->code->u.second);
			fprintf(out, " - ");
			print_op(out, pCode->code->u.third);
		}
		else if(pCode->code->kind == MUL){
			print_op(out, pCode->code->u.first);
			fprintf(out, " := ");
			print_op(out, pCode->code->u.second);
			fprintf(out, " * ");
			print_op(out, pCode->code->u.third);
		}
		else if(pCode->code->kind == _DIV){
			print_op(out, pCode->code->u.first);
			fprintf(out, " := ");
			print_op(out, pCode->code->u.second);
			fprintf(out, " / ");
			print_op(out, pCode->code->u.third);
		}
		else if(pCode->code->kind == RM){
			print_op(out, pCode->code->u.first);
			fprintf(out, " := *");
			print_op(out, pCode->code->u.second);
		}
		else if(pCode->code->kind == MR){
			fprintf(out, "*");
			print_op(out, pCode->code->u.first);
			fprintf(out, " := ");
			print_op(out, pCode->code->u.second);
		}
		else if(pCode->code->kind == UNCON_JUMP){
			fprintf(out, "GOTO ");
			print_op(out, pCode->code->u.first);
		}
		else if(pCode->code->kind == CON_JUMP){
			fprintf(out, "IF ");
			print_op(out, pCode->code->u.first);
			fprintf(out, " %s ", pCode->code->u.relop);
			print_op(out, pCode->code->u.second);
			fprintf(out, " GOTO ");
			print_op(out, pCode->code->u.third);
		}
		else if(pCode->code->kind == _RETURN){
			fprintf(out, "RETURN ");
			print_op(out, pCode->code->u.first);
		}
		else if(pCode->code->kind == MEM_DEC){
			fprintf(out, "DEC	");
			print_op(out, pCode->code->u.first);
			fprintf(out, " %d", pCode->code->u.second->u.value);
			//print_op(out, pCode->code->u.second);
		}
		else if(pCode->code->kind == ARG){
			fprintf(out, "ARG ");
			print_op(out, pCode->code->u.first);
		}
		else if(pCode->code->kind == CALL){
			print_op(out, pCode->code->u.first);
			fprintf(out, " := CALL ");
			print_op(out, pCode->code->u.second);
		}
		else if(pCode->code->kind == PARAM){
			fprintf(out, "PARAM ");
			print_op(out, pCode->code->u.first);
		}
		else if(pCode->code->kind == READ){
			fprintf(out, "READ ");
			print_op(out, pCode->code->u.first);
		}
		else if(pCode->code->kind == WRITE){
			fprintf(out, "WRITE ");
			print_op(out, pCode->code->u.first);
		}
		else if(pCode->code->kind == FUNCTION){
			fprintf(out, "FUNCTION ");
			print_op(out, pCode->code->u.first);
			fprintf(out, " :");
		}
		fprintf(out, "\n");
		pCode = pCode->next;
	}while(pCode != code_table);
	fclose(out);
}




