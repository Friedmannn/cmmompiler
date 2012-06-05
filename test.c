#include "code.h"

void test1(){
	Operand *op1, *op2, *op3;
	Operand *label1, *label2, *label3;
	InterCodes* code, *code2;
	
	op1 = newop("FUNC", "main");
	code = newcode("FUNCTION", op1, 0, 0, 0);
	code_table = link(code_table, code);

	op1 = newop("VARIABLE", "t1");
	code = newcode("READ", op1, 0, 0, 0);
	code_table = link(code_table, code);

	op1 = newop("VARIABLE", "v1");
	op2 = newop("VARIABLE", "t1");
	code = newcode("ASSIGN", op1, op2, 0, 0);
	code_table = link(code_table, code);

	op1 = newop("VARIABLE", "t2");
	op2 = newop_const(0);
	code = newcode("ASSIGN", op1, op2, 0, 0);

	op1 = newop("VARIABLE", "v1");
	op2 = newop("VARIABLE", "t2");
	label1 = newop("LABEL", 0);
	code2 = newcode("CON_JUMP", op1, op2, label1, ">");

	code = link(code, code2);
	code_table = link(code_table, code);

	label2 = newop("LABEL", 0);
	code = newcode("CON_JUMP", op1, op2, label2, "<");
	code_table = link(code_table, code);

	code = newcode("WRITE", op2, 0, 0, 0);
	code_table = link(code_table, code);

	label3 = newop("LABEL", 0);
	code = newcode("UNCON_JUMP", label3, 0, 0, 0);
	code_table = link(code_table, code);

	code = newcode("LABEL_DEF", label1, 0, 0, 0);
	code_table = link(code_table, code);

	op1 = newop("VARIABLE", "t3");
	op2 = newop_const(1);
	code = newcode("ASSIGN", op1, op2, 0, 0);
	code_table = link(code_table, code);

	code = newcode("WRITE", op1, 0, 0, 0);
	code_table = link(code_table, code);

	code = newcode("UNCON_JUMP", label3, 0, 0, 0);
	code_table = link(code_table, code);

	code = newcode("LABEL_DEF", label2, 0, 0, 0);
	code_table = link(code_table, code);

	op1 = newop("VARIABLE", "t6");
	op2 = newop_const(-1);
	code = newcode("ASSIGN", op1, op2, 0, 0);
	code_table = link(code_table, code);

	code = newcode("WRITE", op1, 0, 0, 0);
	code_table = link(code_table, code);

	code = newcode("LABEL_DEF", label3, 0, 0, 0);
	code_table = link(code_table, code);

	op1 = newop("VARIABLE", "t2");
	code = newcode("RETURN", op1, 0, 0, 0);
	code_table = link(code_table, code);

	print_code("test.ir");
}

int main(){
	test1();
}
