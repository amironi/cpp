
#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>


// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <windows.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <queue>
#include <time.h>
#include <sstream>
#include <windows.h>

using namespace std;



template<bool... args>
int reversed_binary_value() {
	const int size = sizeof...(args);
	bool v[size] = { args... };
	int res = 0;
	for (int i = size - 1; i >= 0; --i) {
		res = res * 2 + v[i];
	}
	return res;
}


template<bool T1>
int reversed_binary_value2() {
	return T1;
}

template<bool T1, bool T2, bool... args>
int reversed_binary_value2() {
	return T1 + 2 * reversed_binary_value<T2, args...>();
}


int main()
{
	cout << reversed_binary_value<0, 0, 0 ,1>();
	cout << reversed_binary_value2<0, 0, 0, 1>();

	return 0;
}