#include "stdafx.h"

#define DEFAULT_BUFLEN 512

#define WIN32_LEAN_AND_MEAN

#include <iostream>

#include "ITCPSocket.h"
#pragma comment (lib, "Ws2_32.lib")

ITCPSocket::ITCPSocket()
{
}


ITCPSocket::~ITCPSocket()
{
}


int ITCPSocket::init(int aPort, struct addrinfo*& aAddrinfo, string aIp)
{
	WSADATA wsaData;
	struct addrinfo hints;
	aAddrinfo = NULL;
	string port = std::to_string(aPort);
	bool isServer = aIp.empty();

	// Initialize Winsock
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (ret != 0)
	{
		cout<<"WSAStartup failed with error: "<<ret<<endl;
		return 2;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = isServer ? AI_PASSIVE : 0; //if aIp is empty - is server

	// Resolve the server address and port
	ret = getaddrinfo(aIp.c_str(), port.c_str(), &hints, &aAddrinfo);
	
	if (ret != 0)
	{
		cout << "getaddrinfo failed with error: " << ret << endl;
		WSACleanup();
		return 3;
	}

	return 0;
}

int ITCPSocket::reciveBuffer(SOCKET aConnectSocket, vector<char>& aBuffer)
{
	int ret = 0;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	do {

		int rc = recv(aConnectSocket, recvbuf, recvbuflen, 0);

		if (rc > 0)
		{
			aBuffer.insert(aBuffer.end(), recvbuf, recvbuf + rc);
			return 0;

		}
		else if (rc == 0)
		{
			cout << "Connection closed" << endl;
			return 1;
		}
		else if ( (rc == SOCKET_ERROR) && (WSAGetLastError() == WSAEMSGSIZE) )
		{
			aBuffer.insert(aBuffer.end(), recvbuf, recvbuf + recvbuflen);
		}
		else
		{
			cout << "recv failed with error: " << WSAGetLastError() << endl;
			return -2;
		}

	} while ( true  );

	return 0;
}

int ITCPSocket::sendBuffer(SOCKET aConnectSocket, vector<char>& aBuffer)
{
	if (aBuffer.size() == 0)
	{
		return 0;
	}
	int ret = send(aConnectSocket, &(aBuffer[0]), aBuffer.size(), 0);

	if (ret == SOCKET_ERROR)
	{
		cout << "send failed with error" << WSAGetLastError() << endl;
	}

	return ret;


}

