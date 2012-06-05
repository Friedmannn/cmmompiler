#ifndef GUARD_TRANSLATE
#define GUARD_TRANSLATE

#include "code.h"
#include "symbol_table.h"
#include "syntax_tree.h"

extern VariableNode* varTable;
extern FuncNode* funcTable;

extern struct syntax_tree* root;

extern InterCodes* code_table;

extern void translate();

#endif
