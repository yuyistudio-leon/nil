#include "gc.h"
#include "value.h"
#include "util.h"

GC::GC(){
    flag_scanned = GC_FLAG_STATE2;
}
void GC::RegistVar(PValue var){
    vars.insert(var);
}
void GC::CollectGarbage(PValue root){
    ScanAll(root);
    Util::Println("collect %d variables", DeleteGarbage(root));
    SwapFlag();
}

void GC::DebugPrint(PValue root){
	ScanAll(root, true);
	SwapFlag();
	ScanAll(root);
	SwapFlag();
}
void GC::ScanAll(PValue root, bool show, int level){
    if (level > 32) return;
    if (!show && root->gc_flag == flag_scanned) return;
    root->gc_flag = flag_scanned;
	if (show){
		Util::Println("+ %s ┕%s", Util::ToIndentString(level).c_str(), root->ToString().c_str());
	}
    switch(root->type){
    case REF:
        ScanAll(root->ref, show, level + 1);
        break;
    case HASH:
        for (StrHashIter ni = root->str_hash.begin(); ni != root->str_hash.end(); ni++){
        	if (show){
        		Util::Println("+ %s<%s>", Util::ToIndentString(level + 1).c_str(), ni->first.c_str());
        	}
            ScanAll(ni->second, show, level + 1);
        }
        break;
    default:
        break;
    }
}
void GC::ShowAllVars(PValue root){
    int counter_garbage = 0;
	ScanAll(root);
    for (set<PValue>::iterator iter = vars.begin(); iter != vars.end(); iter++){
        if ((*iter)->gc_flag != flag_scanned){
        	Util::Println("x Garbage: %s", (*iter)->ToString().c_str());
        	counter_garbage++;
        }else{
        	Util::Println(". Data: %s", (*iter)->ToString().c_str());
        }
    }
	SwapFlag();
	ScanAll(root);
	SwapFlag();
	Util::Println("Garbage percent: %lf", counter_garbage * 1.0f / vars.size());
}
int GC::DeleteGarbage(PValue root){
    int counter_garbage = 0;
    for (set<PValue>::iterator iter = vars.begin(); iter != vars.end();){
        if ((*iter)->gc_flag != flag_scanned){
            delete *iter;
            vars.erase(iter++);
            counter_garbage++;
        }else{
            iter++;
        }
    }
    return counter_garbage;
}
void  GC::SwapFlag(){
    if (flag_scanned == GC_FLAG_STATE2){
        flag_scanned = GC_FLAG_STATE1;
    }else{
        flag_scanned = GC_FLAG_STATE2;
    }
}
