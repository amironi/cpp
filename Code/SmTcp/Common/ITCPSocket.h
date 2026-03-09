#pragma once

#include <string> 
#include <vector> 
#include <ws2tcpip.h>

#define DEFAULT_PORT 27015

using namespace std;

class ITCPSocket
{
public:
	ITCPSocket();
	
	virtual ~ITCPSocket();

	//for server don't set aIp
	int init(int aPort, struct addrinfo*& aAddrinfo, string aIp = string() );

protected:

	int reciveBuffer(SOCKET aConnectSocket, vector<char>& aBuffer);

	int sendBuffer(SOCKET aConnectSocket, vector<char>& aBuffer);
};

