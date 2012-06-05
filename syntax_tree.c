#include <stdarg.h>

#include "syntax_tree.h"

extern int yylineno;

struct syntax_tree* root;

struct syntax_tree* new_tree(char* name){
	int i;
	struct syntax_tree* t = (struct syntax_tree*)malloc(sizeof(struct syntax_tree));
	t->value.intval = 0;
	t->leftson = 0;
	t->rightbrother = 0;
	for(i = 0; name[i] != 0; i++)
		t->name[i] = name[i];
	t->name[i] =name[i];
	t->type = 0;//默认为词法单元；
	t->line = yylineno;
	t->exp_type = 0;
	return t;
}

struct syntax_tree* build(int n,...){
	va_list p;
	char* name;
	struct syntax_tree* papa, *tmp1, *tmp2;
	int i;
	if(n <= 2)
		return 0;
	va_start(p, n);
	name = va_arg(p, char*);//get the name;
	papa = new_tree(name);

	tmp1 = va_arg(p, struct syntax_tree*);//get the first son;
	papa->leftson = tmp1;
	
	for(i = 3; i < n; i++){
		tmp2 = va_arg(p, struct syntax_tree*);
		tmp1->rightbrother = tmp2;
		tmp1 = tmp2;
	}

	papa->type = 1;
	papa->line = papa->leftson->line;
	return papa;
}

void printt(struct syntax_tree* t, int n){
	int i;
	if(t == 0)
		return;
	if(t->type == 1 && t->leftson != 0){
		for(i = 0; i < n; i++)
			printf("  ");
		printf("%s (%d)\n", t->name, t->line);
	}
	else if(t->type == 0){
		for(i = 0; i < n; i++)
			printf("  ");
		printf("%s", t->name);
		if(strcmp(t->name, "INT") == 0){
			printf(": %d", t->value.intval);
		}
		else if(strcmp(t->name, "FLOAT") == 0){
			printf(": %f", t->value.floval);
		}
		else if(strcmp(t->name, "ID") == 0){
			printf(": %s", t->value.idval);
		}
		else if(strcmp(t->name, "TYPE") == 0){
			if(t->value.intval == 1)
				printf(": int");
			else
				printf(": float");
		}
		printf("\n");
	}
	printt(t->leftson, n+1);
	printt(t->rightbrother, n);
}

void print_tree(){
	printt(root, 0);
}


