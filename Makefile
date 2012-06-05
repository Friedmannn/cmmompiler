translator:main.c syntax.tab.c lex.yy.c syntax_tree.c semantic_traverse.c symbol_table.c type.c code.c translate.c happy_tree_friends.c
	gcc \
	main.c \
	syntax.tab.c \
	symbol_table.c \
	type.c \
	semantic_traverse.c \
	happy_tree_friends.c \
	code.c \
	translate.c \
	-lfl -ly \
	-o translator

syntax.tab.c:syntax.y 
	bison -d syntax.y

lex.yy.c:lexical.l
	flex lexical.l


clean:
	rm translator syntax.tab.c lex.yy.c 


