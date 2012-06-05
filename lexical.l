%{
	#include <stdlib.h>
	#include "syntax.tab.h"
	#include "syntax_tree.c"
%}

%{
	extern is_error;
%}

letter [_a-zA-Z]
digit [0-9]
hex [0-9a-fA-F]

%option yylineno

%%
({digit}+)|(0[xX]{hex}+) {
	int d;
	if(yytext[0] != '0'){//decimal;
		d = strtol(yytext, 0, 10);
	}
	else{
		if(yytext[1] == 'x' || yytext[1] == 'X')
			d = strtol(yytext, 0, 16);//hex;
		else
			d = strtol(yytext, 0, 8);//oct;
	}
	yylval.type_tree = new_tree("INT");
	yylval.type_tree->value.intval = d;
	return INT;
}
({digit}+\.{digit}+)|({digit}+(\.{digit}+)?[eE][+-]?{digit}+) {
	float f = atof(yytext);
	yylval.type_tree = new_tree("FLOAT");
	yylval.type_tree->value.floval = f;
	return FLOAT;
}
";" {
	/*yylval.type_tree = (struct syntax_tree*)malloc(sizeof(struct syntax_tree));
	yylval.type_tree->value.intval = 0;
	yylval.type_tree->type = SEMI;
	yylval.type_tree->leftson = 0;
	yylval.type_tree->rightbrother = 0;
	*/
	yylval.type_tree = new_tree("SEMI");
	return SEMI;
}
"," {
	yylval.type_tree = new_tree("COMMA");
	return COMMA;
}
"=" {
	yylval.type_tree = new_tree("ASSIGNOP");
	return ASSIGNOP;
}
>|<|>=|<=|==|!= {
	yylval.type_tree = new_tree("RELOP");
	yylval.type_tree->value.idval = (char*)malloc(strlen(yytext)+1);
	yylval.type_tree->value.idval = strcpy(yylval.type_tree->value.idval, yytext);
	return RELOP;
}
"+" {
	yylval.type_tree = new_tree("PLUS");
	return PLUS;
}
"-" {
	yylval.type_tree = new_tree("MINUS");
	return MINUS;
}
"*" {
	yylval.type_tree = new_tree("STAR");
	return STAR;
}
"/" {
	yylval.type_tree = new_tree("DIV");
	return DIV;
}
"&&" {
	yylval.type_tree = new_tree("AND");
	return AND;
}
"||" {
	yylval.type_tree = new_tree("OR");
	return OR;
}
"." {
	yylval.type_tree = new_tree("DOT");
	return DOT;
}
"!" {
	yylval.type_tree = new_tree("NOT");
	return NOT;
}
"int"|"float" {
	yylval.type_tree = new_tree("TYPE");
	if(yytext[0] == 'i')//int
		yylval.type_tree->value.intval = 1;
	else//float
		yylval.type_tree->value.intval = 2;
	return TYPE;
}
"(" {
	yylval.type_tree = new_tree("LP");
	return LP;
}
")" {
	yylval.type_tree = new_tree("RP");
	return RP;
}
"[" {
	yylval.type_tree = new_tree("LB");
	return LB;
}
"]" {
	yylval.type_tree = new_tree("RB");
	return RB;
}
"{" {
	yylval.type_tree = new_tree("LC");
	return LC;
}
"}" {
	yylval.type_tree = new_tree("RC");
	return RC;
}
"struct" {
	yylval.type_tree = new_tree("STRUCT");
	return STRUCT;
}
"return" {
	yylval.type_tree = new_tree("RETURN");
	return RETURN;
}
"if" {
	yylval.type_tree = new_tree("IF");
	return IF;
}
"else" {
	yylval.type_tree = new_tree("ELSE");
	return ELSE;
}
"while" {
	yylval.type_tree = new_tree("WHILE");
	return WHILE;
}
{letter}({letter}|{digit})* {
	yylval.type_tree = new_tree("ID");
	yylval.type_tree->value.idval = (char*)malloc(strlen(yytext)+1);
	yylval.type_tree->value.idval = strcpy(yylval.type_tree->value.idval, yytext);
	return ID;
}
[\n\t ]+ {}
"//" {
		char c = input();
		while(c != '\n' && c != EOF) c = input();
}
"/*" {
		char c = input();
		char c_ = input();
		while(!(c == '*' && c_ == '/')){
			if(c == '/' && c_ == '*'){
				is_error = 1;
				printf("Error type 3 at line %d: Unexpected \'\\*\'\n", yylineno);
			}
			if(c_ == EOF){
				is_error = 1;
				printf("Error type 3 at line %d: Expected \'*\\\' unfound\n", yylineno);
				break;
			}
			c = c_;
			c_ = input();
		}
}
"*/" {
	is_error = 1;
	printf("Error type 3 at line %d: Unexpected \'*/\'\n", yylineno);
}
. {
	is_error = 1;
	printf("Error type 1 at line %d: Mysterious character \'%s\'\n", yylineno, yytext);}
%%


