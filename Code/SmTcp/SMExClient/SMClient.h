#pragma once

#include "../Common/BufferClient.h"
#include "../Common/IntBuffer.h"

#include <list>


class SMClient : public BufferClient
{
public:
	SMClient();
	
	virtual ~SMClient();

	virtual int onConnect(SOCKET aConnectSocket);

protected:
		

	void stringToBuffer(string aStr, IntBuffer& aBuff);
};

