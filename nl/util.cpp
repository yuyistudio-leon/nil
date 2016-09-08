#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

string Util::ToString(double num){
	const int buf_size = 1024;
	char buf[buf_size];
	snprintf(buf, buf_size, "%lf", num);
	return buf;
}
string Util::ToString(void* num){
	const int buf_size = 1024;
	char buf[buf_size];
	snprintf(buf, buf_size, "%p", num);
	return buf;
}
string Util::ToIndentString(int count){
	const int buf_size = 1024 * 128;
	assert(count < buf_size);
	char buf[buf_size];
	memset(buf, ' ', count * 2);
	buf[count * 2] = '\0';
	return buf;
}
void Util::__Print(const char* fmt, va_list mark){
	char buf[1024*128];
	vsnprintf(buf, sizeof(buf), fmt, mark);
	cout << buf;
}
void Util::__PrintlnWithPrefix(const char* prefix, const char* fmt, va_list mark){
	cout << "[" << prefix << "]";
	__Print(fmt, mark);
	cout << endl;
}
void Util::Print(const char* fmt, ...){
	va_list mark;
	va_start(mark, fmt);
	__Print(fmt, mark);
}
void Util::Println(const char* fmt, ...){
	va_list mark;
	va_start(mark, fmt);
	__Print(fmt, mark);
	cout << endl;
}
void Util::Println(){
	cout << endl;
}
void Util::Fatal(const char* fmt, ...){
	va_list mark;
	va_start(mark, fmt);
	__PrintlnWithPrefix("Fatal", fmt, mark);
	exit(-1);
}
void Util::Debug(const char* fmt, ...){
	va_list mark;
	va_start(mark, fmt);
	__PrintlnWithPrefix("Debug", fmt, mark);
}
