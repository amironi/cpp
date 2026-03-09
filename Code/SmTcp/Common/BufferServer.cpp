#include "stdafx.h"
#include "BufferServer.h"

using namespace std;

#define WIN32_LEAN_AND_MEAN

#include <iostream>

BufferServer::BufferServer()
{
}


BufferServer::~BufferServer()
{
}

int BufferServer::startConnect(int aPort)
{
	struct addrinfo *addr = NULL;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	int ret = init(aPort, addr);
	
	if (0 != ret)
	{
		return ret;
	}
	
	// Create a SOCKET for connecting to server
	ListenSocket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) 
	{
		cout << "socket failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	ret = bind(ListenSocket, addr->ai_addr, (int)addr->ai_addrlen);
	if (ret == SOCKET_ERROR) 
	{
		cout<<"bind failed with error: "<<WSAGetLastError()<<endl;
		freeaddrinfo(addr);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(addr);


	while (true)
	{
		ret = listen(ListenSocket, SOMAXCONN);
		if (ret == SOCKET_ERROR) 
		{
			cout<<"listen failed with error: "<<WSAGetLastError()<<endl;
			break;
		}

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			cout<<"accept failed with error: " << WSAGetLastError() << endl;
			break;
		}

		cout << "Connection started" << endl;
		
		do
		{
			vector<char> buffer;
			if (0 != reciveBuffer(ClientSocket, buffer))
			{
				break;
			}
			if (false == onDataRec(ClientSocket, buffer))
			{
				break;
			}
			
		} while (true);
	}

	// No longer need server socket
	closesocket(ListenSocket);

	// shutdown the connection since we're done
	ret = shutdown(ClientSocket, SD_SEND);
	if (ret == SOCKET_ERROR) {
		cout<<"shutdown failed with error: "<<WSAGetLastError()<<endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	cin.get();
	return 0;
}
