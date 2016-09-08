%{


#define  BIGINT unsigned int
#include <ctype.h>
#include <string>
#include <stack>
using namespace std;

typedef class _YYSTYPE{
public:
	string str;
	double num;
public:
	_YYSTYPE(){
		str = "";
		num = 0;
	}
	void SetNum(const double new_num){
		num = new_num;
#ifdef DEBUG
		printf("get num:%lf\n", new_num);
#endif
	}
	void SetID(const char *id){
		str = id;
#ifdef DEBUG
		printf("get id:%s\n", id);
#endif
	}
	void SetStr(const char *new_str){
		str = new_str;
#ifdef DEBUG
		printf("get str:%s\n", new_str);
#endif
	}
} YYSTYPE; // type for yylval
#define YYSTYPE_IS_DECLARED
#include "yacc.hpp" // define yylval here
#include "../nl/parser.h"

%}

digit		[0-9]
xdigit		[0-9a-fA-F]
odigit		[0-7]
char		[a-zA-Z_]

str 			("\""[^"]*"\"")
num		(-?(0.)?{digit}+)|([1-9]{digit}*(\.{digit}+)?)
unary_op		"++"|"--"
binop		"+"|"-"|"*"|"/"|">"|"<"|"=="|"!="|"and"|"or"|"="|".."|"&&"|"||"|"#"
id			{char}+[a-zA-Z_0-9]*
key			"if"|"else"|"elseif"|"for"|"case"|"in"|"func"|"return"|"while"|"break"|"switch"|"default"

	// define the conditional state for parsing comments
%x mcomment scomment
%option yylineno

%%
	// 此处可以定义局部变量。注意，该定义绝对不能从第一列开始！
	// 而词法符号必须从第一列开始。
	int i ;
	BIGINT val;

	// handle comments
"/*" BEGIN(mcomment);
<mcomment>"*/" BEGIN(INITIAL);
<mcomment>. {}
<mcomment>\n {}
"//" BEGIN(scomment);
<scomment>\n BEGIN(INITIAL);
<scomment>. {}

[ \t]	{}
{num}	{double num; sscanf(yytext,"%lf",&num); yylval.SetNum(num); return(TK_NUM); }
{key} {
	switch(yytext[0]) {
		case 'b': return BREAK;
		case 'd': return DEFAULT;
		case 'i': if (yytext[1]=='f')return IF;else return IN;
		case 'e': if (yytext[4] == '\0')return ELSE; else return ELSEIF;
		case 'f': if (yytext[1] == 'o')return FOR; else return TK_FUNC;
		case 'c': return CASE;
		case 'r': return RETURN;
		case 's': return SWITCH;
		case 'w': return WHILE;
		default: printf("unknown key word:%s", yytext); break;
	}
}
{unary_op} {
	switch(yytext[0]){
		case '+': return INC; break;
		case '-': return DEC; break;
		default: printf("unknown unary operator:%s", yytext); break;
	}
}
{binop}	{
	switch(yytext[0]) {
		case '+': return ADD; break;
		case '-': return SUB; break;
		case '*': return MUL; break;
		case '/': return DIV; break;
		case '>': if (yytext[1]=='\0') return GT; else return GE; break;
		case '<': if (yytext[1]=='\0') return LT; else return LE; break;
		case '=': if (yytext[1]=='\0') return ASSIGN;else return EQ; break;
		case '!': return NE; break;
		case 'a': case '&': return AND; break;
		case 'o': case '|': return OR; break;
		case '.': return CONCAT; break;
		case '#': return SIZEOF; break;
		default: printf("unknown binary operator:%s", yytext); break;
	}
}
{id}	{
	yylval.SetID( yytext );
	return(ID);
}
{str} {
	string str = yytext;
	str.erase(str.begin());
	str.erase(--str.end());// stupid? hehe...
	yylval.SetStr(str.c_str());
	return TK_STR;
}
\n {}
. {return yytext[0];}

%%

