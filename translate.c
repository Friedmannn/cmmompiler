#include "translate.h"
#include "happy_tree_friends.h"

extern void trans(struct syntax_tree* t);
extern InterCodes* translate_Cond(struct syntax_tree* exp, Operand* label_true, Operand* label_false);
extern InterCodes* getAddr(struct syntax_tree* exp, Type type, char* place);

InterCodes* translate_Args(struct syntax_tree* Args);

InterCodes* translate_exp(struct syntax_tree* exp, char* place){
	if(strcmp(exp->leftson->name, "INT") == 0){	//Exp=>INT
		if(place == 0)
			return 0;
		int value = exp->leftson->value.intval;
		Operand* op1 = newop("VARIABLE", place);
		Operand* op2 = newop_const(value);
		InterCodes* code = newcode("ASSIGN", op1, op2, 0, 0);
		return code;
	}

	if(strcmp(exp->leftson->name, "ID") == 0 && exp->leftson->rightbrother == 0){	//Exp => ID
		if(place == 0)
			return 0;
		Operand* op1 = newop("VARIABLE", place);
		Operand* op2 = newop("VARIABLE", exp->leftson->value.idval);
		InterCodes* code = newcode("ASSIGN", op1, op2, 0, 0);
		return code;
	}

	if(exp->leftson->rightbrother != 0 && strcmp(exp->leftson->rightbrother->name, "ASSIGNOP") == 0){	//Exp => Exp ASSIGNOP Exp
		if(strcmp(exp->leftson->leftson->name, "ID") == 0){	//Exp1 => ID
			char* t1 = newvar();
			struct syntax_tree* exp1 = exp->leftson;
			struct syntax_tree* exp2 = exp1->rightbrother->rightbrother;
			InterCodes* code1 = translate_exp(exp2, t1);
			Operand* op1 = newop("VARIABLE", exp1->leftson->value.idval);
			Operand* op2 = newop("VARIABLE", t1);
			code1 = link(code1, newcode("ASSIGN", op1, op2, 0, 0));
			if(place != 0){
				Operand* op3 = newop("VARIABLE", place);
				Operand* op4 = newop("VARIABLE", exp1->leftson->value.idval);
				code1 = link(code1, newcode("ASSIGN", op3, op4, 0, 0));
			}
			return code1;
		}
		if(strcmp(exp->leftson->leftson->name, "Exp") == 0 && strcmp(exp->leftson->leftson->rightbrother->name, "LB") == 0){
			char* addr = newvar();
			struct syntax_tree* exp1 = exp->leftson;
			struct syntax_tree* id = exp1->leftson;
			while(strcmp(id->name, "ID") != 0){
				id = id->leftson;
			}
			VariableNode* var = findVarTab(id->value.idval);
			Type type = var->varType;
			InterCodes* code1 = getAddr(exp1, type, addr);
			char* t1 = newvar();
			struct syntax_tree* exp2 = exp1->rightbrother->rightbrother;
			InterCodes* code2 = translate_exp(exp2, t1);
			Operand* op1 = newop("VARIABLE", addr);
			Operand* op2 = newop("VARIABLE", t1);
			InterCodes* code3 = newcode("MR", op1, op2, 0, 0);
			code1 = link(code1, code2);
			code1 = link(code1, code3);
			if(place != 0){
				Operand* op3 = newop("VARIABLE", place);
				code1 = link(code1, newcode("RM", op3, op1, 0, 0));
			}
			return code1;
		}
	}

	if(exp->leftson->rightbrother != 0 && (strcmp(exp->leftson->rightbrother->name, "PLUS") == 0 || strcmp(exp->leftson->rightbrother->name, "MINUS") == 0 || strcmp(exp->leftson->rightbrother->name, "STAR") == 0 || strcmp(exp->leftson->rightbrother->name, "DIV") == 0)){	//Exp => Exp PLUS|MINUS|STAR|DIV Exp
		if(place == 0){
			return 0;
		}
		char* t1 = newvar();
		char* t2 = newvar();
		struct syntax_tree* exp1 = exp->leftson;
		struct syntax_tree* exp2 = exp1->rightbrother->rightbrother;
		InterCodes* code1 = translate_exp(exp1, t1);
		InterCodes* code2 = translate_exp(exp2, t2);
		Operand* op1 = newop("VARIABLE", place);
		Operand* op2 = newop("VARIABLE", t1);
		Operand* op3 = newop("VARIABLE", t2);
		InterCodes* code3;
		if(strcmp(exp1->rightbrother->name, "PLUS") == 0)
			code3 = newcode("ADD", op1, op2, op3, 0);
		if(strcmp(exp1->rightbrother->name, "MINUS") == 0)
			code3 = newcode("SUB", op1, op2, op3, 0);
		if(strcmp(exp1->rightbrother->name, "STAR") == 0)
			code3 = newcode("MUL", op1, op2, op3, 0);
		if(strcmp(exp1->rightbrother->name, "DIV") == 0)
			code3 = newcode("_DIV", op1, op2, op3, 0);
		code1 = link(code1, code2);
		code1 = link(code1, code3);
		return code1;
	}

	if(exp->leftson->rightbrother != 0 && strcmp(exp->leftson->rightbrother->name, "LB") == 0){	//Exp => Exp LB Exp RB
		char* addr = newvar();
		struct syntax_tree* id = exp->leftson;
		while(strcmp(id->name, "ID") != 0){
			id = id->leftson;
		}
		VariableNode* var = findVarTab(id->value.idval);
		Type type = var->varType;
		InterCodes* code1 = getAddr(exp, type, addr);
		Operand* op1 = newop("VARIABLE", place);
		Operand* op2 = newop("VARIABLE", addr);
		InterCodes* code2 = newcode("RM", op1, op2, 0, 0);
		return link(code1, code2);
	}

	if(strcmp(exp->leftson->name, "LP") == 0){	//Exp => LP Exp RP
		return translate_exp(exp->leftson->rightbrother, place);
	}

	if(strcmp(exp->leftson->name, "MINUS") == 0){	//Exp => MINUS Exp
		if(place == 0)
			return 0;

		char* t1 = newvar();
		InterCodes* code1 = translate_exp(exp->leftson->rightbrother, t1);
		Operand* op1 = newop("VARIABLE", place);
		Operand* op2 = newop_const(0);
		Operand* op3 = newop("VARIABLE", t1);
		InterCodes* code2 = newcode("SUB", op1, op2, op3, 0);
		code1 = link(code1, code2);
		return code1;
	}

	if(strcmp(exp->leftson->name, "NOT") == 0 || 
			strcmp(exp->leftson->rightbrother->name, "RELOP") == 0 ||
			strcmp(exp->leftson->rightbrother->name, "AND") == 0 ||
			strcmp(exp->leftson->rightbrother->name, "OR") == 0){	//Exp => Exp RELOP|AND|OR Exp || NOT Exp
		Operand* label1 = newop("LABEL", 0);
		Operand* label2 = newop("LABEL", 0);
		Operand* op1 = newop("VARIABLE", place);
		Operand* op2 = newop_const(0);
		Operand* op3 = newop_const(1);
		InterCodes* code0 = newcode("ASSIGN", op1, op2, 0, 0);
		InterCodes* code1 = translate_Cond(exp, label1, label2);
		InterCodes* code2 = newcode("LABEL_DEF", label1, 0, 0, 0);
		code2 = link(code2, newcode("ASSIGN", op1, op3, 0, 0));
		code2 = link(code2, newcode("LABEL_DEF", label2, 0, 0, 0));
		code0 = link(code0, code1);
		code0 = link(code0, code2);
		return code0;
	}

	if(strcmp(exp->leftson->name, "ID") == 0 && exp->leftson->rightbrother != 0){	//CALL FUNCTION
		FuncNode* sym = findFuncTab(exp->leftson->value.idval);
		char* fName = sym->fName;
		struct syntax_tree* args = exp->leftson->rightbrother->rightbrother;
		if(strcmp(args->name, "RP") == 0){	//No Args
			Operand* op1 = newop("VARIABLE", place);
			if(strcmp(fName, "read") == 0){
				InterCodes* code1 = newcode("READ", op1, 0, 0, 0);
				return code1;
			}
			Operand* op2 = newop("FUNC", fName);
			InterCodes* code1 = newcode("CALL", op1, op2, 0, 0);
			return code1;
		}
		else{								//With Args
			if(strcmp(exp->leftson->value.idval, "write") == 0){
				char* t1 = newvar();
				InterCodes* code1 = translate_exp(args->leftson, t1);
				Operand* op1 = newop("VARIABLE", t1);
				InterCodes* code2 = newcode("WRITE", op1, 0, 0, 0);
				return link(code1, code2);
			}
			InterCodes* code1 = translate_Args(args);
			Operand* op1 = newop("VARIABLE", place);
			Operand* op2 = newop("FUNC", fName);
			InterCodes* code2 = newcode("CALL", op1, op2, 0, 0);
			return link(code1, code2);
		}
	}

	return 0;
}

InterCodes* getAddr(struct syntax_tree* exp, Type type, char* place){
		HappyTreeFriends* friends = 0;
		while(type->kind == array){
			HappyTreeFriends* fnd = (HappyTreeFriends*)malloc(sizeof(HappyTreeFriends));
			fnd->happy = type->u.array.size;
			friends = happy_insert(friends, fnd);
			type = type->u.array.elem;
		}
		struct syntax_tree* son = exp->leftson;
		InterCodes* code1 = translate_exp(son->rightbrother->rightbrother, place);
		Operand* op = newop("VARIABLE", place);
		son = son->leftson;
		while(son->rightbrother != 0){
			char* t1 = newvar();
			InterCodes* code2 = translate_exp(son->rightbrother->rightbrother, t1);
			code1 = link(code1, code2);
			Operand* op1 = newop("VARIABLE", t1);
			Operand* op2 = newop_const(friends->happy);
			code2 = newcode("MUL", op1, op1, op2, 0);
			code1 = link(code1, code2);
			code2 = newcode("ADD", op, op, op1, 0);
			code1 = link(code1, code2);
			son = son->leftson;
			friends = friends->next;
		}
		Operand* op1 = newop_const(4);
		InterCodes* code2 = newcode("MUL", op, op, op1, 0);
		return link(code1, code2);
}

InterCodes* translate_stmt(struct syntax_tree* stmt){
	if(strcmp(stmt->leftson->name, "Exp") == 0){	//Stmt => Exp SEMI
		return translate_exp(stmt->leftson, 0);
	}

	if(strcmp(stmt->leftson->name, "CompSt") == 0){	//Stmt => CompSt
		trans(stmt->leftson);
		return 0;
	}

	if(strcmp(stmt->leftson->name, "RETURN") == 0){	//Stmt => RETURN Exp SEMI
		char* t1 = newvar();
		InterCodes* code1 = translate_exp(stmt->leftson->rightbrother, t1);
		Operand* op1 = newop("VARIABLE", t1);
		InterCodes* code2 = newcode("RETURN", op1, 0, 0, 0);
		code1 = link(code1, code2);
		return code1;
	}

	if(strcmp(stmt->leftson->name, "IF") == 0){	//Stmt => IF...
		Operand* label1 = newop("LABEL", 0);
		Operand* label2 = newop("LABEL", 0);
		Operand* label3;
		InterCodes* code1 = translate_Cond(stmt->leftson->rightbrother->rightbrother, label1, label2);
		code_table = link(code_table, code1);
		InterCodes* code3;
		InterCodes* temp = newcode("LABEL_DEF", label1, 0, 0, 0);
		code_table = link(code_table, temp);
		InterCodes* code2 = translate_stmt(stmt->leftson->rightbrother->rightbrother->rightbrother->rightbrother);
		code_table = link(code_table, code2);
		if(stmt->leftson->rightbrother->rightbrother->rightbrother->rightbrother->rightbrother != 0){
			label3 = newop("LABEL", 0);
			temp = newcode("UNCON_JUMP", label3, 0, 0, 0);
			code_table = link(code_table, temp);
		}
		temp = newcode("LABEL_DEF", label2, 0, 0, 0);
		code_table = link(code_table, temp);
		if(stmt->leftson->rightbrother->rightbrother->rightbrother->rightbrother->rightbrother != 0){
			code3 = translate_stmt(stmt->leftson->rightbrother->rightbrother->rightbrother->rightbrother->rightbrother->rightbrother);
			code_table = link(code_table, code3);
			temp = newcode("LABEL_DEF", label3, 0, 0, 0);
			code_table = link(code_table, temp);
		}
	}

	if(strcmp(stmt->leftson->name, "WHILE") == 0){	//Stmt => WHILE..
		Operand* label1 = newop("LABEL", 0);
		Operand* label2 = newop("LABEL", 0);
		Operand* label3 = newop("LABEL", 0);
		InterCodes* temp = newcode("LABEL_DEF", label1, 0, 0, 0);
		code_table = link(code_table, temp);
		InterCodes* code1 = translate_Cond(stmt->leftson->rightbrother->rightbrother, label2, label3);
		code_table = link(code_table, code1);
		temp = newcode("LABEL_DEF", label2, 0, 0, 0);
		code_table = link(code_table, temp);
		code1 = translate_stmt(stmt->leftson->rightbrother->rightbrother->rightbrother->rightbrother);
		code_table = link(code_table, code1);
		temp = newcode("UNCON_JUMP", label1, 0, 0, 0);
		code_table = link(code_table, temp);
		temp = newcode("LABEL_DEF", label3, 0, 0, 0);
		code_table = link(code_table, temp);
	}

	return 0;
}

InterCodes* translate_Cond(struct syntax_tree* exp, Operand* label_true, Operand* label_false){
	if(exp->leftson->rightbrother != 0 && strcmp(exp->leftson->rightbrother->name, "RELOP") == 0){
		char* t1 = newvar();
		char* t2 = newvar();
		InterCodes* code1 = translate_exp(exp->leftson, t1);
		InterCodes* code2 = translate_exp(exp->leftson->rightbrother->rightbrother, t2);
		Operand* op1 = newop("VARIABLE", t1);
		Operand* op2 = newop("VARIABLE", t2);
		char* relop = exp->leftson->rightbrother->value.idval;
		InterCodes* code3 = newcode("CON_JUMP", op1, op2, label_true, relop);
		code3 = link(code3, newcode("UNCON_JUMP", label_false, 0, 0, 0));
		code1 = link(code1, code2);
		code1 = link(code1, code3);
		return code1;
	}
	
	if(strcmp(exp->leftson->name, "NOT") == 0){
		return translate_Cond(exp, label_false, label_true);
	}

	if(exp->leftson->rightbrother != 0 && strcmp(exp->leftson->rightbrother->name, "AND") == 0){
		Operand* label1 = newop("LABEL", 0);
		InterCodes* code1 = translate_Cond(exp->leftson, label1, label_false);
		InterCodes* code2 = translate_Cond(exp->leftson->rightbrother->rightbrother, label_true, label_false);
		InterCodes* temp = newcode("LABEL_DEF", label1, 0, 0, 0);
		code1 = link(code1, temp);
		code1 = link(code1, code2);
		return code1;
	}

	if(exp->leftson->rightbrother != 0 && strcmp(exp->leftson->rightbrother->name, "OR") == 0){
		Operand* label1 = newop("LABEL", 0);
		InterCodes* code1 = translate_Cond(exp->leftson, label_true, label1);
		InterCodes* code2 = translate_Cond(exp->leftson->rightbrother->rightbrother, label_true, label_false);
		InterCodes* temp = newcode("LABEL_DEF", label1, 0, 0, 0);
		code1 = link(code1, temp);
		code1 = link(code1, code2);
		return code1;
	}

	else{
		char* t1 = newvar();
		InterCodes* code1 = translate_exp(exp, t1);
		Operand* op1 = newop("VARIABLE", t1);
		Operand* op2 = newop_const(0);
		InterCodes* code2 = newcode("CON_JUMP", op1, op2, label_true, "!=");
		InterCodes* code3 = newcode("UNCON_JUMP", label_false, 0, 0, 0);
		code1 = link(code1, code2);
		code1 = link(code1, code3);
		return code1;
	}

	return 0;
}

InterCodes* translate_FunDec(struct syntax_tree* FunDec){
	FuncNode* sym = findFuncTab(FunDec->leftson->value.idval);
	char* fName = sym->fName;
	VariableNode* param = sym->args;
	Operand* op1 = newop("FUNC", fName);
	InterCodes* code = newcode("FUNCTION", op1, 0, 0, 0);
	while(param != 0){
		Operand* op2 = newop("VARIABLE", param->vName);
		InterCodes* code1 = newcode("PARAM", op2, 0, 0, 0);
		code = link(code, code1);
		param = param->next;
	}
	return code;
}

InterCodes* translate_Dec(struct syntax_tree* Dec){
	struct syntax_tree* VarDec = Dec->leftson;
	if(VarDec->rightbrother == 0){	//Dec => VarDec
		if(strcmp(VarDec->leftson->name, "VarDec") == 0){	//VarDec => VarDec LB INT RB
			struct syntax_tree* son = VarDec->leftson;
			int size = 4 * son->rightbrother->rightbrother->value.intval;
			while(strcmp(son->leftson->name, "VarDec") == 0){
				son = son->leftson;
				size = size * son->rightbrother->rightbrother->value.intval;
			}
			char* varName = son->leftson->value.idval;
			Operand* op1 = newop("VARIABLE", varName);
			Operand* op2 = newop_const(size);
			InterCodes* code = newcode("MEM_DEC", op1, op2, 0, 0);
			return code;
		}
	}
	else{									//Dec => VarDec ASSIGNOP Exp
		if(strcmp(VarDec->leftson->name, "ID") == 0){
			char* t1 = newvar();
			InterCodes* code1 = translate_exp(VarDec->rightbrother->rightbrother, t1);
			Operand* op1 = newop("VARIABLE", VarDec->leftson->value.idval);
			Operand* op2 = newop("VARIABLE", t1);
			InterCodes* code2 = newcode("ASSIGN", op1, op2, 0, 0);
			code1 = link(code1, code2);
			return code1;
		}
	}
	return 0;
}

InterCodes* translate_Args(struct syntax_tree* Args){
	InterCodes *code1 = 0, *code2 = 0;
	struct syntax_tree* exp = Args->leftson;
	while(exp != 0){
		char* t1 = newvar();
		InterCodes* temp1 = translate_exp(exp, t1);
		code1 = link(code1, temp1);
		Operand* op1 = newop("VARIABLE", t1);
		InterCodes* temp2 = newcode("ARG", op1, 0, 0, 0);
		code2 = link(temp2, code2);

		if(exp->rightbrother == 0)
			exp = exp->rightbrother;
		else 
			exp = exp->rightbrother->rightbrother->leftson;
	}
	return link(code1, code2);
}


void trans(struct syntax_tree* t){
	if(t == 0)
		return;

	if(strcmp(t->name, "Stmt") == 0){
		InterCodes* code = translate_stmt(t);
		code_table = link(code_table, code);
		trans(t->rightbrother);
		return;
	}

	if(strcmp(t->name, "FunDec") == 0){
		InterCodes* code = translate_FunDec(t);
		code_table = link(code_table, code);
	}

	if(strcmp(t->name, "Dec") == 0){
		InterCodes* code = translate_Dec(t);
		code_table = link(code_table, code);
	}

	trans(t->leftson);
	trans(t->rightbrother);
}

void translate(){
	trans(root);
}

