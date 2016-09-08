#ifndef _UTIL_HEADER_
#define _UTIL_HEADER_

#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
using namespace std;

class Util{
public:
	static string ToString(double num);
	static string ToString(void* num);
	static string ToIndentString(int count);
	static inline bool IsZero(double num);
	template<typename TYPE>
	static TYPE Abs(TYPE var);
private:
	static void __Print(const char* fmt, va_list mark);
	static void __PrintlnWithPrefix(const char* prefix, const char* fmt, va_list mark);
public:
	static void Print(const char* fmt, ...);
	static void Println(const char* fmt, ...);
	static void Println();
	static void Fatal(const char* fmt, ...);
	static void Debug(const char* fmt, ...);
};

template<typename TYPE>
TYPE Util::Abs(TYPE var){
	if (var < 0)return -var;
	return var;
}
bool Util::IsZero(double num){
	return (Abs<double>(num) < 0.000001);
}
#endif
