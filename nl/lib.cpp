#include "lib.h"

#define RETURN_IF_NO_PARAMS	assert (params != NULL); \
	if (params->IsNil()) return;


LIB_FUNC(Print){
	int param_count = (unsigned)params->Size();
	for (int i = 0; i < param_count; ++i){
  		Util::Print("%s ", params->Get(i)->ToString().c_str());
	}
	Util::Println();
}
LIB_FUNC(Write){
	int param_count = (unsigned)params->Size();
	for (int i = 0; i < param_count; ++i){
  		Util::Print("%s", params->Get(i)->ToString().c_str());
	}
}
LIB_FUNC(SetMetaHash){
    if (params->Size() != 2){
    	Util::Fatal("error: count of parameters should be 2, but is %d", params->Size());
    }
    params->Get(0)->SetMetaHash(params->Get(1));
}
LIB_FUNC(Exit){
	exit(0);
}
