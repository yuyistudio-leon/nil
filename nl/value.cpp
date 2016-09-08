#include "value.h"
#include "gc.h"


PValue Value::Call(VM* pvm, PValue params){
    switch(type){
    case CFUNC:
        cfunc(pvm, params);
        break;
    default:
        Util::Fatal("DEBUG error: cannot call on non-cfunc value");
    }
    return this;
}
PValue Value::Set(const Type new_type){
    type = new_type;
    return this;
}
string Value::ToString(){
    switch(type){
    case NIL:
        return "<null>";
    case NUM:
       	return Util::ToString(num);
    case STR:
        return str;
    case HASH:
        return "<hash:" + Util::ToString((void*)this) + ">";
    case REF:
        return "<ref:" + ref->ToString() + ">";
    case FUNC:
    	return "<func:" + Util::ToString(num) + ">";
    case CFUNC:
        return "<cfunc:" + Util::ToString((void*)cfunc) + ">";
    default:
        return "<unknown>";
    }
}
size_t Value::Size(){
	switch(type){
    case NIL:
        return 0;
    case NUM:
    case STR:
    case FUNC:
    case CFUNC:
        return 1;
    case HASH:
        return str_hash.size();
    case REF:
        return ref->Size();
    default:
        Util::Fatal("unknown type");
        return 0;
    }
}

Value::Value(GC* gc){
    CommonInit();
    assert(gc != NULL);
    this->gc = gc;
    gc->RegistVar(this);
}
Value::Value(){
    CommonInit();
}
void Value::CommonInit(){
    type = NIL;
    this->gc = NULL;
    gc_flag = GC_FLAG_STATE1;
    meta_hash = NULL;
}

PValue Value::__GetByStr(const Str index, bool index_elementary){
    // 将nil转换为hash，以便使用index操作
    if (type == NIL){
        type = HASH;
    }
    if (type == HASH){
    	StrHashIter result = str_hash.find(index);
	    if (result != str_hash.end()){
	        assert(result->second != NULL);
	        return result->second;
	    }
        // try to index meta
        if (meta_hash){
            return meta_hash->__GetByStr(index);
        }
    }else if (type == REF){
    	return ref->__GetByStr(index, index_elementary);
    }else{
        // try to index meta
        if (meta_hash){
            return meta_hash->__GetByStr(index);
        }
        // index elementary data
        if (index_elementary){
            type = HASH;
            str_hash.clear();
        }else{
            Util::Fatal("error: cannot index elementary data");
        }
    }

    // insert a new value
    PValue nil_value = new Value(gc);
    assert(nil_value != NULL);
    str_hash[index] = nil_value;
    return nil_value;
}
PValue Value::Set(const Num new_value){
	type = NUM;
	num = new_value;
    return this;
}
PValue Value::Set(const PValue new_value){
    type = new_value->type;
    switch(type){
    case HASH:
    	type = REF;
        ref = new_value;
        break;
    case REF:
        ref = new_value->ref;
        break;
    case STR:
        str = new_value->str;
        break;
    case NUM:
    case FUNC:
    	num = new_value->num;
    	break;
    case NIL:
        // do nothing
        break;
    case CFUNC:
        cfunc = new_value->cfunc;
        break;
    default:
        Util::Fatal("unknown value type : %s", TypeStr().c_str());
    }
    return this;
}
PValue Value::Set(const Str new_value){
	type = STR;
    str = new_value;
    return this;
}


void Value::SetGC(GC* new_gc){
    gc = new_gc;
}
void Value::__EraseByStr(const Str index){
    if (type == REF){
        ref->__EraseByStr(index);
    }
    str_hash.erase(index);
}
PValue Value::Set(const char* new_value){
    return Set(string(new_value));
}
PValue Value::Set(const int new_value){
    return Set((Num)new_value);
}
PValue Value::Set(const bool new_value){
    return Set(new_value ? 1 : 0);
}
PValue Value::SetNil(){
    type = NIL;
    return this;
}
PValue Value::Set(CFunc new_value){
    type = CFUNC;
    cfunc = new_value;
    return this;
}
PValue Value::SetFunc(const int ip){
    type = FUNC;
    num = ip;
    return this;
}
// basic methods
string Value::TypeStr(Type type){
    switch(type){
    case NIL: return "nil";
    case FUNC: return "func";
    case CFUNC: return "cfunc";
    case STR: return "str";
    case NUM: return "num";
    case HASH: return "hash";
    default: return "unknown";
    }
}
string Value::GetHash(){
    if (type == REF){
        return ref->GetHash();
    }
    return this->TypeStr() + this->ToString();
}
string Value::TypeStr(){
    return Value::TypeStr(type);
}
Num Value::ToNum(){
    switch(type){
    case NUM:
        return num;
    default:
        return 0;
    }
}
bool Value::IsTrue(){
    switch (type){
    case NIL:
        return false;
    case NUM:
        return Util::Abs<Num>(num) > 0.00001;
    case STR:
        return str != "";
    default:
        return true;
    }
}
#define BINOP(func_name) \
bool Value::func_name(PValue pv1, PValue pv2)
BINOP(Gt){
    return !Le(pv1, pv2);
}
BINOP(Lt){
    if (pv1->type != pv2->type) return false;
    switch(pv1->type){
    case NUM: return pv1->num < pv2->num;
    case STR: return strcmp(pv1->str.c_str(), pv2->str.c_str()) < 0;
    default: return false;
    }
}
BINOP(Ge){
    return !Lt(pv1, pv2);
}
BINOP(Le){
    if (pv1->type != pv2->type) return false;
    switch(pv1->type){
    case NUM: if (Util::IsZero(pv1->num-pv2->num)) return true; else return pv1->num < pv2->num;
    case STR: return strcmp(pv1->str.c_str(), pv2->str.c_str()) <= 0;
    default: return false;
    }
}
BINOP(Eq){
    if (pv1->type != pv2->type){
        if (pv1->type == REF){
            return pv2->type == HASH && pv1->ref == pv2;
        }
        if (pv2->type == REF){
            return pv1->type == HASH && pv2->ref == pv1;
        }
        return false;
    }
    switch(pv1->type){
    case NUM:
    case FUNC:
        return Util::IsZero(pv1->num - pv2->num);
    case STR:
        return pv1->str == pv2->str;
    case CFUNC:
        return pv1->cfunc == pv2->cfunc;
    case HASH:
        return false;
    case REF:
        return pv1->ref == pv2->ref;
    case NIL:
        return true;
    default:
        Util::Fatal("error: unknown data type: %s", pv1->TypeStr().c_str());
    }
    return false;
}
BINOP(Ne){
    return !Eq(pv1, pv2);
}

string Value::Concat(PValue pv1, PValue pv2){
    return pv1->ToString() + pv2->ToString();
}

PValue Value::Inc(){
    if (type == NUM){
        num++;
    }
    return this;
}
PValue Value::Dec(){
    if (type == NUM){
        num--;
    }
    return this;
}

PValue Value::Negate(){
    switch(type){
    case NUM:
        num = -num;
        break;
    case STR:
        str = string(str.rbegin(), str.rend());
        break;
    default:
        type = NIL;
        break;
    }
    return this;
}
PValue Value::Not(){
    switch(type){
    case NUM:
        if (Util::IsZero(num)) num = 1; else num = 0;
        break;
    case STR:
        if (str == "") str = "!"; else str = "";
        break;
    case NIL:
        type = NUM;
        num = 1;
        break;
    default:
        type = NIL;
        break;
    }
    return this;
}
bool Value::IsEqual(PValue pv){
    return Eq(this, pv);
}
void Value::DebugPrint(){
    Util::Println("+++++++++++++++++++++++++++++++++++++");
    assert(gc != NULL);
    gc->DebugPrint(this);
}
PValue Value::SetMetaHash(PValue meta_hash){
    if (type == REF){
        return ref->SetMetaHash(meta_hash);
    }
    this->meta_hash = meta_hash->IsHash() ? meta_hash : NULL;
    return this;
}
Value::~Value(){}
