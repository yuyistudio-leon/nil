#ifndef _GC_HEADER_
#define _GC_HEADER_

#include "define.h"
using namespace std;

class GC{
public:
    GC();
    void ScanAll(PValue root, bool show = false, int level = 0);
    void RegistVar(PValue var);
    void CollectGarbage(PValue root);
    void DebugPrint(PValue root);
    void ShowAllVars(PValue root);
public:
    GcFlag flag_scanned;
    set<PValue> vars;
private:
    int DeleteGarbage(PValue root);
    void SwapFlag();
};

#endif


