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

%token		TK_NUM ID TK_STR IF ELSE ELSEIF FOR CASE IN TK_FUNC RETURN WHILE BREAK INC DEC SWITCH DEFAULT DEFER EXCEPT

%left		ASSIGN
%left		AND OR
%left		EQ NE LT GT GE LE
%left		ADD SUB
%left		MUL DIV
%left		CONCAT


%nonassoc EMPTY
%nonassoc ELSE
%nonassoc ELSEIF



%%

program:
	stats {GenCode("EOP");}

stats:
    stats stat
    |

stat:
	stat_clear_stack {GenCode("ClearStack");}
	| expr_if | expr_for | expr_switch | expr_while | BREAK ';' {GenCode("Break");}
	| '{' stats '}'
	| DEFER stat
	| stat_return
	| ';'
stat_clear_stack:
	expr ';'
	| expr_func

stat_return:
	RETURN stat_return_body ';' { GenCode("Return");}
stat_return_body:
	expr
	| expr ',' stat_return_body
	|

expr:
	expr_binop | table

expr_func:
	expr_func_delegate expr_func_name_delegate '(' expr_func_param_list_delegate ')' '{' expr_func_body '}' {GenCode("FuncEnd"); Operate("PopFunction");}
expr_func_delegate:
	TK_FUNC
expr_func_name_delegate:
	expr_func_name {GenCode("SetTypeFunc"); Operate("PushFunction");}
expr_func_name:
	expr_func_name '.'  ID {GenCode("IndexStr", $3.str);}
	| ID {GenCode("Load", $1.str);}
	//| {Operate("FuncWithNoName");}
expr_func_param_list_delegate:
	expr_func_param_list
expr_func_param_list:
	expr_func_param ',' expr_func_param_list
	| expr_func_param
	|
expr_func_body:
	stats
expr_func_param:
	ID {GenCode("LoadParam", $1.str);}

expr_switch:
	expr_switch_delegate expr_switch_cond '{' expr_switch_body '}' {GenCode("PushContext"); Operate("PatchJumpEnd"); Operate("PopParseRecord");}
expr_switch_delegate:
	SWITCH {Operate("PushParseRecord", "breakable"); GenCode("PushContext"); }
expr_switch_cond:
	expr
expr_switch_body:
	expr_switch_body_without_default
	| expr_switch_body_without_default expr_switch_default_delegate '{' stats '}'
expr_switch_default_delegate:
	DEFAULT
expr_switch_body_without_default:
	expr_switch_branch expr_switch_body_without_default
	|
expr_switch_branch:
	expr_switch_branch_cond '{' stats '}' { GenCode("JumpEnd"); Operate("PatchJump");}
expr_switch_branch_cond:
	expr { GenCode("EqCase"); GenCode("Jumpf");}

expr_for:
	expr_for_for_delegate '(' expr_for_init ';' expr_for_cond ';' expr_for_loop ')' expr_for_body
expr_for_for_delegate:
	FOR {GenCode("PushContext"); Operate("PushParseRecord", "breakable");}
expr_for_body:
	expr_common_body {Operate("HangGenCodePaste"); GenCode("JumpToRecordPos"); GenCode("PopContext"); Operate("PatchJumpEnd"); Operate("PopParseRecord");}
expr_for_init:
	expr  {GenCode("ClearStack"); Operate("RecordJumpPos");}
	| {Operate("RecordJumpPos");}
expr_for_cond:
	expr  {GenCode("JumpfEnd"); Operate("HangGenCodeStart");}
	| {Operate("HangGenCodeStart");}
expr_for_loop:
	expr {GenCode("ClearStack"); Operate("HangGenCodeEnd");}
	| {Operate("HangGenCodeEnd");}

expr_if:
	IF {Operate("PushParseRecord", "unbreakable"); GenCode("PushContext");} 
	expr_if_cond  expr_if_body  expr_elseif expr_else {
		Operate("PatchJumpEnd");
		GenCode("PopContext");
		Operate("PopParseRecord");// traverse the record vector
	}
expr_if_cond:
	'(' expr ')' {GenCode("Jumpf");}
expr_elseif:
	ELSEIF { GenCode("PushContext"); } 
	expr_if_cond  expr_if_body  expr_elseif
	| %prec EMPTY
expr_else:
	ELSE expr_common_body
	| %prec EMPTY
expr_if_body:
	expr_common_body {
	 	GenCode("JumpEnd");
		Operate("PatchJump");// record to a vector
	}

expr_while:
	expr_while_delegate expr_while_cond expr_common_body {GenCode("JumpToRecordPos"); Operate("PatchJumpEnd"); GenCode("PopContext"); Operate("PopParseRecord");}
expr_while_delegate:
	WHILE {GenCode("PushContext"); Operate("PushParseRecord", "breakable"); Operate("RecordJumpPos");}
expr_while_cond:
	'(' expr ')' {GenCode("JumpfEnd");}


expr_common_body:
	stat

table:
	'[' {GenCode("GenTable");} table_body ']'
table_body:
	table_expr ',' table_body
	| table_expr
	|
table_expr:
	'(' expr ')' ASSIGN expr {GenCode("InsertIndexStack");}
	| ID ASSIGN expr {GenCode("InsertIndexStr", $1.str);}
	| table {GenCode("InsertIntoStack");}
	| expr_binop_normal {GenCode("InsertIntoStack");}

expr_binop:
	expr_binop_normal
	| expr_assign
expr_binop_normal:
	expr_binop_normal ADD expr_binop_normal	{ GenCode("Add");}
	| expr_binop_normal SUB expr_binop_normal { GenCode("Sub");}
	| expr_binop_normal MUL expr_binop_normal { GenCode("Mul");}
	| expr_binop_normal DIV expr_binop_normal { GenCode("Div");}
	| expr_binop_normal LT expr_binop_normal {GenCode("Lt");}
	| expr_binop_normal GT expr_binop_normal {GenCode("Gt");}
	| expr_binop_normal EQ expr_binop_normal {GenCode("Eq");}
	| expr_binop_normal NE expr_binop_normal {GenCode("Ne");}
	| expr_binop_normal GE expr_binop_normal {GenCode("Ge");}
	| expr_binop_normal LE expr_binop_normal {GenCode("Le");}
	| expr_binop_normal CONCAT expr_binop_normal {GenCode("Concat");}
	| expr_binop_normal AND expr_binop_normal { GenCode("And");}
	| expr_binop_normal OR expr_binop_normal { GenCode("Or");} // 13 warnings here,
	| expr_prefix
expr_assign:
	expr_prefix ASSIGN expr {GenCode("AssignStack");}

expr_prefix:
	'!' expr_postfix {GenCode("Negate");}
	| INC expr_postfix {GenCode("Inc");}
	| DEC expr_postfix {GenCode("Dec");}
	| expr_postfix
expr_postfix:
	expr_postfix '.' ID {GenCode("IndexStr", $3.str);}
	| expr_postfix '.' '(' expr ')' {Operate("IndexStack");}
	| expr_postfix '[' expr ']' {GenCode("IndexStack");}
	| expr_primary
	| expr_postfix_call_delegate '(' expr_param_list ')' {GenCode("Call"); Operate("PopParseRecord");}
	| expr_postfix_class_call_delegate '(' expr_param_list ')' {GenCode("ClassCall"); Operate("PopParseRecord");}
expr_postfix_call_delegate:
	expr_postfix {Operate("PushParseRecord", "unbreakable"); GenCode("StartCallParam", "", 1);}
expr_postfix_class_call_delegate:
    expr_postfix ':' ID {GenCode("ClassIndex", $3.str); GenCode("StartCallParam", "", 2); Operate("PushParseRecord", "unbreakable");}
expr_param_list:
	expr_param ',' expr_param_list
	| expr_param |
expr_param:
	expr
expr_primary:
	TK_NUM { GenCode("PushNum", "", $1.num);}
	| ID {GenCode("Load", $1.str);}
	| TK_STR {GenCode("PushStr", $1.str);}
	| '(' expr ')'

%%

int yyerror(const char *str){
	//if (feof(yyin)) return 1;
 	snprintf(g_buf, GBUF_SIZE,"error:%s:%d:%s\n", "filename", yylineno, str);
 	Parser::self->error_msg = g_buf;
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


