#ifndef _STACK_HEADER_
#define _STACK_HEADER_

#include "define.h"
using namespace std;

template<typename T>
class Stack {
protected:
	vector<T> data;
public:
	typedef typename vector<T>::reverse_iterator reverse_iterator;
	typedef typename vector<T>::iterator iterator;
	virtual T& Push();
	virtual T& Push(T t);
	virtual void Pop();
	T& Top();
	T& operator[](int index);
	size_t Size();
	T& Set(int index, T &new_value);
	void ClearStack();
	void ClearTail(int n);
	inline reverse_iterator rbegin(){return data.rbegin();}
	inline reverse_iterator rend(){return data.rend();}
	inline iterator begin(){return data.begin();}
	inline iterator end(){return data.end();}
};

template<typename T>
T& Stack<T>::Push(){
	data.push_back(T());
	return Top();
}
template<typename T>
T& Stack<T>::Push(T t){
	data.push_back(t);
	return Top();
}
template<typename T>
void Stack<T>::Pop(){
	assert(data.size() > 0);
	data.pop_back();
}
template<typename T>
T& Stack<T>::Top(){
	return *data.rbegin();
}
template<typename T>
T& Stack<T>::operator[](int index){
	assert(index >= 0 && index < data.size());
	return data[index];
}
template<typename T>
size_t Stack<T>::Size(){
	return data.size();
}
template<typename T>
T& Stack<T>::Set(int index, T &new_value){
	data[index] = new_value;
	return new_value;
}
template<typename T>
void Stack<T>::ClearStack(){
	data.clear();
}
template<typename T>
void Stack<T>::ClearTail(int n){
	assert(n >= 0);
	data.erase(data.end() - n, data.end());
}

#endif
