#include "vm.h"

map<string,VM*> VM::vms;

VM* VM::GetVM(string name){
    VM* pvm = vms.find(name)->second;
    if (pvm == NULL){
        pvm = new VM();
    }
    return pvm;
}
void VM::ReleaseVM(string name){
    VM* pvm = vms.find(name)->second;
    if (pvm != NULL){
        vms.erase(name);
        delete pvm;
    }
}

void VarStack::__PushTempVar(PValue temp){
    tmp_vars.insert(temp);
}
VarStack::VarStack(){
}
VarStack::~VarStack(){

}
void VarStack::PushNil(){
    st.Push(CreateTempVar());
}
void VarStack::PushStr(string str){
    st.Push(CreateTempVar());
    st.Top()->Set(str);
}
void VarStack::PushNum(Num num){
    st.Push(CreateTempVar());
    st.Top()->Set(num);
}
void VarStack::Push(PValue pv){
    assert(pv != NULL);
    st.Push(pv);
}
PValue& VarStack::RIndex(int offset){
    int index = st.Size() - 1 - offset;
    assert(index >= 0 && (unsigned)index < st.Size());
    return st[index];
}
PValue& VarStack::Index(int index){
    assert(index >= 0 && (unsigned)index < st.Size());
    return st[index];
}
void VarStack::Pop(int count){
    for (int i = 0; i < count; ++i){
        if (st.Top()->IsTemp()){
            __PushTempVar(st.Top());
        }
        st.Pop();
    }// stupid way? haha...
}
void VarStack::CollectTempVar(PValue temp){
    delete temp; // todo
}
PValue VarStack::CreateTempVar(PValue src){
    PValue tmp = new Value();
    if (src != NULL){
        tmp->Set(src);
    }
    return tmp;
}
void VarStack::CollectUselessTempVars(){
    for (set<PValue>::iterator i_var = tmp_vars.begin(); i_var != tmp_vars.end(); ++i_var){
        CollectTempVar(*i_var);
    }
    tmp_vars.clear();
}
size_t VarStack::Size(){
    return st.Size();
}
PValue& VarStack::Top(){
    return st.Top();
}
PValue VarStack::Set(int index, PValue new_value){
    return st.Set(index, new_value);
}
PValue VarStack::SetRIndex(int reverse_index, PValue new_value){
    return st.Set(Size() - reverse_index - 1, new_value);
}
void VarStack::ClearStack(){
    st.ClearStack();
}
void VarStack::ClearTail(int n){
    st.ClearTail(n);
}
void VarStack::Print(){
    Util::Println("/---------var-stack-------------");
    for (int i = 0; i < (int)st.Size(); ++i){
        PValue pv = st[i];
        printf("|%d\t%s", i, pv->ToString().c_str());
        if (pv->IsTemp()){
            printf("\ttemp<%p>", pv);
        }
        Util::Println();
    }
    Util::Println("\\---------var-stack-------------");
}

size_t ContextStack::Size(){
    return cur_index + 1;
}
void ContextStack::SetG(PValue G){
    assert(G);
    this->context = G->Get(CONTEXT_KEY);
    PValue global_context = context->Get(0);
    global_context->Set("print", Print);
    global_context->Set("write", Write);
    global_context->Set("setclass", SetMetaHash);
    global_context->Set("exit", Exit);
    if (g_conf.print_parse_op){
        Util::Println("------------ lib loading -----------");
        global_context->DebugPrint();
        Util::Println("------------ lib loaded -----------");
    }
}
ContextStack::ContextStack(){cur_index = -1;}
void ContextStack::Push(PValue context){
    ++cur_index;
    if (context != NULL){
        this->context->Set(cur_index, context);
    }
}
void ContextStack::Add(string var_name, PValue pv){
    assert(pv != NULL);
    context->Get(cur_index)->Set(var_name, pv);
}
void ContextStack::Add(int index, PValue pv){
    Add(Util::ToString(index), pv);
}
PValue ContextStack::GetCascade(string var_name){
    // print global context
    // context->Get(0)->DebugPrint();
    int index = cur_index;
    PValue init_pv = context->Get(index--)->Get(var_name);
    PValue pv = init_pv;
    while(pv->IsNil() && index >= 0){
        pv = context->Get(index--)->Get(var_name);
    }
    if (pv->IsNil()){
        return init_pv;
    }else{
        return pv;
    }
}
PValue ContextStack::GetArgs(){
    return context->Get(cur_index - 1)->Get("args");
}
PValue ContextStack::Create(string var_name){
    PValue p_context = context->Get(cur_index);
    p_context->Erase(var_name);
    return p_context->Get(var_name);
}
void ContextStack::EraseCurrent(string var_name){
    PValue pv = context->Get(cur_index)->Get(var_name);
    if (!pv->IsNil()){
        context->Get(cur_index)->Erase(var_name);
    }
}
void ContextStack::Pop(){
    context->Erase(Util::ToString(cur_index--));
}


// VMs manager
VM::VM(){running_codes = NULL; gc = new GC(); G.SetGC(gc); context.SetG(&G); ip = 0;}
VM::~VM(){delete gc;}
// VM state API
void VM::ShowAllVars(){gc->ShowAllVars(this);}
// VM control API
void VM::CollectGarbage(){gc->CollectGarbage(this);}
void VM::RunCode(Codes &codes){
    if (g_conf.print_running_code){
        Util::Println("---------------vm running--------------");
    }
    InitRunEnv();
    ExecuteCodes(codes);
}
void VM::LoadVariable(string var_name){
    PValue pv = context.GetCascade(var_name);
    var_stack.Push(pv);
}
void VM::InitRunEnv(){
    // init context
    context.Push();
    // init arguments
    int arg_count = 10;
    PValue args = context.Create("args");
    for (int i = 0; i < arg_count; ++i){
        args->Set(i, string("p") + Util::ToString(i));
    }
    // init call stack
    CallInfo &call_info = call_stack.Push();
    call_info.ip = 0;
    call_info.param_count = arg_count;
    call_info.param_index = 10;
    call_info.stack_size = 0;
}
void VM::ExecuteCodes(Codes &codes){
    running_codes = &codes;
    for (ip = 0; ip < (int)codes.size();){
        Code &code = codes[ip];
        if (g_conf.print_running_code){
            Util::Println("\t+%d\t%s\t%s\t%lf", ip, code.action.c_str(), code.str1.c_str(), code.num1);
        }
        // 解析Instruction
        #define EXE_DONE goto EXE_DONE_JUMP_ANCHOR
        #define	EXE_BEGIN(action_name) if (code.action == #action_name){
        #define EXE_CODE(action_name) }else if (code.action == #action_name){
        #define EXE_DEFAULT(action_name) }else if (code.action == #action_name){this->action_name();
        #define EXE_BINARY_OP(action_name) }else if (code.action == #action_name){ \
            PValue pv_left = var_stack.RIndex(1); \
            PValue pv_right = var_stack.RIndex(0); \
            PValue result = pv_left; \
            if (!result->IsTemp()){ \
                result = var_stack.CreateTempVar(); \
                var_stack.SetRIndex(1, result); \
            }
        #define EXE_STACK_TOP_OP(action_name) }else if (code.action == #action_name){ \
            PValue pv_left = var_stack.RIndex(1); \
            PValue pv_right = var_stack.RIndex(0);
        #define EXE_UNKNOWN }else{
        #define EXE_END }
        EXE_BEGIN(Load) LoadVariable(code.str1);
        EXE_CODE(PushStr) var_stack.PushStr(code.str1);
        EXE_CODE(PushNum) var_stack.PushNum(code.num1);
        EXE_CODE(PushContext) context.Push();
        EXE_CODE(PopContext) context.Pop();
        EXE_CODE(Jumpf)
            if (!var_stack.Top()->IsTrue()){
                ip = (int)code.num1;
            }else{
                ++ip;
            }
            var_stack.Pop();
            continue;
        EXE_BINARY_OP(Add)
            result->Set(pv_left->ToNum() + pv_right->ToNum());
            var_stack.Pop();
        EXE_BINARY_OP(Sub)
            result->Set(pv_left->ToNum() - pv_right->ToNum());
            var_stack.Pop();
        EXE_BINARY_OP(Mul)
            result->Set(pv_left->ToNum() * pv_right->ToNum());
            var_stack.Pop();
        EXE_BINARY_OP(Div)
            if (Util::IsZero(pv_right->ToNum())){
                Util::Fatal("div by zero");
            }
            result->Set(pv_left->ToNum() / pv_right->ToNum());
            var_stack.Pop();
        EXE_BINARY_OP(Lt)
            result->Set(Value::Lt(pv_left, pv_right));
            var_stack.Pop();
        EXE_BINARY_OP(Gt)
            result->Set(Value::Gt(pv_left, pv_right));
            var_stack.Pop();
        EXE_BINARY_OP(Eq)
            result->Set(Value::Eq(pv_left, pv_right));
            var_stack.Pop();
        EXE_STACK_TOP_OP(EqCase)
            PValue result = pv_right;
            if (!result->IsTemp()){
                result = var_stack.CreateTempVar();
                var_stack.SetRIndex(1, result);
            }
            result->Set(Value::Eq(pv_left, pv_right));
        EXE_BINARY_OP(Ne)
            result->Set(Value::Ne(pv_left, pv_right));
            var_stack.Pop();
        EXE_BINARY_OP(Ge)
            result->Set(Value::Ge(pv_left, pv_right));
            var_stack.Pop();
        EXE_BINARY_OP(Le)
            result->Set(Value::Le(pv_left, pv_right));
            var_stack.Pop();
        EXE_BINARY_OP(Concat)
            result->Set(Value::Concat(pv_left, pv_right));
            var_stack.Pop();

        EXE_STACK_TOP_OP(And)
            PValue result = pv_left;
            if (pv_left->IsTrue() && pv_right->IsTrue()){
                result = pv_right;
            }
            var_stack.Pop(2);
            var_stack.Push(result);
        EXE_STACK_TOP_OP(Or)
            PValue result = pv_left;
            if (!pv_left->IsTrue()){
                result = pv_right;
            }
            var_stack.Pop(2);
            var_stack.Push(result);

        EXE_STACK_TOP_OP(IndexStack)
            assert(!pv_left->IsTemp());
            pv_left = pv_left->Get(pv_right);
            var_stack.Pop(2);
            var_stack.Push(pv_left);
        // 生成table的操作
        EXE_CODE(GenTable)
            var_stack.Push(context.Create("__table__tmp__"));
        EXE_STACK_TOP_OP(InsertIndexStack)
            PValue p_tab = var_stack.RIndex(2);
            p_tab->InsertIndex(pv_left, pv_right);
            var_stack.Pop(2);
        EXE_STACK_TOP_OP(InsertIndexStr)
            Value v;
            v.Set(code.str1);
            pv_left->InsertIndex(&v, pv_right);
            var_stack.Pop();
        EXE_STACK_TOP_OP(InsertIntoStack)
            Value v;
            v.Set((Num)pv_left->Size());
            pv_left->InsertIndex(&v, pv_right);
            var_stack.Pop();
        EXE_CODE(AssignStack)
            CallInfo &call_info = call_stack.Top();
            int left_var_count = (int)code.num1;
            int right_var_count = var_stack.Size() - call_info.stack_size - left_var_count;
            int left_base = var_stack.Size() - right_var_count - left_var_count;
            int right_base = var_stack.Size() - right_var_count;
            if (g_conf.pause_after_instruction){
                Util::Debug("%d:%d %d:%d", left_base, right_base, left_var_count, right_var_count);
            }
            // 将等号右边的值全部替换成临时变量，免得赋值过程中左侧值改变影响右侧的值。例如a,b=b,a，第一步a=b，第二步b=a，如果直接赋值的话最终的结果就是 a和b都变成了原来b的值。
            for (int i = 0; i < right_var_count; ++i){
                PValue& p_right = var_stack.Index(right_base + i);
                if (!p_right->IsTemp()){
                    p_right = var_stack.CreateTempVar(p_right);
                }
            }
            for (int i = 0; i < right_var_count; ++i){
                PValue p_right = var_stack.Index(right_base + i);
                PValue p_left = var_stack.Index(left_base + i);
                p_left->Set(p_right);
            }
            var_stack.Pop(right_var_count);
        EXE_CODE(StartCallParam)// code.num1=1表示普通函数调用，此时函数占据了栈上一个位置i；=2表示类函数调用，此时类实例和函数占据栈上两个位置。
            CallInfo &call_info = call_stack.Push();
            call_info.stack_size = var_stack.Size() - (int)code.num1;// 减去堆栈上函数占据的位置
        EXE_CODE(ClassIndex)
            var_stack.Push(var_stack.RIndex(0)->Get(code.str1));
        EXE_CODE(ClassCall)
            CallInfo &call_info = call_stack.Top();
            // 直到这里才知道究竟有几个参数
            call_info.param_count = var_stack.Size() - call_info.stack_size - 1;
            // 被调用函数来载入参数用的，用来记录解析到第几个参数了
            call_info.param_index = 0;
            // 获取第一个参数。第一个参数在函数声明中是第一个参数，按照python的习惯，叫做self
            PValue self = var_stack.RIndex(call_info.param_count);// index(param_count)是要调用的函数
            // 将参数按顺序依次保存到当前函数的args表中。函数调用完成后清理。
            PValue args = context.Create("args");
            args->Set(0, self);
            for (int i = 1; i < call_info.param_count; ++i){
                PValue pv = var_stack.RIndex(call_info.param_count - i - 1);
                args->Set(i, pv);
            }
            // 获取函数
            PValue func = var_stack.RIndex(call_info.param_count - 1);
            // 清理堆栈上的函数和参数
            if (g_conf.print_call_stack){
                var_stack.Print();
            }
            var_stack.Pop(call_info.param_count + 1);// pops the params and the function and the class object
            if (g_conf.print_call_stack){
                var_stack.Print();
            }
            // 调用函数
            if (func->IsCFunc()){
                context.Push();
                func->Call(this, args);
                context.Pop();
                context.EraseCurrent("args");
                call_stack.Pop();
            }else if(func->IsFunc()){
                context.Push();
                call_info.ip = ip;
                ip = func->num;
                continue;
            }else{
                // Util::Fatal("error: call non-func value: %s", func->TypeStr().c_str());

                // or, handle like this...
                // the 'func` remained there as the return-value
                var_stack.Push(func);
                call_stack.Pop();
            }
        EXE_CODE(Call)
            CallInfo &call_info = call_stack.Top();
            // 直到这里才知道究竟有几个参数
            call_info.param_count = var_stack.Size() - call_info.stack_size - 1;
            // 被调用函数来载入参数用的，用来记录解析到第几个参数了
            call_info.param_index = 0;
            // 将参数按顺序依次保存到当前函数的args表中。函数调用完成后清理。
            PValue args = context.Create("args");
            for (int i = 0; i < call_info.param_count; ++i){
                PValue pv = var_stack.RIndex(call_info.param_count - i - 1);
                args->Set(i, pv);
            }
            // 获取函数
            PValue func = var_stack.RIndex(call_info.param_count);
            // 清理堆栈上的函数和参数
            if (g_conf.print_call_stack){
                Util::Println("-- before call --");
                var_stack.Print();
            }
            var_stack.Pop(call_info.param_count + 1);// pops the params and the function
            if (g_conf.print_call_stack){
                Util::Println("-- before call, stack cleared --");
                var_stack.Print();
            }
            // 调用函数
            if (func->IsCFunc()){
                context.Push();
                func->Call(this, args);
                context.Pop();
                context.EraseCurrent("args");
                call_stack.Pop();
            }else if(func->IsFunc()){
                context.Push();
                call_info.ip = ip;
                ip = func->num;
                continue;
            }else{
                // Util::Fatal("error: call non-func value");

                // or, handle like this...
                // the 'func` remained there as the return-value
                var_stack.Push(func);
                call_stack.Pop();
            }
        EXE_CODE(Jump)
            ip = (int)code.num1;
            continue;
        EXE_CODE(LoadParam)
            CallInfo &call_info = call_stack.Top();
            PValue args = context.GetArgs();
            if (call_info.param_index < call_info.param_count){
                int cur_index = call_info.param_index++;
                PValue param = args->Get(cur_index);
                if (g_conf.print_call_stack){
                    Util::Println("LoadParam<%s:%s>", code.str1.c_str(), param->ToString().c_str());
                }
                context.Add(code.str1, param);
            }
        EXE_CODE(Negate)
            var_stack.Top()->Negate();
        EXE_CODE(Not)
            var_stack.Top()->Not();
        EXE_CODE(Inc)
            var_stack.Top()->Inc();
        EXE_CODE(Sizeof)
            size_t size = var_stack.Top()->Size();
            var_stack.Pop();
            var_stack.PushNum(size);
        EXE_CODE(Dec)
            var_stack.Top()->Dec();
        EXE_CODE(SetTypeFunc)
            var_stack.Top()->SetFunc(code.num1);
        EXE_CODE(FuncEnd)
            CallInfo &call_info = call_stack.Top();
            ip = call_info.ip + 1;// 函数调用完了，继续执行下一条语句
            context.Pop();
            call_stack.Pop();
            if (g_conf.print_call_stack){
                Util::Println("-- after call --");
                var_stack.Print();
            }
            continue;
        EXE_CODE(ClearStack)
            CallInfo &call_info = call_stack.Top();
            int clear_count = var_stack.Size() - call_info.stack_size;
            if (clear_count < 0){
                Util::Fatal("var stack abnormal: var_stack: %d; call_stack: %d", var_stack.Size(), call_info.stack_size);
            }
            var_stack.ClearTail(clear_count);
            var_stack.CollectUselessTempVars();
        EXE_CODE(EOP) Util::Println("[Success]"); break;
        EXE_CODE(IndexStr)
            PValue pv = var_stack.Top();
            if (pv->IsTemp()){
                Util::Fatal("attempt to index temporary var");
            }else{
                var_stack.Pop();
                var_stack.Push(pv->Get(code.str1));
            }
        /*
        EXE_CODE(LoadParam)
            context.Add(code.str1, var_stack.RIndex( (int)code.num1 ));
        */
        EXE_UNKNOWN
            Util::Fatal("unknown instruction: %s", code.action.c_str());
        EXE_END

    EXE_DONE; // to avoid warnings when the jump anchor is not used.
    EXE_DONE_JUMP_ANCHOR:
        ++ip;
        if (g_conf.pause_after_instruction){
            var_stack.Print();
            char a;
            Util::Println("press ENTER to continue...");
            cin.get(a);
        }
    }
    Util::Println("[var stack size: %d]\n[context size: %d]", var_stack.Size(), context.Size());
    running_codes = NULL;
}
