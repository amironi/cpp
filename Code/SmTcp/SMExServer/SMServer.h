#pragma once

#include "../Common/BufferServer.h"
#include "../Common/IntBuffer.h"

class SMServer : public BufferServer
{
public:
	SMServer();
	~SMServer();
	virtual bool onDataRec(SOCKET aClientSocket, vector<char>& aBuffer);
};

