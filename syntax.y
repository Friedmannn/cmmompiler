%{
	#include <stdio.h>
	#include "syntax_tree.h"

	int is_error = 0;
%}


/*Declared types*/
%union{
	struct syntax_tree* type_tree;
}


/*Declared tokens*/
%token <type_tree> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

%right ASSIGNOP
%left PLUS MINUS
%left STAR DIV
%left LP RP

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/*Declared non-terminals*/
%type <type_tree> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%%
Program				: ExtDefList	{$$ = build(3, "Program", $1);
									root = $$;}
ExtDefList			: /*empty*/	{$$ = new_tree("ExtDefList"); $$->type = 1;}
					| ExtDef ExtDefList	{$$ = build(4, "ExtDefList", $1, $2);}
ExtDef              : Specifier ExtDecList SEMI	{$$ = build(5, "ExtDef", $1, $2, $3);}
					| Specifier SEMI	{$$ = build(4, "ExtDef", $1, $2);}
					| Specifier FunDec CompSt	{$$ = build(5, "ExtDef", $1, $2, $3);}
					| error SEMI	{yyerrok;}
ExtDecList			: VarDec	{$$ = build(3, "ExtDecList", $1);}
					| VarDec COMMA ExtDecList	{$$ = build(5, "ExtDecList",$1, $2, $3);}

Specifier			: TYPE	{$$ = build(3, "Specifier", $1);}
					| StructSpecifier	{$$ = build(3, "Specifier", $1);}
StructSpecifier		: STRUCT OptTag LC DefList RC	{$$ = build(7, "StructSpecifier", $1, $2, $3, $4, $5);}
					| STRUCT Tag	{$$ = build(4, "StructSpecifier", $1, $2);}
OptTag				: ID	{$$ = build(3, "OptTag", $1);}
					| /*Empty*/	{$$ = new_tree("OptTag"); $$->type = 1;}
Tag					: ID	{$$ = build(3, "Tag", $1);}

VarDec				: ID	{$$ = build(3, "VarDec", $1);}
					| VarDec LB INT RB	{$$ = build(6, "VarDec", $1, $2, $3, $4);}
FunDec				: ID LP VarList RP	{$$ = build(6, "FunDec", $1, $2, $3, $4);}
					| ID LP RP	{$$ = build(5, "FunDec", $1, $2, $3);}
VarList				: ParamDec COMMA VarList	{$$ = build(5, "VarList", $1, $2, $3);}
					| ParamDec	{$$ = build(3, "VarList", $1);}
ParamDec			: Specifier VarDec	{$$ = build(4, "ParamDec", $1, $2);}

CompSt				: LC DefList StmtList RC	{$$ = build(6, "CompSt", $1, $2, $3, $4);}
					| error RC	{yyerrok;}
StmtList			: Stmt StmtList	{$$ = build(4, "StmtList", $1, $2);}
					| /*Empty*/	{$$ = new_tree("StmtList"); $$->type = 1;}
Stmt				: Exp SEMI	{$$ = build(4, "Stmt", $1, $2);}
					| CompSt	{$$ = build(3, "Stmt", $1);}
					| RETURN Exp SEMI	{$$ = build(5, "Stmt", $1, $2, $3);}
					| IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE{$$ = build(7, "Stmt", $1, $2, $3, $4, $5);}
					| IF LP Exp RP Stmt ELSE Stmt	{$$ = build(9, "Stmt", $1, $2, $3, $4, $5, $6, $7);}
					| WHILE LP Exp RP Stmt	{$$ = build(7, "Stmt", $1, $2, $3, $4, $5);}
					| error SEMI	{yyerrok;}

DefList				: Def DefList	{$$ = build(4, "DefList", $1, $2);}
					| /*Empty*/	{$$ = new_tree("DefList"); $$->type = 1;}
Def					: Specifier DecList SEMI	{$$ = build(5, "Def", $1, $2, $3);}
DecList				: Dec	{$$ = build(3, "DecList", $1);}
					| Dec COMMA DecList	{$$ = build(5, "DecList", $1, $2, $3);}
Dec					: VarDec	{$$ = build(3, "Dec", $1);}
					| VarDec ASSIGNOP Exp	{$$ = build(5, "Dec", $1, $2, $3);}

Exp					: Exp ASSIGNOP Exp	{$$ = build(5, "Exp", $1, $2, $3);}
					| Exp AND Exp	{$$ = build(5, "Exp", $1, $2, $3);}
					| Exp OR Exp	{$$ = build(5, "Exp", $1, $2, $3);}
					| Exp RELOP Exp {$$ = build(5, "Exp", $1, $2, $3);}
					| Exp MINUS Exp	{$$ = build(5, "Exp", $1, $2, $3);}
					| Exp PLUS Exp	{$$ = build(5, "Exp", $1, $2, $3);}
					| Exp STAR Exp	{$$ = build(5, "Exp", $1, $2, $3);}
					| Exp DIV Exp	{$$ = build(5, "Exp", $1, $2, $3);}
					| LP Exp RP		{$$ = build(5, "Exp", $1, $2, $3);}
					| MINUS Exp		{$$ = build(4, "Exp", $1, $2);}
					| NOT Exp		{$$ = build(4, "Exp", $1, $2);}
					| ID LP Args RP	{$$ = build(6, "Exp", $1, $2, $3, $4);}
					| ID LP RP		{$$ = build(5, "Exp", $1, $2, $3);}
					| Exp LB Exp RB	{$$ = build(6, "Exp", $1, $2, $3, $4);}
					| Exp DOT ID	{$$ = build(5, "Exp", $1, $2, $3);}
					| INT			{$$ = build(3, "Exp", $1);}
					| FLOAT			{$$ = build(3, "Exp", $1);}
					| ID			{$$ = build(3, "Exp", $1);}
Args				: Exp COMMA Args	{$$ = build(5, "Args", $1, $2, $3);}
					| Exp			{$$ = build(3, "Args", $1);}
%%

#include "lex.yy.c"

yyerror(char* msg){
	is_error = 1;
	printf("Error type 2 at line %d: %s\n", yylineno, msg);
}
