#ifndef _DEFINE_HEADER_
#define _DEFINE_HEADER_

// independent
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <functional>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

using namespace std;

#define VERSION_STR "0.0.0.1"

typedef double Num;
typedef string Str;
typedef char GcFlag;

#define DEBUG

typedef struct {
	bool print_arranged_base_ip;
	bool print_generated_code;
	bool print_running_code;
	bool print_call_stack;
	bool print_parse_op;
	bool pause_after_instruction;
} GlobalConfig;
extern GlobalConfig g_conf;

#define GC_FLAG_STATE1 ('a')
#define GC_FLAG_STATE2 ('b')

enum Type{NUM,STR,HASH,REF,FUNC,CFUNC,NIL};

class Util;
class GC;
class Value;
class Function;
class Code;
class VM;
class Parser;

typedef Value* PValue;
typedef map<Str,PValue> StrHash;
typedef map<Num,PValue> NumHash;
typedef map<Str,PValue>::iterator StrHashIter;
typedef map<Num,PValue>::iterator NumHashIter;
typedef pair<Str,PValue> StrHashPair;
typedef pair<Num,PValue> NumHashPair;
typedef vector<Code> Codes;
typedef vector<Code>::iterator CodesIter;
typedef void (*CFunc)(VM* pvm, PValue params);

#define GBUF_SIZE (1024*1024)
extern char* g_buf;

#include "stack.h"

#endif
