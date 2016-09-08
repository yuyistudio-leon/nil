#ifndef _CODE_HEADER_
#define _CODE_HEADER_

#include "define.h"
using namespace std;

class Code{
public:
	Code(string _action, Str _str1 = "", Num _num1 = 0, Str _str2 = "", Num _num2 = 0)
		: action(_action), str1(_str1), num1(_num1), str2(_str2), num2(_num2){}
	void PatchJumpOffset(int offset){
		num1 = offset;
	}
	string action;
	Str str1;
	Num num1;
	Str str2;
	Num num2;
};


#endif
