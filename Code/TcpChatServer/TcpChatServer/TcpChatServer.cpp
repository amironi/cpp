// TcpChatServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <winsock2.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 1212



int _tmain(int argc, _TCHAR* argv[])
{
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = 0;//inet_addr("0.0.0.0.0.0);
	service.sin_port = htons(DEFAULT_PORT);

	if (bind(ListenSocket,
		(SOCKADDR *) & service, sizeof (service)) == SOCKET_ERROR) {
			wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
	}
	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	fd_set socketList;
	FD_ZERO(&socketList);
	FD_SET(ListenSocket, &socketList);

	// Create a SOCKET for accepting incoming requests.
	wprintf(L"Waiting for client to connect...\n");

	while(1)
	{
		fd_set outputList;

		memcpy(&outputList, &socketList, sizeof(fd_set) );

		select(outputList.fd_count, &outputList, NULL, NULL, NULL);

		{
			for( UINT i = 0 ; i < outputList.fd_count; i++ )
			{
				//if( FD_ISSET(ListenSocket, outputList) )
				if( outputList.fd_array[i] == ListenSocket )
				{
					//----------------------
					// Accept the connection.
					SOCKET AcceptSocket = accept(ListenSocket, NULL, NULL);


					if (AcceptSocket == INVALID_SOCKET) {
						wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
						closesocket(ListenSocket);
						WSACleanup();
						return 1;
					} 
					else
					{
						FD_SET(AcceptSocket, &socketList);
						wprintf(L"Client connected.\n");
					}
				}
				else
				{
					int recvbuflen = 0;
					CHAR recvbuf[1024];

					SOCKET clientSocket = outputList.fd_array[i];
					int iResult = recv(clientSocket, recvbuf, 1024, 0);
					if ( iResult > 0 )
					{
						wprintf(L"Bytes received: %d\n", iResult);

						for( UINT j = 0 ; j < socketList.fd_count; j++ )
						{
							//if( FD_ISSET(ListenSocket, outputList) )
							if( socketList.fd_array[j] != ListenSocket && 
								socketList.fd_array[j] != clientSocket )
							{
								iResult = send( socketList.fd_array[j], recvbuf, iResult, 0 );

								if (iResult == SOCKET_ERROR) {
									wprintf(L"send failed with error: %d\n", WSAGetLastError());
									FD_CLR(socketList.fd_array[j], &socketList);
									closesocket(clientSocket);
									WSACleanup();
									return 1;
								}
							}
						}
						printf("Bytes Sent: %d\n", iResult);
					}
 					else
 					{
 						wprintf(L"Connection closed\n");
 
 						FD_CLR(clientSocket, &socketList);
 
 						closesocket(clientSocket);
 					}
				}
			}
		}
	}
	// No longer need server socket
	closesocket(ListenSocket);

	WSACleanup();
	return 0;

}

