#ifndef _VALUE_HEADER_
#define _VALUE_HEADER_

#include "define.h"
#include "util.h"
#include "gc.h"
using namespace std;


class Value{
private:
    friend class GC;
public:
    // returned value will be managed by GC
    Value(GC* gc);
    // returned value should be managed by your code
    Value();
    void SetGC(GC* new_gc);
private:
    void CommonInit();
	PValue __GetByStr(const Str index, bool index_elementary = true);
    void __EraseByStr(const Str index);
public:
    // for hash
    template<typename TYPE> PValue Get(TYPE index, bool index_elementary = true);
    template<typename TYPE> void Erase(TYPE index);

    // for func
    PValue Call(VM* pvm, PValue params);

    // for all types
    PValue Set(const PValue new_value);
    PValue Set(const Str new_value);
    PValue Set(const char* new_value);
    PValue Set(const int new_value);
    PValue Set(const bool new_value);
    PValue SetNil();
    PValue Set(const Num new_value);
    PValue Set(const Type new_type);
    PValue Set(const Str index, const Str new_value);
    PValue Set(CFunc new_value);
	PValue SetFunc(const int ip);
    // basic methods
    static string TypeStr(Type type);
    string GetHash();
    string TypeStr();
    string ToString();
    Num ToNum();
    size_t Size();
    bool IsTrue();
    #define BINOP_DECLARE(func_name) \
    static bool func_name(PValue pv1, PValue pv2)
    BINOP_DECLARE(Gt);
    BINOP_DECLARE(Lt);
    BINOP_DECLARE(Ge);
    BINOP_DECLARE(Le);
    BINOP_DECLARE(Eq);
    BINOP_DECLARE(Ne);

    string Concat(PValue pv1, PValue pv2);

	inline bool IsTemp(){
		return gc == NULL;
	}
	inline bool IsNum(){return type == NUM;}
	inline bool IsStr(){return type == STR;}
	inline bool IsNil(){
		return type == NIL;
	}
	inline bool IsCFunc(){
		return type == CFUNC;
	}
	inline bool IsFunc(){
		return type == FUNC;
	}
	inline bool IsHash(){
		return type == HASH || (type == REF && ref->IsHash());
	}
    PValue Inc();
    PValue Dec();
    PValue Negate();
	PValue Not();
    bool IsEqual(PValue pv);
    void DebugPrint();
    // index operation
	template<typename TYPE_INDEX, typename TYPE_VALUE> PValue Set(const TYPE_INDEX index, TYPE_VALUE new_value);
	template<typename TYPE>	PValue InsertIndex(PValue index, TYPE value);
	PValue SetMetaHash(PValue meta_hash);
public:
    virtual ~Value();
//todo protected:
public:
    // type info
    Type type;
    // data fields
    Num num;
    Str str;
    StrHash str_hash;
    PValue ref;
    int ref_cnt;
    CFunc cfunc;
    PValue meta_hash;
    // value gc flag
    GcFlag gc_flag;
    // gc pointer
    GC* gc;
};


// for hash
template<typename TYPE> PValue Value::Get(TYPE index, bool index_elementary){
	Value index_value;
	index_value.Set(index);
	return __GetByStr(index_value.GetHash(), index_elementary);
}
template<typename TYPE> void Value::Erase(TYPE index){
	Value index_value;
	index_value.Set(index);
	return __EraseByStr(index_value.GetHash());
}
// index operation
template<typename TYPE_INDEX, typename TYPE_VALUE> PValue Value::Set(const TYPE_INDEX index, TYPE_VALUE new_value){
	return this->Get<TYPE_INDEX>(index)->Set(new_value);
}
template<typename TYPE>	PValue Value::InsertIndex(PValue index, TYPE value){
	return this->__GetByStr(index->GetHash())->Set(value);
}

#endif
