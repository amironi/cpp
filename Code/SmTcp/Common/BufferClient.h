#pragma once

#include "ITCPSocket.h"

class BufferClient : public ITCPSocket
{
public:
	BufferClient();

	~BufferClient();

	int startConnect(int aPort, const string& aIp);

	virtual int onConnect(SOCKET aConnectSocket) = 0;
};

