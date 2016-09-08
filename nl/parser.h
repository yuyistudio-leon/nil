#ifndef _PARSER_HEADER_
#define _PARSER_HEADER_

#include "define.h"
#include "stack.h"
#include "util.h"
#include "code.h"
using namespace std;

#define CODE_HANDLER_DECLARE(name) void name(Code &code, Codes &codes)
#define OP_HANDLER_DECLARE(name) void name(Codes &codes, ParseContext &pc, string action, Str str1, Num num2)

int yymain(string);

class Parser{
private:
	struct ParseContext{
		stack<int> jump_list;
		vector<int> jump_end_list;
		int record_pos;
		Codes hanged_codes;
		bool breakable;
		int assign_left_counter;
		int assign_left_count;
		int table_index;
		bool has_context;
		ParseContext(){
			has_context = false;
			table_index = 0;
			record_pos = 0;
			breakable = false;
			assign_left_counter = 0;
			assign_left_count = 0;
		}
	};
	struct FuncInfo{
		int no;
		Codes codes;
	};
	typedef struct{
		int base_ip;
		int count;
	} FuncCodeInfo;
	typedef void (Parser:: *GenCodeHandler)(Code&, Codes&);
	typedef void (Parser:: *OpHandler)(Codes&, ParseContext&, string, Str, Num);
private:
	map<string,GenCodeHandler> gen_code_handlers;
	map<string,OpHandler> op_handlers;
	Stack<FuncInfo> funcs;
	Stack<FuncInfo> funcs_parsed;
	Stack<ParseContext> parse_record;
	Codes all_codes;// flatten codes
	int cur_func_no;
	bool hang_code;
	vector<FuncCodeInfo> func_code_infos;
public:
	string error_msg;
    static Parser* self;
public:
    bool Parse(string filename);
    Codes &GetAllCodes();
	bool GenCode(string action, Str param1 = "", Num param2 = 0);
	void Operate(string op, Str param1 = "", Num param2 = 0);
	Parser();
private:
	// code generators
	CODE_HANDLER_DECLARE(Jumpf);
	CODE_HANDLER_DECLARE(PushContext);
	CODE_HANDLER_DECLARE(AssignStack);
	CODE_HANDLER_DECLARE(Break);
	CODE_HANDLER_DECLARE(JumpToRecordPos);
	CODE_HANDLER_DECLARE(JumpEnd);
	CODE_HANDLER_DECLARE(JumpfEnd);
	CODE_HANDLER_DECLARE(SetTypeFunc);
	CODE_HANDLER_DECLARE(Return);
	// operation handlers
	OP_HANDLER_DECLARE(PatchJump);
	OP_HANDLER_DECLARE(PatchJumpEnd);
	OP_HANDLER_DECLARE(RecordAssignLeftCount);
	OP_HANDLER_DECLARE(RecordJumpPos);
	OP_HANDLER_DECLARE(RecordAssignLeft);
	OP_HANDLER_DECLARE(HangGenCodeStart);
	OP_HANDLER_DECLARE(HangGenCodeEnd);
	OP_HANDLER_DECLARE(HangGenCodePaste);
	OP_HANDLER_DECLARE(PushParseRecord);
	OP_HANDLER_DECLARE(PopParseRecord);
	OP_HANDLER_DECLARE(PushFunction);
	OP_HANDLER_DECLARE(PopFunction);
private:
	void ShowCodes();
	void PrintCodes(Codes &codes);
	void OutputCodes();
};

#define YCode Parser::self->GenCode
#define YOp Parser::self->Operate

#endif
