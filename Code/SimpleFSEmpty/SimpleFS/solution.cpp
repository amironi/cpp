
// SimpleFS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>

//2231
using namespace std;

class FsItem
{
public:
	FsItem(char* aName, bool aIsFolder, FsItem* aParent = nullptr)
	{
		int len = strlen(aName) + 1;
		mName = new char[len];
		strncpy_s(mName, len, aName, len);

		mParent = aParent;
		mNextSibling = nullptr;
		mFirstChild = nullptr;
		mIsFolder = aIsFolder;
	}

	~FsItem()
	{
		while (mFirstChild)
		{
			FsItem* next = mFirstChild->mNextSibling;
			delete mFirstChild;
			mFirstChild = next;
		}

		delete[] mName;
	}

	void print( int offset )
	{
		for (auto it = mFirstChild; 
			it != nullptr; 
			it = it->mNextSibling)
		{
			printf("%.*s%s\n", offset, mTabs, it->mName);
			it->print(offset + 1);
		}
	}

	void printPath()
	{
		if (mParent && mParent != &mRoot)
		{
			mParent->printPath();
			cout << "\\";
		}
		
		cout << mName;
	}

	FsItem* find(char* aName, bool aFolderOnly )
	{
		if (!aFolderOnly || mIsFolder)
		{
			if (strcmp(mName, aName) == 0)
			{
				return this;
			}

			for (auto it = mFirstChild; it != nullptr; it = it->mNextSibling)
			{
				FsItem* f = it->find(aName, aFolderOnly);
				if (f)
				{
					return f;
				}
			}
		}
		return nullptr;
	}

	void removeChild(char* aName)
	{
		auto it = mFirstChild;
		FsItem* prev = nullptr;
		

		for (; it != nullptr; it = it->mNextSibling)
		{
			if (strcmp(it->mName, aName) == 0)
			{
				break;
			}
			prev = it;
		}

		if (!it)
		{
			cout << aName<<" not found!" << endl;
		}
		else
		{
			if (prev)
			{
				prev->mNextSibling = it->mNextSibling;
			}
			else
			{
				mFirstChild = it->mNextSibling;
			}
		}
	}

	void addChild(FsItem* aNewChild)
	{
		auto it = mFirstChild;
		FsItem* prev = nullptr;
		for (; it != nullptr; it = it->mNextSibling)
		{
			int comp = strcmp(it->mName, aNewChild->mName);
			if (comp == 0)
			{
				cout << it->mName << " alredy exist!" << endl;
				return;
			}
			else if (comp > 0)
			{
				break;
			}
			prev = it;
		}

		if (prev)
		{
			auto next = prev->mNextSibling;
			prev->mNextSibling = aNewChild;
			aNewChild->mNextSibling = next;
		}
		else
		{
			aNewChild->mNextSibling = mFirstChild;
			mFirstChild = aNewChild;
		}

		aNewChild->mParent = this;
	}
	
	void addChild(char* aName, bool aIsFolder)
	{
		addChild(new FsItem(aName, aIsFolder));
	}

	void moveChildren(FsItem* aNewFolder)
	{
		while (mFirstChild)
		{
			FsItem* next = mFirstChild->mNextSibling;
			aNewFolder->addChild(mFirstChild);
			mFirstChild = next;
		}
		
	}


	static FsItem mRoot;
	static char* mTabs;
	char* mName;
	FsItem* mNextSibling;
	FsItem* mParent;
	FsItem* mFirstChild;
	bool mIsFolder;
	
};

FsItem FsItem::mRoot = FsItem("root", true);
char* FsItem::mTabs = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

void displayPath(char* aName)
{
	FsItem* item = FsItem::mRoot.find(aName, false);
	if (!item)
	{
		cout << aName << " not found!" << endl;
	}
	else
	{
		item->printPath();
		cout << endl;
	}
}

void move(char* aName, char* aNewFolder)
{
	FsItem* item = FsItem::mRoot.find(aName, false);
	if (!item)
	{
		cout << aName << " not found!" << endl;
	}
	else
	{
		FsItem* newParent = FsItem::mRoot.find(aNewFolder, true);
		if (!newParent)
		{
			cout << aNewFolder << " not found!" << endl;
		}
		else
		{
			item->mParent->removeChild(aName);
			newParent->addChild(item);
		}
	}
}

void createFile(char* aName, char* aFolder)
{
	FsItem* parent = FsItem::mRoot.find(aFolder, true);
	
	if (parent == nullptr)
	{
		cout << "Diectory " <<aFolder<<" not found!" << endl;
	}
	else
	{
		parent->addChild(aName, false);
	}
}

void createFolder(char* aName, char* aFolder)
{
	FsItem* parent = FsItem::mRoot.find(aFolder, true);

	if (parent == nullptr)
	{
		cout << "Diectory " << aFolder << " not found!" << endl;
	}
	else
	{
		parent->addChild(aName, true);
	}
}

void remove(char* aName, bool aRecurcive)
{
	FsItem* item = FsItem::mRoot.find(aName, false);
	if (!item)
	{
		cout << aName << " not found!" << endl;
	}
	else
	{
		item->mParent->removeChild(aName);

		if (!aRecurcive)
		{
			item->moveChildren(item->mParent);
		}
		
		delete item;
	}
}

void printFS()
{
	printf("--------\n");
	FsItem::mRoot.print(0);
	printf("--------\n");
	
}

int main()
{
	char c;
	bool cont = true;
	while (cont)
	{
		cout << "1. diaply full path" << endl;
		cout << "2. move" << endl;
		cout << "3. create file" << endl;
		cout << "4. create folder" << endl;
		cout << "5. delete" << endl;
		cout << "6. print FS" << endl;
		cout << "Enter your choise: (x for exit)";

		c = _getch();
		
		cout << endl;

		if (c == EOF)
		{
			cont = false;
		}

		char ch1[500];
		char ch2[500];

		switch (c)
		{
		case '1':
			cout << "name: " << endl;
			cin >> ch1;
			displayPath(ch1);
			break;
		case '2':
			cout << "name: " ;
			cin >> ch1;
			cout << endl << "new folder: " ;
			cout << endl;
			cin >> ch2;
			move(ch1, ch2);
			break;
		case '3':
			cout << "name: ";
			cin >> ch1;
			cout << endl << "folder: ";
			cin >> ch2;
			cout << endl;
			createFile(ch1, ch2);
			break;
		case '4':
			cout << "name: ";
			cin >> ch1;
			cout << endl << "folder: ";
			cin >> ch2;
			cout << endl;
			createFolder(ch1, ch2);
			break;
		case '5':
			cout << "name: ";
			cin >> ch1;
			cout << endl << "recurcive? [y/n]:";
			cin >> ch2;
			cout << endl;
			remove(ch1, ch2[0] == 'y' );
			break;
		case '6':
			printFS();
			break;
		case 'x':
			cont = false;
			break;
		};
	}
    return 0;
}

SimpleFS.cpp
Open with
Displaying SimpleFS.cpp.