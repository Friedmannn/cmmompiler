#ifndef SYNTAXTREE
#define SYNTAXTREE

#include "type.h"
#include <stdio.h>
#include <stdlib.h>

struct syntax_tree{
	int type;//0代表词法单元，1代表语法单元；
	union{
		int intval;//when name is TYPE, 1 is int and 2 is float;
		float floval;
		char* idval;
	}value;
	int line;
	char name[20];
	struct syntax_tree* leftson;
	struct syntax_tree* rightbrother;

	struct Type_ *exp_type;
	//Type exp_type;
};

extern struct syntax_tree* root;

extern struct syntax_tree* new_tree(char* name);
extern struct syntax_tree* build(int n,...);

extern void print_tree();

#endif
