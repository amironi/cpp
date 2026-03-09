#include "stdafx.h"
#include "IntBuffer.h"


IntBuffer::IntBuffer()
{
}

IntBuffer::IntBuffer(vector<char>& aBuffer)
{
	mBuffer = aBuffer;
}

IntBuffer::~IntBuffer()
{
}

int IntBuffer::getInt(int aPos)
{
	int* d = (int*)&(mBuffer[aPos]);
	return *d;
}

int IntBuffer::getIntList(list<int>& aList)
{
	for (int i = 0; i < mBuffer.size() - 3; i += 4)
	{
		int d = getInt(i);
		aList.push_back(d);
	}

	return 0;
}

void IntBuffer::setIntList(list<int>& aList)
{
	clear();
	for (auto i : aList)
	{
		addInt(i);
	}
}


void IntBuffer::addInt(int aVal)
{
	mBuffer.push_back((char)aVal);
	mBuffer.push_back((char)(aVal >> 8));
	mBuffer.push_back((char)(aVal >> 16));
	mBuffer.push_back((char)(aVal >> 24));
}

char* IntBuffer::get()
{
	return &(mBuffer[0]);
}

int IntBuffer::size()
{
	return mBuffer.size();
}

void IntBuffer::clear()
{
	mBuffer.clear();
}
