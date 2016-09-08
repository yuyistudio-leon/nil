#include<iostream>
#include<vector>
using namespace std;
int main(){
	vector<int> arr;
	arr.push_back(99);
	arr.push_back(88);
	cout << arr.size() << endl;
	arr.erase(arr.end() - 1, arr.end());
	cout << arr.size() << endl;
	return 0;
}
