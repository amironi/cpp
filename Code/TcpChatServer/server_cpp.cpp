<html><!-- #BeginTemplate "/Templates/ftp.dwt" -->
<head>
<title>Untitled Document</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>

<body bgcolor="#FFFFFF">
<table width="100%" border="0" height="173">
  <tr> 
    <td width="1%" height="90"> 
      <h1>&nbsp;</h1>
    </td>
    <td width="98%" height="90" valign="bottom"> 
      <!-- #BeginLibraryItem "/Library/project_menu.lbi" -->
<table width="100%" border="0">
  <tr> 
    <td width="41%">
      <div align="center"><font size="4"><a href="http://www.mtholyoke.edu/%7Exuansun/csamples/cpfiles/cpfiles_cpp.html">Copy 
        Files</a> | <a href="http://www.mtholyoke.edu/%7Exuansun/csamples/ftp/server_cpp.html">FTP</a> 
        | </font><font size="3"><a href="../../codes.html"><font size="4"><i><b><font size="3">Return 
        to sample codes main page.</font></b></i></font></a></font></div>
    </td>
  </tr>
</table>
<!-- #EndLibraryItem --><h1><font color="#86A82D">FTP Project</font></h1>
      <table width="100%" border="0">
        <tr> 
          <td width="30%" height="2"> 
            <h4><font size="4"> C++ Source Files in the Project:</font></h4>
          </td>
          <td width="70%" height="2"><font size="4"><a href="server_cpp.html">server.cpp</a> 
            | <a href="client_cpp.html">client.cpp</a> | <a href="administration_cpp.html">administration.cpp</a> 
            | <a href="item_cpp.html">item_record.cpp</a></font></td>
        </tr>
        <tr> 
          <td width="30%" height="37"> 
            <h4><font size="4"> C++ Header Files in the Project:</font></h4>
          </td>
          <td width="70%" height="37"><font size="4"><a href="item_h.html">item_record.h</a></font></td>
        </tr>
      </table>
      <hr>
    </td>
    <td width="1%" height="90"> 
      <h1>&nbsp;</h1>
    </td>
  </tr>
  <tr> 
    <td width="1%" height="6">&nbsp;</td>
    <td width="98%" height="6"><!-- #BeginEditable "sample%20codes" --> 
      <pre>
//======================================================================
//  By Xuan Sun 
//  Dec. 22, 99
//  File: server.cpp
//  Description: This is a FTP server. The main thread accepts connection 
//		from clients, and starts a thread for each of the clients.
//		It retrieves a data file from the source date file at the   	
//		beginning. This data file contains a list of files and folders
//		that are available for people to download and upload to.
//		An array of objects, itemRecord, are generated from this 
//		data file. They are use all through the file transfer process.
//		Clients communicate with the server what they want to do 
//		by sending the index number of this array.
//======================================================================= 

#include windows.h  // not able to use brackets because of HTML tags
#include stdio.h 
#include string.h
#include stdlib.h
#include time.h
#include "item_record.h"

void Thread(LPVOID);
void upload(SOCKET &);
void download(SOCKET &);
void create(void);
void store(void);
void retrieve(void);
void load(SOCKET &);

HANDLE ThreadHandle;
DWORD ThreadID;
itemRecord items[11];
itemRecord currItem;
char loadDir[50];	// where the file is upload to, or download from

void main()
{
	//====================================================
	//	START:	Get Reord 
	//====================================================
	//initialize record list(global)
	create();
	//read from remote file
	retrieve();
	//write to local file
	store();
	//====================================================
	//	START:	Get Record 
	//====================================================


	//====================================================
	//	START:	Initialize Socket Info
	//====================================================
	WORD wVersionRequested;
	WSADATA	wsaData;
	struct sockaddr_in ClientAddr, ServerAddr;
	int ClientSize=sizeof(sockaddr_in);
	int err;
	SOCKET s, new_s;
	
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_port=htons(6000);

	wVersionRequested = MAKEWORD( 1,1);
	//====================================================
	//	END:	Initialize Socket Info
	//====================================================


	//====================================================
	//	START:	Prepare Socket 
	//====================================================
	if (WSAStartup(wVersionRequested, &wsaData)!=0) {
		printf("In WSAStartup, SERVER Error: %d\n",WSAGetLastError());
		exit(1);
	}
	
	s=socket(AF_INET, SOCK_STREAM, 0);
	if (s==INVALID_SOCKET){
		exit(2);
	}
	
	if(bind(s,(struct sockaddr *)&ServerAddr,sizeof(sockaddr_in))==SOCKET_ERROR) {
		printf("In bind, SERVER  Error: %d\n",WSAGetLastError());
		exit(3);
	}

	printf("\nServer waiting for clients...\n");
	
	err=listen(s,4);
	if (err==SOCKET_ERROR) {
		printf("In listen, SERVER  Error: %d\n",WSAGetLastError());
		exit(3);
	}
	//====================================================
	//	END:	Prepare Socket 
	//====================================================


	//====================================================
	//	START:	Spawn Thread 
	//====================================================
	while(1) {
	
		new_s= accept( s,(struct sockaddr *) &ClientAddr, &ClientSize);
		if (err==SOCKET_ERROR) {
			printf("In accept, SERVER  Error: %d\n",WSAGetLastError());
			exit(3);
		}

		//===========================================
		ZeroMemory( &ThreadID, sizeof(DWORD) );
		
		ThreadHandle = CreateThread(

			NULL,		// the thread is not inherited by child process
			0,		// stack size
			(LPTHREAD_START_ROUTINE)Thread,		// thread function
			(LPVOID)&new_s,	// the parameter passed to the thread function
			0,		// execution state specified for the thread.  0 means start immediately
			&ThreadID);	// place to store thread identifier returned by the system
		
		if (ThreadHandle == 0) {
			printf("Create thread error %d\n",GetLastError());
		}
		//===========================================
	}
	//====================================================
	//	END:	Spawn Thread 
	//====================================================


	getchar();
	closesocket(s);
	closesocket(new_s);
	WSACleanup();
}


	//====================================================
	//	START:	Thread Function
	//====================================================
void Thread (LPVOID lpThreadSocket)
{
	int ClientSize=sizeof(sockaddr_in);
	double i=0;
	DWORD ID;
	SOCKET ThreadSocket;
	ID=ThreadID;
	char recordDir[50]="g:\\classwork\\w_ftp\\p_ftp\\record.txt";// source data file

	//get the socket passed on from main thread
	ThreadSocket=*((SOCKET *)lpThreadSocket); 

	strcpy(loadDir,recordDir); // in order to download data file to the client

	//send record file to the client side
	download(ThreadSocket);

	load(ThreadSocket);	// in control of all upload, download and quit

	printf("The client has TERMINATED. Thread %d is quiting as well.\n", ID);
	closesocket(ThreadSocket);
}


	//====================================================
	//	START:	Upload Function 
	//====================================================
void upload(SOCKET &uploadSocket)
{
	HANDLE writetofile;
	char moveBuffer[1100];
	char sNumRead[3], *stopString;
	unsigned long numRead, numWritten;
	int j=3,err,counter=0;
	double i=0;

	// create the file handle that holds the uploaded file
	writetofile=CreateFile(
		loadDir,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (writetofile==INVALID_HANDLE_VALUE) {
		printf("in WRITEHANDLE error: %d.\n", GetLastError);
		exit(2);
	}

	// loop to receive buffer by buffer of file pieces
	do{
		err=recv(uploadSocket,sNumRead, j,0);	// recv the size of this piece
		if (err==SOCKET_ERROR) {
			printf("In recv numRead, THREAD Error: %d\n",WSAGetLastError());
			getchar();
			exit(6);
		}

		i=strtod(sNumRead, &stopString);	// convert from char[] to ul
		numRead = strtoul( sNumRead, &stopString, 10 );

		// if buffer not empty, recv it, and write to the local file
		if (numRead!=0) {
			err=recv(uploadSocket, moveBuffer, numRead,0);
			if (err==SOCKET_ERROR) {
				printf("In recv numRead, THREAD Error: %d\n",WSAGetLastError());
				getchar();
				exit(7);
			}
			err=WriteFile(
				writetofile,
				moveBuffer,	
				numRead,
				&numWritten,
				NULL);

			if (err==NULL){
				printf("in WriteFile error: %d.", GetLastError);
				exit(3);
			}
			counter=counter+(int)numWritten;
		}


	}while( numRead!=0);
		
	printf("\nThread: %d\n", ThreadID);
	printf("Total amount of bytes received: %d\n", counter);
		
	err=CloseHandle(writetofile);
	if (err==NULL){
		printf("in close writetohandle error: %d.", GetLastError);
		exit(5);
	}
}


	//====================================================
	//	START:	Download Function 
	//====================================================
void download(SOCKET &downloadSocket)
{
	HANDLE	filehandle;
	char moveBuffer[1100];
	char sNumRead[3];	
	unsigned long numRead;	//real size in ul returned by ReadFile
	int j=3, err, counter=0;	//j is the length of the size of buffer size:100

	//get the file handle for downloading
	filehandle=CreateFile(
		loadDir,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (filehandle==INVALID_HANDLE_VALUE){
		printf("in READHANDLE error: %d.\n", GetLastError);
		exit(1);
	}

	// loop to send buffer by buffer of file piece
	do {
		err=ReadFile(
			filehandle,
			moveBuffer,
			100,
			&numRead,
			NULL);

		if (err==NULL){
			printf("in ReadFile error: %d.\n", GetLastError);
			exit(3);
		}
		
		_ultoa(numRead, sNumRead, 10);	// convert ul to char[]
		
		err=send(downloadSocket, sNumRead, j,0);// send number(char[]) of bytes read
		if (err==SOCKET_ERROR) {
			printf("In send numRead, Client error: %d\n",WSAGetLastError());
			exit(10);
		}
		
		if (numRead!=0) {
			err=send(downloadSocket, moveBuffer, numRead,0);// send buffer
			if (err==SOCKET_ERROR) {
				printf("In send moveBuffer, Client error: %d\n",WSAGetLastError());
				exit(11);
			}
			counter=counter+(int)numRead;
		}
	}while(numRead!=0);	// read and send until there is nothing in the file

	printf("\nThread: %d\n", ThreadID);
	printf("Total amount of bytes transfered: %d\n", counter);

	//download finished, close the file handle
	err=CloseHandle(filehandle);
	if (err==NULL){
		printf("in close filehandle error: %d.", GetLastError);
		exit(4);
	}
}


	//====================================================
	//	START:	load Function 
	//	This function tells the program what to do, download
	//	or upload or quit, which file or folder to work on. 
	//====================================================
void load(SOCKET &loadSocket)
{
	int err, idLen=0, itemIndex=0;
	double j=0;
	char sIdLen[3], *stopString;
	char itemId[10], itemName[50], itemDir[50], itemComm[100];
	char uploadFileName[50];

	do{	err=recv(loadSocket,sIdLen, 3,0);	//receive length of the itemId
		if (err==SOCKET_ERROR) {
			printf("In recv numRead, THREAD Error: %d\n",WSAGetLastError());
			getchar();
			exit(6);
		}

		j=strtod(sIdLen, &stopString);	// convert the message to int type
		idLen = strtoul( sIdLen, &stopString, 10 );
			
		err=recv(loadSocket, itemId, idLen, 0);	//receive the itemId
		if (err==SOCKET_ERROR) {
			printf("In recv numRead, THREAD Error: %d\n",WSAGetLastError());
			getchar();
			exit(7);
		}
		
		j=strtod(itemId, &stopString);//convert the message to int type
		itemIndex=strtoul(itemId, &stopString, 10);

		// go out of loop
		if (itemIndex ==0) break; // 0 means the client is quiting

		// locate the current item to be the one sended by the client
		currItem=items[itemIndex];
		currItem.getRecord(itemId, itemName, itemDir, itemComm);
		strcpy(loadDir,itemDir);

		// if current item is a file, do download
		if(strcmp(itemComm, "file")==0) {
			download(loadSocket);
		
		// if current item is a folder, do upload
		} else if(strcmp(itemComm, "folder")==0) {
			err=recv(loadSocket,sIdLen, 3,0); // recv the length of the uploadFileName to be received
			if (err==SOCKET_ERROR) {
				printf("In recv numRead, THREAD Error: %d\n",WSAGetLastError());
				getchar();
				exit(6);
			}

			j=strtod(sIdLen, &stopString);
			idLen = strtoul( sIdLen, &stopString, 10 );

			// recv the uploadFileName
			err=recv(loadSocket, uploadFileName, idLen, 0);
			if (err==SOCKET_ERROR) {
				printf("In recv numRead, THREAD Error: %d\n",WSAGetLastError());
				getchar();
				exit(7);
			}

			// for some unknown reason the received file name always have
			// some junk and "folder" appended to it. The size of the junk
			// is the declared size of uploadFileName, 50, less the size of 
			// the file name. I have checked that idLen is correct here, 
			// and in the client, the file name send is correct as well.
			// So to fix the file name:
			uploadFileName[idLen]=NULL;

			//append file name to loadDir
			strcpy(loadDir, strcat(loadDir,uploadFileName));
			upload(loadSocket);
		} else;
	}while(1);
}


	//====================================================
	//	START:	Functions for itemRecord Maintainance
	//====================================================
void create (void) 
{
	char itemId[10];
	for (int i=1; i<=10; i++) {
		_itoa(i, itemId, 10);
		items[i].iniRecord(itemId);
	}
}

void store (void)
{
	FILE *storeHandle;
	storeHandle=fopen("g:\\classwork\\w_ftp\\p_ftp\\record.txt", "w");
	char itemId[10], itemName[50], itemDir[50], itemComm[100];
	for (int i=1; i<=10; i++) {
		items[i].getRecord(itemId, itemName, itemDir, itemComm);
		fprintf(storeHandle, "%s\n%s\n%s\n%s\n", itemId, itemName, itemDir, itemComm);	
	}
	fclose(storeHandle);

	printf("\nFile Stored.\n");
}

void retrieve (void)
{
	FILE *retrieveHandle;
	char itemId[10], itemName[50], itemDir[50], itemComm[100];
	retrieveHandle=fopen("g:\\classwork\\w_ftp\\p_ftpadn\\record.txt", "r");
	
	for (int i=1; i<=10; i++) {
		fscanf(retrieveHandle, "%s\n%s\n%s\n%s\n", itemId, itemName, itemDir, itemComm);
		items[i].setRecord(itemId, itemName, itemDir, itemComm);
	}
	fclose(retrieveHandle);
	printf("\nFile Retrieved.\n");
}
	//====================================================
	//	END:	Functions for itemRecord Maintainance
	//====================================================

</pre>
      <pre>&nbsp;</pre>
      <!-- #EndEditable --></td>
    <td width="1%" height="6">&nbsp;</td>
  </tr>
  <tr> 
    <td width="1%" height="2">&nbsp;</td>
    <td width="98%" height="2">&nbsp;</td>
    <td width="1%" height="2">&nbsp;</td>
  </tr>
</table>
</body>
<!-- #EndTemplate --></html>
