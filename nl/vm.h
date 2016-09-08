#ifndef _VM_HEADER_
#define _VM_HEADER_

#include "define.h"
#include "util.h"
#include "gc.h"
#include "value.h"
#include "code.h"
#include "parser.h"
#include "lib.h"


class VarStack{
private:
	Stack<PValue> st;// stack
	set<PValue> tmp_vars;
    void __PushTempVar(PValue temp);
public:
	VarStack();
	~VarStack();
    void PushNil();
    void PushStr(string str);
    void PushNum(Num num);
    void Push(PValue pv);
    PValue& RIndex(int offset);
    PValue& Index(int index);
    void Pop(int count = 1);
    void CollectTempVar(PValue temp);
    PValue CreateTempVar(PValue src = NULL);
    void CollectUselessTempVars();
    size_t Size();
    PValue& Top();
    PValue Set(int index, PValue new_value);
    PValue SetRIndex(int reverse_index, PValue new_value);
    void ClearStack();
    void ClearTail(int n);
    void Print();
};

#define CONTEXT_KEY ("__context__")
class ContextStack{
	PValue context;
	int cur_index;
public:
	size_t Size();
	void SetG(PValue G);
	ContextStack();
	void Push(PValue context = NULL);
	void Add(string var_name, PValue pv);
	void Add(int index, PValue pv);
	PValue GetCascade(string var_name);
	PValue GetArgs();
	PValue Create(string var_name);
	void EraseCurrent(string var_name);
	void Pop();
};

class VM : public Value{
	typedef struct{
		int ip;
		int stack_size;
		int param_count;
		int param_index;
		vector<PValue> return_values;
	} CallInfo;
	Stack<CallInfo> call_stack;
public:
    ~VM();
	// VMs manager
    static VM* GetVM(string name = "main");
    static void ReleaseVM(string name = "main");
    // VM state API
    void ShowAllVars();
    // VM control API
    void CollectGarbage();
	void RunCode(Codes &codes);
	// Runtime functions
	void LoadVariable(string var_name);
private:
	// VM state
	int ip;// abbr for Instruction Pointer
	Value G;
	VarStack var_stack;
	Codes* running_codes;
	ContextStack context;// when called, paramsters are stored here
    static map<string,VM*> vms;
private:
    VM();
	// init environment before running the code
	void InitRunEnv();
	// run the codes
	void ExecuteCodes(Codes &codes);
};


#endif
