#include "parser.h"

Parser* Parser::self;

Parser::Parser(){
	// save the only instance
	assert(self == NULL);
	self = this;
	cur_func_no = -1;
	hang_code = false;

	// code generation definition
	#define CODE(name) gen_code_handlers.insert(pair<string,GenCodeHandler>(#name, &Parser::name));
	CODE(Jumpf);	CODE(JumpEnd);	CODE(JumpfEnd);
	CODE(JumpToRecordPos);
	CODE(SetTypeFunc);
	CODE(Return);
	CODE(Break);
	CODE(AssignStack);
	CODE(PushContext);
	// FuncEnd
	// Call

	// operations definition
	#define OP(name) op_handlers.insert(pair<string,OpHandler>(#name, &Parser::name))
	//#include "define_op.h"
	OP(PatchJump);
	OP(PatchJumpEnd);
	OP(PushParseRecord);
	OP(PopParseRecord);
	OP(PushFunction);
	OP(PopFunction);
	OP(RecordJumpPos);
	OP(HangGenCodeStart);
	OP(HangGenCodeEnd);
	OP(HangGenCodePaste);
	OP(RecordAssignLeft);
	OP(RecordAssignLeftCount);
}


#define CODE_HANDLER(name) void Parser::name(Code &code, Codes &codes)
#define OP_HANDLER(name) void Parser::name(Codes &codes, ParseContext &pc, string action, Str str1, Num num2)

bool Parser::Parse(string filename){
	if (g_conf.print_parse_op){
		Util::Println("--------------parse start--------------");
	}
	Operate("PushFunction");
	yymain(filename);
	Operate("PopFunction");
	if (g_conf.print_parse_op){
		Util::Println("--------------parse done--------------");
	}
	if (error_msg == ""){
		OutputCodes();
		return true;
	}else{
		Util::Println(error_msg.c_str());
		return false;
	}
}
Codes &Parser::GetAllCodes(){return all_codes;}
bool Parser::GenCode(string action, Str param1, Num param2){
	if (g_conf.print_parse_op){
		Util::Println("CODE: %s%s%s", action.c_str(), (param1 == "" ? "" : (": " + param1).c_str()),
			(param2 == 0 ? "" : (": " + Util::ToString(param2)).c_str()));
	}
	GenCodeHandler handler = gen_code_handlers[action];
	Code code(action, param1, param2);
	Codes* p_codes;
	if (hang_code){
		p_codes = &parse_record.Top().hanged_codes;
	}else{
		p_codes = &funcs.Top().codes;
	}
	Codes& codes = *p_codes;
	if (handler == NULL) {
		codes.push_back(code);
	}else{
		(this->*handler)(code, codes);
		codes.push_back(code);
	}
	return true;
}
void Parser::Operate(string op, Str param1, Num param2){
	if (g_conf.print_parse_op){
		Util::Println("\tOP: %s%s%s", op.c_str(), (param1 == "" ? "" : (": " + param1).c_str()),
			(Util::IsZero(param2) ? "" : (": " + Util::ToString(param2)).c_str()));
	}
	OpHandler handler = op_handlers[op];
	if (handler != NULL){
		(this->*handler)(funcs.Top().codes, parse_record.Top(), op, param1, param2);
		if (g_conf.print_parse_op){
			Util::Println("\tOP <func,parse> <%d,%d>", funcs.Size(), parse_record.Size());
		}
	}else{
		Util::Fatal("unknown op: %s", op.c_str());
	}
}
CODE_HANDLER(Jumpf){
	this->parse_record.Top().jump_list.push(/* index of code jumpf: */codes.size());
}
CODE_HANDLER(PushContext){
	this->parse_record.Top().has_context = true;
}
CODE_HANDLER(AssignStack){
	ParseContext &pc = this->parse_record.Top();
	code.num1 = pc.assign_left_count;
	pc.assign_left_count = 0;
	pc.assign_left_counter = 0;
}
CODE_HANDLER(Break){
	int count_context = 0;
	code.action = "Jump";
	Stack<ParseContext> &pcs = this->parse_record;
	for (int i = pcs.Size() - 1; i >= 0; --i){
		ParseContext &pc = pcs[i];
		if (pc.breakable){
			for (int j = 0; j < count_context; ++j){
				this->GenCode("PopContext");// pop some contexts when break
			}
			pc.jump_end_list.push_back(codes.size());
			return;
		}else if (pc.has_context){
			++count_context;
		}
	}
	Util::Fatal("no breakable context");
}
CODE_HANDLER(JumpToRecordPos){
	code.action = "Jump";
	code.num1 = this->parse_record.Top().record_pos;
}
CODE_HANDLER(JumpEnd){
	code.action = "Jump";
	this->parse_record.Top().jump_end_list.push_back(/* index of code jump_end: */codes.size());
}
CODE_HANDLER(JumpfEnd){
	code.action = "Jumpf";
	this->parse_record.Top().jump_end_list.push_back(/* index of code jumpf_end: */codes.size());
}
CODE_HANDLER(SetTypeFunc){
	code.num1 = this->cur_func_no + 1;
}
CODE_HANDLER(Return){
	code.num1 = this->cur_func_no;
}
// operation handlers
OP_HANDLER(PatchJump){
	if(!(pc.jump_list.size() > 0 && codes.size() > (unsigned int)pc.jump_list.top())){
		Util::Fatal("jump_list.size=%d\tcodes.size==%d", pc.jump_list.size(), codes.size());
	}
	Code &jumpf = codes[pc.jump_list.top()];
	jumpf.PatchJumpOffset(codes.size());
	pc.jump_list.pop();
}
OP_HANDLER(PatchJumpEnd){
	for (int i_jump = 0; (unsigned)i_jump < pc.jump_end_list.size(); ++i_jump){
		Code &code_jump_end = codes[pc.jump_end_list[i_jump]];
		code_jump_end.PatchJumpOffset(codes.size()); // jump to next code of current codes
	}
}
OP_HANDLER(RecordAssignLeftCount){
	this->parse_record.Top().assign_left_count = this->parse_record.Top().assign_left_counter;
}
OP_HANDLER(RecordJumpPos){
	this->parse_record.Top().record_pos = codes.size();
}
OP_HANDLER(RecordAssignLeft){
	this->parse_record.Top().assign_left_counter += 1;
}
OP_HANDLER(HangGenCodeStart){
	this->hang_code = true;
}
OP_HANDLER(HangGenCodeEnd){
	this->hang_code = false;
}
OP_HANDLER(HangGenCodePaste){
	Codes &hanged_codes = this->parse_record.Top().hanged_codes;
	int code_count = (int)hanged_codes.size();
	for (int i = 0; i < code_count; ++i){
		codes.push_back(hanged_codes[i]);
	}
	hanged_codes.clear();
}
OP_HANDLER(PushParseRecord){
	if (str1 == ""){
		Util::Fatal("parser: first parameter for PushParseRecord cannot be empty, but breakable or unbreakable");
	}
	this->parse_record.Push();
	this->parse_record.Top().breakable = (str1 == "breakable");
}
OP_HANDLER(PopParseRecord){
	this->parse_record.Pop();
}
OP_HANDLER(PushFunction){
	this->funcs.Push();
	this->funcs.Top().no = ++this->cur_func_no;
	this->Operate("PushParseRecord", "unbreakable");
}
OP_HANDLER(PopFunction){
	this->funcs_parsed.Push(this->funcs.Top());
	this->funcs.Pop();
	this->Operate("PopParseRecord");
}
void Parser::ShowCodes(){
	for (vector<FuncInfo>::iterator i_func = funcs.begin(); i_func != funcs.end(); ++i_func){
		FuncInfo& f = *i_func;
		for (int i_code = 0; (unsigned)i_code < f.codes.size(); ++i_code){
			Code& code = f.codes[i_code];
			Util::Println("%d\t%s\t%s\t%lf",i_code, code.action.c_str(), code.str1.c_str(), code.num1);
		}
	}
}
void Parser::PrintCodes(Codes &codes){
	Util::Println("~==========================");
	for (int i_code = 0; (unsigned)i_code < codes.size(); ++i_code){
		Code& code = codes[i_code];
		Util::Println("-%d\t%s\t%s\t%lf", i_code, code.action.c_str(), code.str1.c_str(), code.num1);
	}
	Util::Println("~==========================");
}
void Parser::OutputCodes(){
	func_code_infos.clear();
	all_codes.clear();
	int base_ip = 0;
	func_code_infos.resize(funcs_parsed.Size());
	// arrage codes of different functions
	while (funcs_parsed.Size() > 0){
		FuncInfo &f = funcs_parsed.Top();
		func_code_infos[f.no].base_ip = base_ip;
		func_code_infos[f.no].count = f.codes.size();

		// patch jump address
		for (int i_code = 0; (unsigned)i_code < f.codes.size(); ++i_code){
			Code& code = f.codes[i_code];
			if (strncmp(code.action.c_str(), "Jump", 4) == 0){
				code.num1 += base_ip;
			}
		}
		all_codes.insert(all_codes.end(), f.codes.begin(), f.codes.end());// copy the codes

		funcs_parsed.Pop();
		base_ip += f.codes.size();
	}
	// patch SetTypeFunc code address
	for (int i_code = 0; (unsigned)i_code < all_codes.size(); ++i_code){
		Code& code = all_codes[i_code];
		if (code.action == "SetTypeFunc"){
			code.num1 = func_code_infos[(int)code.num1].base_ip;
		} else if (code.action == "Return"){
			code.action = "Jump";// Return实际上就是无条件跳转嘛。跳到函数的最后一条指令。
			FuncCodeInfo &func = func_code_infos[(int)code.num1];
			code.num1 = func.base_ip + func.count - 1;
		}
	}
	if (g_conf.print_generated_code){
		PrintCodes(all_codes);
		exit(-1);
	}
}
