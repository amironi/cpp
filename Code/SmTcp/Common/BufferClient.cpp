#include "stdafx.h"
#include "BufferClient.h"

#include <iostream>


BufferClient::BufferClient()
{
}


BufferClient::~BufferClient()
{
}

int BufferClient::startConnect(int aPort, const string& aIp )
{
	SOCKET ConnectSocket = INVALID_SOCKET;

	struct addrinfo *addr = NULL;
	struct addrinfo *ptr = NULL;
	

	int ret = init(aPort, addr);

	

	if (0 != ret)
	{
		return ret;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = addr; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
		{
			cout<<"socket failed with error: "<<WSAGetLastError()<<endl;
			WSACleanup();
			return 1;
		}

		// Connect to server.
		ret = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (ret == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(addr);

	if (ConnectSocket == INVALID_SOCKET) {
		cout << "Unable to connect to server!"<<endl;
		WSACleanup();
		return 1;
	}

	ret = onConnect(ConnectSocket);

	if (ret != 1)
	{
		cout << "Error: " << ret << endl;
	}

	
	// shutdown the connection since no more data will be sent
	ret = shutdown(ConnectSocket, SD_SEND);
	if (ret == SOCKET_ERROR) 
	{
		cout<<"shutdown failed with error: "<<WSAGetLastError()<<endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
