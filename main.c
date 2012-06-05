#include <stdio.h>
#include "syntax_tree.h"
#include "semantic_traverse.h"
#include "translate.h"

extern int is_error;
extern void semantic_traverse();

int main(int argc, char** argv){
	if(argc <= 2) return 1;
	FILE* f = fopen(argv[1], "r");
	if(!f){
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();
	if(!is_error){
		//print_tree();
		semantic_traverse();
	}
	/*print_var_table();
	printf("====================\n");
	print_func_table();*/

	translate();
	print_code(argv[2]);
	return 0;
}


