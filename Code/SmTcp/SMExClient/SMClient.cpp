#include "stdafx.h"
#include "SMClient.h"
#include <sstream>

#include "..\Common\IntBuffer.h"

SMClient::SMClient()
{
}


SMClient::~SMClient()
{
}

int SMClient::onConnect(SOCKET aConnectSocket)
{
	int ret = 0;
	//int com;
	string com;
	while (ret == 0)
	{
		cout << "Enter Text: (x for exit)" << endl;
		getline(cin, com);

		if (com == "x" || com == "X")
		{
			ret = 1;
		}
		else
		{
			IntBuffer buff;

			stringToBuffer(com, buff);
			if (buff.size() == 0)
			{
				cout<<"Invalid input!"<<endl;
			}
			else
			{
				if (sendBuffer(aConnectSocket, buff.mBuffer) == SOCKET_ERROR)
				{
					ret = -1;
				}
				else
				{
					buff.clear();
					ret = reciveBuffer(aConnectSocket, buff.mBuffer);
					if (ret == 0)
					{
						list<int> retList;
						buff.getIntList(retList);

						cout << "Ret val: ";
						for (auto i : retList)
						{
							cout << i << " ";
						}
						cout << endl;

					}
				}
			}
		}
	}

	return ret;
}

void SMClient::stringToBuffer(string aStr, IntBuffer& aBuff)
{
	stringstream stream(aStr);
	
	while (1)
	{
		int n;
		stream >> n;
		if (!stream)
		{
			break;
		}
		aBuff.addInt(n);
	}
}