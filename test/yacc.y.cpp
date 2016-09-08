%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <string.h>
using namespace std;

int yyerror(const char*);
#include "lex.c"  //这是词法分析器生成的文件，必须包含！

map<string, double> reg;// 这里可以写一些全局的代码
static BIGINT ival;

int i;
%}

%token		TK_NUM ID TK_STR IF ELSE ELSEIF FOR CASE IN TK_FUNC RETURN WHILE BREAK INC DEC SIZEOF SWITCH DEFAULT

%left		ASSIGN
%left		AND OR
%left		EQ NE LT GT GE LE
%left		ADD SUB
%left		MUL DIV
%left		CONCAT

%nonassoc IFX
%nonassoc ELSEIF
%nonassoc ELSE

%%

program:
	stats {GenCode("EOP");}
	;
stats:
    stats stat
    | stat
    ;
stat:
	expr ';' {GenCode("ClearStack");}
	| expr_if
	| '{' stats '}'
	| stat_assign
	;
stat_assign:
	expr_prefix  ASSIGN expr_not_empty_list  ';'
	;
expr:
	expr_binop | table
expr_not_empty_list:
	expr
	| expr_not_empty_list ',' expr
	;
expr_if:
	IF expr stat  expr_elseif_list %prec IFX
	| IF expr stat  expr_elseif_list expr_else
expr_elseif_list:
	expr_elseif_list expr_elseif
	|
	;
expr_elseif:
	ELSEIF expr stat
expr_else:
	ELSE stat
	;
table:
	'`' table_body '@'
table_body:
	table_body ',' table_expr
	| table_expr
	|
table_expr:
	'[' expr ']' ASSIGN expr {GenCode("InsertIndexStack");}
	| ID ASSIGN expr {GenCode("InsertIndexStr", $1.str);}
	| table {GenCode("InsertIntoStack");}
	| expr_binop {GenCode("InsertIntoStack");}

expr_binop:
	expr_binop ADD expr_binop	{ GenCode("Add");}
	| expr_binop SUB expr_binop { GenCode("Sub");}
	| expr_binop MUL expr_binop { GenCode("Mul");}
	| expr_binop DIV expr_binop { GenCode("Div");}
	| expr_binop LT expr_binop {GenCode("Lt");}
	| expr_binop GT expr_binop {GenCode("Gt");}
	| expr_binop EQ expr_binop {GenCode("Eq");}
	| expr_binop NE expr_binop {GenCode("Ne");}
	| expr_binop GE expr_binop {GenCode("Ge");}
	| expr_binop LE expr_binop {GenCode("Le");}
	| expr_binop CONCAT expr_binop {GenCode("Concat");}
	| expr_binop AND expr_binop { GenCode("And");}
	| expr_binop OR expr_binop { GenCode("Or");} // 13 warnings here,
	| expr_prefix
expr_prefix:
	'!' expr_postfix {GenCode("Negate");}
	| SIZEOF expr_postfix {GenCode("Sizeof");}
	| INC expr_postfix {GenCode("Inc");}
	| DEC expr_postfix {GenCode("Dec");}
	| expr_postfix //45
expr_postfix:
	expr_postfix '.' ID
	| expr_postfix '[' expr ']'
	| expr_postfix '\\' expr_param_list '/'//49
	| expr_postfix ':' ID '(' expr_param_list ')'
	| expr_primary
expr_param_list:
	expr_param_list ',' expr
	| expr
	|
expr_primary:
	TK_NUM
	| ID
	| TK_STR
	| '(' expr ')'

%%

int yyerror(const char *str){
	//if (feof(yyin)) return 1;
 	Util::Println("error:%s:%d:%s:expect %s before %s\n", "filename", yylineno, str, "?", yytext);
	return 1;
 }

int yywrap(){
        // 这个函数是做什么的？
        // 简单的说，yywrap()在当前输入的符号流（token 流）结束时（比如碰到了EOF）被调用。
	// 所以,如果当存在若干个token流需要被解析（包括词法、语法解析）时，可以在此处对yyin这个
        // 内置的文件指针进行重新指派（比如yyin=fopen("xxx","r")或者rewind(yyin)啦）。
        // 当yyin重新被指派后，为了开始新的解析，本函数必须return 0。
        // 当没有新的输入流需要解析时，本函数须 return一非零值，以结束解析。[/color]

      return 1;
}
int yymain(string filename){
	yyin = fopen(filename.c_str(), "r");
	if (yyin == NULL) { printf("failed to open file %s in read mode\n", filename.c_str()); exit(-1);}
	yyparse();
	fclose(yyin);
}


