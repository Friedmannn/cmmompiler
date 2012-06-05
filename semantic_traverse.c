#include "type.h"
#include "syntax_tree.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern struct  syntax_tree* root;
extern void exp_tra(struct syntax_tree* t);

void traverse(struct syntax_tree* t){
	if(t == 0)
		return;

	if(strcmp(t->name, "ExtDef") == 0){//meet an ExtDef;
		if(strcmp(t->leftson->rightbrother->name, "ExtDecList") == 0){  //ExtDef => Specifier ExtDecList SEMI;
			struct syntax_tree* ext_dec_list = t->leftson->rightbrother;
			struct syntax_tree* p;
			Type newtype = 0;
			if(strcmp(t->leftson->leftson->name, "TYPE") == 0){  //basic type;
				struct syntax_tree* type = t->leftson->leftson;
				newtype = (Type)malloc(sizeof(struct Type_));
				newtype->kind = basic;
				newtype->u.basic = type->value.intval;
			}
			p = ext_dec_list->leftson;
			while(p != 0){
				if(strcmp(p->leftson->name, "ID") == 0){  //VarDec => ID;
					if(findVarTab(p->leftson->value.idval) == 0){
						insertVarTab(p->leftson->value.idval, newtype);
					}
					else{
						printf("Error type 3 at line %d: Variable redefined.\n", p->line);
					}
				}
				
				else if(strcmp(p->leftson->name, "VarDec") == 0){  //VarDec => VarDec LB INT RB
					struct syntax_tree* q = p->leftson;
					for(; strcmp(q->name, "ID") != 0; q = q->leftson);
					if(findVarTab(q->value.idval) == 0){
						q = p->leftson;
						while(strcmp(q->name, "VarDec") == 0){
							Type newtype2 = (Type)malloc(sizeof(struct Type_));
							newtype2->kind = array;
							newtype2->u.array.elem = newtype;
							newtype2->u.array.size = q->rightbrother->rightbrother->value.intval;
							newtype = newtype2;
							q = q->leftson;
						}
						insertVarTab(q->value.idval, newtype);
					}
					else{
						printf("Error type 3 at lint %d: Variable redefined.\n", p->line);
					}
				}

				if(p->rightbrother == 0)
					p = 0;
				else
					p = p->rightbrother->rightbrother->leftson;
			}
		}
		
		if(strcmp(t->leftson->rightbrother->name, "FunDec") == 0){  //ExtDef => Specifier FunDec CompSt
			struct syntax_tree* specifier = t->leftson;
			struct syntax_tree* fundec = specifier->rightbrother;
			Type returnType = 0;
			VariableNode* args = 0;
			char* fName = fundec->leftson->value.idval;
			if(findFuncTab(fName) != 0){
				printf("Error type 4 at line %d: Function redefined.\n", t->line);
				fName = "_DEFAUL_FUNC_NAME";
			}
			if(strcmp(specifier->leftson->name, "TYPE") == 0){  //Basic type;
				returnType = (Type)malloc(sizeof(struct Type_));
				returnType->kind = basic;
				returnType->u.basic = specifier->leftson->value.intval;
			}
			
			if(strcmp(fundec->leftson->rightbrother->rightbrother->name, "VarList") == 0){ //FunDec => ID LP VarList RP
				struct syntax_tree* paramDec = fundec->leftson->rightbrother->rightbrother->leftson;
				while(paramDec != 0){
					Type varType = 0;
					char* varName;
					VariableNode* newArg;
					struct syntax_tree* varDec = paramDec->leftson->rightbrother->leftson;  //may be a VarDec or an ID
					if(strcmp(paramDec->leftson->leftson->name, "TYPE") == 0){
						varType = (Type)malloc(sizeof(struct Type_));
						varType->kind = basic;
						varType->u.basic = paramDec->leftson->leftson->value.intval;
					}
					
					for(; strcmp(varDec->name, "VarDec") == 0; varDec = varDec->leftson){
						Type newType = (Type)malloc(sizeof(struct Type_));
						newType->kind = array;
						newType->u.array.elem = varType;
						newType->u.array.size = varDec->rightbrother->rightbrother->value.intval;
						varType = newType;
					}
					
					//Now varDec is an ID
					varName = varDec->value.idval;

					//Insert the current ParamDec to Variable Table and the argsLink
					if(findVarTab(varName) == 0)
						insertVarTab(varName, varType);
					else
						printf("Error type 3 at line %d: Variable redefined.\n", t->line);

					newArg = (VariableNode*)malloc(sizeof(VariableNode));
					newArg->vName = (char*)malloc(strlen(varName) + 1);
					strcpy(newArg->vName, varName);
					newArg->varType = varType;
					newArg->next = 0;
					if(args == 0)
						args = newArg;
					else{
						VariableNode* p = args;
						for(; p->next != 0; p = p->next);
						p->next = newArg;
					}

					if(paramDec->rightbrother == 0)
						paramDec = 0;
					else
						paramDec = paramDec->rightbrother->rightbrother->leftson;
				}
			}
			insertFuncTab(fName, returnType, args);
		}
	}

	if(strcmp(t->name, "Def") == 0){  //meet a Def;
		Type newtype = 0;
		struct syntax_tree* p;
		if(strcmp(t->leftson->leftson->name, "TYPE") == 0){
			struct syntax_tree* type = t->leftson->leftson;
			newtype = (Type)malloc(sizeof(struct Type_));
			newtype->kind = basic;
			newtype->u.basic = type->value.intval;
		}
		p = t->leftson->rightbrother->leftson;
		while(p != 0){  //p is Dec;
			struct syntax_tree* var_dec = p->leftson;
			for(; strcmp(var_dec->name, "ID") != 0; var_dec = var_dec->leftson);
			if(findVarTab(var_dec->value.idval) == 0){
				var_dec = p->leftson->leftson;
				while(strcmp(var_dec->name, "VarDec") == 0){
					Type newtype2 = (Type)malloc(sizeof(struct Type_));
					newtype2->kind = array;
					newtype2->u.array.elem = newtype;
					newtype2->u.array.size = var_dec->rightbrother->rightbrother->value.intval;
					newtype = newtype2;
					var_dec = var_dec->leftson;
				}
				insertVarTab(var_dec->value.idval, newtype);

			}
			else{
				printf("Error type 3 at line %d: Variable redefined.\n", p->line);
			}

			if(p->leftson->rightbrother != 0){  //Dec => VarDec ASSIGNOP Exp
				exp_tra(p->leftson->rightbrother->rightbrother);
				if(p->leftson->rightbrother->rightbrother->exp_type != 0 && !type_equal(newtype, p->leftson->rightbrother->rightbrother->exp_type)){
					printf("Error type 5 at line %d: Assignop type mismatched.\n", p->line);
				}
			}


			if(p->rightbrother == 0)
				p = 0;
			else
				p = p->rightbrother->rightbrother->leftson;
		}
	}

	if(strcmp(t->name, "Stmt") == 0){  // meet a Stmt
		struct syntax_tree* tmp = t->leftson;
		for(;tmp != 0; tmp = tmp->rightbrother){
			exp_tra(tmp);
		}
		if(strcmp(t->leftson->name, "RETURN") == 0){  //Stmt => RETURN Exp SEMI
			if(t->leftson->rightbrother->exp_type != 0 && type_equal(funcTable->returnType, t->leftson->rightbrother->exp_type) == 0){
				printf("Error type 8 at line %d: Return type mismatched.\n", t->line);
			}
		}
	}

	if(strcmp(t->name, "StructSpecifier") == 0){  //meet a StructSpecifier;
		traverse(t->rightbrother);
		return;
	}

	traverse(t->leftson);
	traverse(t->rightbrother);
}

void semantic_traverse(){
	initial_sym_table();
	traverse(root);
}



void exp_tra(struct syntax_tree* t){
	if(t == 0 || strcmp(t->name, "Exp") != 0)
		return;
	
	if(strcmp(t->leftson->name, "FLOAT") == 0){
		t->exp_type = (Type)malloc(sizeof(struct Type_));
		t->exp_type->kind = basic;
		t->exp_type->u.basic = 2;
	}

	else if(strcmp(t->leftson->name, "INT") == 0){
		t->exp_type = (Type)malloc(sizeof(struct Type_));
		t->exp_type->kind = basic;
		t->exp_type->u.basic = 1;
	}

	else if(strcmp(t->leftson->name, "ID") == 0 && t->leftson->rightbrother == 0){
		if(t->leftson->rightbrother == 0){  //Exp => ID
			VariableNode* new_node = findVarTab(t->leftson->value.idval);
			if(new_node == 0){
				printf("Error type 1 at line %d: Variable undefined.\n", t->line);
			}
			else{
				t->exp_type = new_node->varType;
			}
		}
	}

	else if(strcmp(t->leftson->name, "LP") == 0){  //Exp => LP Exp RP
		exp_tra(t->leftson->rightbrother);
		t->exp_type = t->leftson->rightbrother->exp_type;
	}

	else if(strcmp(t->leftson->name, "MINUS") == 0){  //Exp =>MiNUS Exp;
		exp_tra(t->leftson->rightbrother);
		if(t->leftson->rightbrother->exp_type->kind != basic){
			printf("Error type 7 at line %d: Operand type mismatched.\n", t->line);
		}
		else{
			t->exp_type = t->leftson->rightbrother->exp_type;
		}
	}

	else if(strcmp(t->leftson->name, "NOT") == 0){  //Exp => NOT Exp;
		exp_tra(t->leftson->rightbrother);
		if(!(t->leftson->rightbrother->exp_type->kind == basic && t->leftson->rightbrother->exp_type->u.basic == 1)){
			printf("Error type 7 at line %d: Operand type mismatched.\n", t->line);
		}
		else{
			t->exp_type = t->leftson->rightbrother->exp_type;
		}
	}

	else if(strcmp(t->leftson->rightbrother->name, "ASSIGNOP") == 0){  //Exp => Exp ASSIGNOP Exp
		exp_tra(t->leftson);
		exp_tra(t->leftson->rightbrother->rightbrother);
		if(strcmp(t->leftson->leftson->name, "INT") == 0 || 
				strcmp(t->leftson->leftson->name, "FLOAT") == 0 ||
				strcmp(t->leftson->leftson->name, "MINUS") == 0 ||
				strcmp(t->leftson->leftson->name, "NOT") == 0 ||
				((t->leftson->leftson->rightbrother != 0) && (strcmp(t->leftson->leftson->rightbrother->name, "AND") == 0 ||
				strcmp(t->leftson->leftson->rightbrother->name, "OR") == 0 ||
				strcmp(t->leftson->leftson->rightbrother->name, "RELOP") == 0 ||
				strcmp(t->leftson->leftson->rightbrother->name, "PLUS") == 0 ||
				strcmp(t->leftson->leftson->rightbrother->name, "MINUS") == 0 ||
				strcmp(t->leftson->leftson->rightbrother->name, "STAR") == 0 ||
				strcmp(t->leftson->leftson->rightbrother->name, "DIV") == 0 ||
				strcmp(t->leftson->leftson->rightbrother->name, "LP") == 0))){
			printf("Error type 6 at line %d: Exp on the left of '=' has only right value.\n", t->line);
		}

		if(t->leftson->exp_type != 0 && t->leftson->rightbrother->rightbrother->exp_type != 0 && !type_equal(t->leftson->exp_type, t->leftson->rightbrother->rightbrother->exp_type)){
			printf("Error type 5 at line %d: Assignop type mismatched.\n", t->line);
		}
		t->exp_type = t->leftson->exp_type;
	}

	else if(strcmp(t->leftson->rightbrother->name, "AND") == 0 || strcmp(t->leftson->rightbrother->name, "OR") == 0){  //Exp => Exp (AND | OR) Exp
		exp_tra(t->leftson);
		exp_tra(t->leftson->rightbrother->rightbrother);
		if(!(t->leftson->exp_type->kind == basic && t->leftson->exp_type->u.basic == 1) || !(t->leftson->rightbrother->rightbrother->exp_type->kind == basic && t->leftson->rightbrother->rightbrother->exp_type->u.basic == 1)){
			printf("Error type 7 at line %d: Operand type mismatched.\n", t->line);
		}
		else{
			t->exp_type = t->leftson->exp_type;
		}
	}

	else if(strcmp(t->leftson->rightbrother->name, "PLUS") == 0 || strcmp(t->leftson->rightbrother->name, "MiNUS") == 0 || strcmp(t->leftson->rightbrother->name, "STAR") == 0 || strcmp(t->leftson->rightbrother->name, "DIV") == 0){  
		//Exp => Exp PLUS Exp | Exp MINUS Exp | Exp STAR Exp | Exp DIV Exp
		exp_tra(t->leftson);
		exp_tra(t->leftson->rightbrother->rightbrother);
		if(!type_equal(t->leftson->exp_type, t->leftson->rightbrother->rightbrother->exp_type) || t->leftson->exp_type->kind != basic || t->leftson->rightbrother->rightbrother->exp_type->kind != basic){
			printf("Error type 7 at line %d: Operand type mismatched.\n", t->line);
		}
		else{
			t->exp_type = t->leftson->exp_type;
		}
	}

	else if(strcmp(t->leftson->rightbrother->name, "LB") == 0){  //Exp => Exp LB Exp RB
		//printf("Exp => Exp LB Exp\n");
		exp_tra(t->leftson);
		exp_tra(t->leftson->rightbrother->rightbrother);
		if(t->leftson->rightbrother->rightbrother->exp_type != 0 && (!(t->leftson->rightbrother->rightbrother->exp_type->kind == basic && t->leftson->rightbrother->rightbrother->exp_type->u.basic == 1))){
			printf("Error type 12 at line %d: Exp in [] is not integer.\n", t->line);
		}
		if(t->leftson->exp_type != 0 && t->leftson->exp_type->kind != array){
			printf("Error type 10 at line %d: [] Can't be apply to variable which is not an array.\n", t->line);
		}
		else if(t->leftson->exp_type != 0){
			t->exp_type = t->leftson->exp_type->u.array.elem;
		}
	}

	else if(strcmp(t->leftson->rightbrother->name, "LP") == 0){  //Exp => ID LP Args RP | ID LP RP
		FuncNode* func = findFuncTab(t->leftson->value.idval);
		if(func == 0){
			if(findVarTab(t->leftson->value.idval) == 0){
				printf("Error type 2 at line %d: Function undefined.\n", t->line);
			}
			else{
				printf("Error type 11 at line %d: Apply () to normal Variable.\n", t->line);
			}
		}
		else{
			VariableNode* args = func->args;
			if(args == 0){
				if(t->leftson->rightbrother->rightbrother->leftson != 0){
					printf("Error type 9 at line %d: Function args mismatched.\n", t->line);
				}
			}
			else{
				if(t->leftson->rightbrother->rightbrother->leftson == 0){
					printf("Error type 9 at line %d: Function args mismatched.\n", t->line);
				}
				else{
					struct syntax_tree* exp = t->leftson->rightbrother->rightbrother->leftson;
					VariableNode* currentArg = args;
					while(exp != 0){
						exp_tra(exp);
						if(!type_equal(currentArg->varType, exp->exp_type)){
							printf("Error type 9 at line %d: Function args mismatched.\n", t->line);
							break;
						}

						currentArg = currentArg->next;

						if(exp->rightbrother != 0){
							if(currentArg == 0){
								printf("Error type 9 at line %d: Function args mismatched.\n", t->line);
								break;
							}
							exp = exp->rightbrother->rightbrother->leftson;
						}
						else
							exp = 0;
					}
				}
			}
			t->exp_type = func->returnType;
		}
	}
}




