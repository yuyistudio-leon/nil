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

%token		TK_NUM ID TK_STR IF ELSE ELSEIF FOR CASE IN TK_FUNC RETURN WHILE BREAK SWITCH DEFAULT


%nonassoc EMPTY
%nonassoc ELSE
%nonassoc ELSEIF
%nonassoc EXPR_FOR_INIT // todo

%nonassoc ID

%left		ASSIGN
%left		AND OR
%left		EQ NE LT GT GE LE

%nonassoc EXPR_BINOP_TO_EXPR; // 优先级比sub低！表示减法符号优先级比负号低。

%left		ADD SUB
%left		MUL DIV
%left		CONCAT
%right		INC DEC SIZEOF '!'

%nonassoc ','
%nonassoc POSTFIX_TO_PREFIX  // ! func ( param )  应该解释成  !(func(param))  而不是  (!func)(param)
%nonassoc '(' '[' '.' ':'

%%

program:
	stats {YCode("EOP");}

stats:
    stats stat
    | stat

stat:
	stat_clear_stack {YCode("ClearStack");}
	| expr_if | expr_for | expr_while | BREAK ';' {YCode("Break");}
	| '{' stats '}'
	| '{' '}'
	| stat_return
	| ';'
stat_clear_stack:
	expr_func
	| stat_assign
	| expr_switch // clear stack for 'case' expr
	| expr ';'

stat_return:
	RETURN expr_list ';' { YCode("Return");}

stat_assign:
	expr_not_empty_list ASSIGN {YOp("RecordAssignLeftCount");} expr_not_empty_list ';' {YCode("AssignStack");}

expr:
	expr_binop %prec EXPR_BINOP_TO_EXPR
	| table
expr_list:
	expr_not_empty_list
	|
expr_not_empty_list:
	expr {YOp("RecordAssignLeft");}
	| expr_not_empty_list ',' expr {YOp("RecordAssignLeft");}
	;
expr_func:
	expr_func_delegate expr_func_name_delegate '(' expr_func_param_list_delegate ')' '{' expr_func_body '}' {YCode("FuncEnd"); YOp("PopFunction");}
expr_func_delegate:
	TK_FUNC
expr_func_name_delegate:
	expr_func_name {YCode("SetTypeFunc"); YOp("PushFunction");}
expr_func_name:
	expr_func_name '.'  ID {YCode("IndexStr", $3.str);}
	| ID {YCode("Load", $1.str);}
	//| {YOp("FuncWithNoName");}
expr_func_param_list_delegate:
	expr_func_param_list
expr_func_param_list:
	expr_func_param_list ',' expr_func_param
	| expr_func_param
	|
expr_func_body:
	stats
expr_func_param:
	ID {YCode("LoadParam", $1.str);}

expr_switch:
	expr_switch_delegate expr_switch_cond '{' expr_switch_body '}' {YOp("PatchJumpEnd"); YCode("PopContext"); YOp("PopParseRecord");}
expr_switch_delegate:
	SWITCH {YOp("PushParseRecord", "breakable"); YCode("PushContext"); }
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
	expr_switch_branch_cond '{' stats '}' { YCode("JumpEnd"); YOp("PatchJump");}
expr_switch_branch_cond:
	expr { YCode("EqCase"); YCode("Jumpf");}

expr_for:
	FOR {YOp("PushParseRecord", "breakable"); YCode("PushContext");}
	'(' expr_for_init expr_for_cond ';' expr_for_loop ')'
	stat {YOp("HangGenCodePaste"); YCode("JumpToRecordPos"); YOp("PatchJumpEnd"); YCode("PopContext"); YOp("PopParseRecord");}
expr_for_init:
	stat  {YOp("RecordJumpPos");} %prec EXPR_FOR_INIT
expr_for_cond:
	expr  {YCode("JumpfEnd"); YOp("HangGenCodeStart");}
	| {YOp("HangGenCodeStart");} %prec EMPTY
expr_for_loop:
	expr {YCode("ClearStack"); YOp("HangGenCodeEnd");}
	| {YOp("HangGenCodeEnd");}

expr_if:
	IF {YOp("PushParseRecord", "unbreakable"); YCode("PushContext");}
	expr_if_cond  expr_if_body  expr_elseif_list expr_else {
			YOp("PatchJumpEnd");
			YCode("PopContext");
			YOp("PopParseRecord");// traverse the record vector
		}
expr_if_cond:
	expr {YCode("Jumpf");}
expr_elseif_list:
	expr_elseif_list expr_elseif
	| %prec EMPTY
expr_elseif:
	ELSEIF expr_if_cond expr_if_body
expr_else:
	ELSE stat
	| %prec EMPTY
expr_if_body:
	stat {
	 	YCode("JumpEnd");
		YOp("PatchJump");// record to a vector
	}

expr_while:
	expr_while_delegate expr_while_cond stat {YCode("JumpToRecordPos"); YOp("PatchJumpEnd"); YCode("PopContext"); YOp("PopParseRecord");}
expr_while_delegate:
	WHILE {YOp("PushParseRecord", "breakable"); YCode("PushContext"); YOp("RecordJumpPos");}
expr_while_cond:
	'(' expr ')' {YCode("JumpfEnd");}

table:
	'[' {YOp("PushParseRecord", "unbreakable"); YCode("GenTable");}
	table_body ']' {YOp("PopParseRecord");}
	| '[' ']' {YCode("GenTable");}
table_body:
	table_body ',' table_expr
	| table_expr
table_expr:
	expr ASSIGN expr {YCode("InsertIndexStack");}
	| ID ASSIGN expr {YCode("InsertIndexStr", $1.str);}
	| expr {YCode("InsertIntoStack");}

expr_binop:
	expr_binop ADD expr_binop	{ YCode("Add");}
	| expr_binop SUB expr_binop { YCode("Sub");}
	| expr_binop MUL expr_binop { YCode("Mul");}
	| expr_binop DIV expr_binop { YCode("Div");}
	| expr_binop LT expr_binop {YCode("Lt");}
	| expr_binop GT expr_binop {YCode("Gt");}
	| expr_binop EQ expr_binop {YCode("Eq");}
	| expr_binop NE expr_binop {YCode("Ne");}
	| expr_binop GE expr_binop {YCode("Ge");}
	| expr_binop LE expr_binop {YCode("Le");}
	| expr_binop CONCAT expr_binop {YCode("Concat");}
	| expr_binop AND expr_binop { YCode("And");}
	| expr_binop OR expr_binop { YCode("Or");} // 13 warnings here,
	| expr_prefix
expr_prefix:
	'!' expr_postfix {YCode("Not");}
	| SIZEOF expr_postfix {YCode("Sizeof");}
	| INC expr_postfix {YCode("Inc");}
	| DEC expr_postfix {YCode("Dec");}
	| SUB expr_postfix {YCode("Negate");}
	| expr_postfix %prec POSTFIX_TO_PREFIX
expr_postfix:
	expr_postfix '.' ID {YCode("IndexStr", $3.str);}
	| expr_postfix '[' expr ']' {YCode("IndexStack");}
	| expr_postfix '(' {YOp("PushParseRecord", "unbreakable"); YCode("StartCallParam", "", 1);}
		expr_list ')' {YCode("Call"); YOp("PopParseRecord");}
	| expr_postfix ':' ID {YCode("ClassIndex", $3.str); YCode("StartCallParam", "", 2); YOp("PushParseRecord", "unbreakable");}
		'(' expr_list ')' {YCode("ClassCall"); YOp("PopParseRecord");}
	| expr_primary
expr_primary:
	TK_NUM { YCode("PushNum", "", $1.num);}
	| ID {YCode("Load", $1.str);}
	| TK_STR {YCode("PushStr", $1.str);}
	| '(' expr ')'

%%

int yyerror(const char *str){
	//if (feof(yyin)) return 1;
 	Util::Println("error:%s:%d:%s:expect %s before %s\n", "filename", yylineno, str, "?", yytext);
 	Parser::self->error_msg = "error";
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


