#pragma once

#include "ITCPSocket.h"

class BufferServer : public ITCPSocket
{
public:
	
	BufferServer();

	virtual ~BufferServer();

	int startConnect(int aPort);

	virtual bool onDataRec(SOCKET aConnectSocket, vector<char>& aBuffer) = 0;
};

