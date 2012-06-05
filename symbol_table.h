#ifndef GUARD_SYMBOL_TABLE
#define GUARD_SYMBOL_TABLE

#include "type.h"

/*Variables*/
typedef struct VariableNode_ VariableNode;
struct VariableNode_{
	char* vName;
	Type varType;
	VariableNode* next;
};

extern VariableNode* varTable;

extern void insertVarTab(char* vName, Type varType);
extern VariableNode* findVarTab(char* vName);
extern void print_var_table();

/*Functions*/
typedef struct FuncNode_ FuncNode;
struct FuncNode_{
	char* fName;
	Type returnType;
	VariableNode* args;
	FuncNode* next;
};

extern FuncNode* funcTable;

extern void insertFuncTab(char* fName, Type returnType, VariableNode* args);
extern FuncNode* findFuncTab(char* fName);
extern void print_func_table();

extern void initial_sym_table();

#endif
