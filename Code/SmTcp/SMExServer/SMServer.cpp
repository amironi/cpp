#include "stdafx.h"
#include "SMServer.h"

#include <list>
#include <algorithm>


SMServer::SMServer()
{
}


SMServer::~SMServer()
{
}

bool SMServer::onDataRec(SOCKET aClientSocket, vector<char>& aBuffer)
{
	list<int> sorted;
	
	IntBuffer buff(aBuffer);

	buff.getIntList(sorted);

	sorted.sort();
	
	buff.setIntList(sorted);

	list<int> sortedtt;
	buff.getIntList(sortedtt);

	sendBuffer(aClientSocket, buff.mBuffer);

	return true;
}
