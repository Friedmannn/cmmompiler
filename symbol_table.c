#include <string.h>
#include "symbol_table.h"
#include <stdlib.h>
#include <stdio.h>

VariableNode* varTable;
FuncNode* funcTable;

void insertVarTab(char* vName, Type varType){
	VariableNode* newNode;
	void* p = 0;
	p = malloc(sizeof(VariableNode));
	if(p == 0){
		printf("内存分配失败！\n");
		return;
	}
	newNode = (VariableNode*)p;

	p = malloc(strlen(vName) + 1);
	if(p == 0){
		printf("内存分配失败！\n");
		return;
	}
	newNode->vName = (char*)p;
	strcpy(newNode->vName, vName);
	
	newNode->varType = varType;

	newNode->next = varTable;
	varTable = newNode;
}

VariableNode* findVarTab(char* vName){
	VariableNode* pNode = varTable;
	while(pNode != 0){
		if(strcmp(pNode->vName, vName) == 0)
			break;
		pNode = pNode->next;
	}
	return pNode;
}

void insertFuncTab(char* fName, Type returnType, VariableNode* args){
	FuncNode* newNode = (FuncNode*)malloc(sizeof(FuncNode));
	newNode->fName = (char*)malloc(strlen(fName) + 1);
	strcpy(newNode->fName, fName);
	newNode->returnType = returnType;
	newNode->args = args;

	newNode->next = funcTable;
	funcTable = newNode;
}

FuncNode* findFuncTab(char* fName){
	FuncNode* pNode = funcTable;
	for(;pNode != 0; pNode = pNode->next){
		if(strcmp(pNode->fName, fName) == 0)
			break;
	}
	return pNode;
}

void print_var_table(){
	VariableNode* p;
	for(p = varTable; p != 0; p = p->next){
		if(p->varType->kind == basic){
			printf("%s:", p->vName);
			if(p->varType->u.basic == 1)
				printf("int\n");
			else
				printf("float\n");
		}
		else if(p->varType->kind == array){
			Type tp = p->varType;
			printf("%s:", p->vName);
			while(tp->kind != basic){
				printf("[%d]", tp->u.array.size);
				tp = tp->u.array.elem;
			}
			if(tp->u.basic == 1)
				printf("int\n");
			else
				printf("float\n");
		}
	}
	printf("\n");
}

void print_func_table(){
	FuncNode* p;
	for(p = funcTable; p != 0; p = p->next){
		VariableNode* v;
		printf("%s\n", p->fName);
		if(p->returnType->u.basic == 1)
			printf("return:int\n");
		else if(p->returnType->u.basic == 2)
			printf("return:float\n");
		printf("args:\n");
		for(v = p->args; v != 0; v = v->next){
			printf("%s:", v->vName);
			if(v->varType->kind == basic){
				if(v->varType->u.basic == 1)
					printf("int, ");
				else
					printf("float, ");
			}
			else if(v->varType->kind == array){
				Type tp = v->varType;
				while(tp->kind != basic){
					printf("[%d]", tp->u.array.size);
					tp = tp->u.array.elem;
				}
				if(tp->u.basic == 1)
					printf("int, ");
				else
					printf("float, ");
			}
		}
		printf("\n");
	}
}

void initial_sym_table(){
	VariableNode* arg = (VariableNode*)malloc(sizeof(VariableNode));
	arg->vName = "writeArg";
	arg->varType = (Type)malloc(sizeof(struct Type_));
	arg->varType->kind = basic;
	arg->varType->u.basic = 1;
	arg->next = 0;
	insertFuncTab("write", arg->varType, arg);
	insertFuncTab("read", arg->varType, 0);
}
