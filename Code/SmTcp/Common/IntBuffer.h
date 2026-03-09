#pragma once

#include <vector>
#include <list>

using namespace std;

class IntBuffer
{
public:
	IntBuffer();

	IntBuffer(vector<char>& aBuffer);

	~IntBuffer();

	vector<char> mBuffer;

	int getInt(int aPos);

	int getIntList(list<int>& aList);

	void setIntList(list<int>& aList);

	void addInt(int aVal);

	char* get();

	int size();

	void sort();

	void clear();
};	

