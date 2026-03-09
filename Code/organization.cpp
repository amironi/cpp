// organization.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
//#include "string.h"
#include "conio.h"
#include <iostream>
#include <time.h>
#include "memlk.h"
#include "Item.h"

using namespace std;



bool isLess(char* a, char* b)
{
        return (strcmp(a, b) < 0);
}

class Item
{
public:

        char* mName;
        Item* mNextSibling;
        Item* mParent;
        Item* mFirstChild;

        Item(char* aName)
        {
                int len = strlen(aName) + 1;
                mName = new char[len];
                strncpy_s(mName, len, aName, len);

                mParent = nullptr;
                mNextSibling = nullptr;
                mFirstChild = nullptr;
        }

        ~Item()
        {
                delete[] mName;

                while (mFirstChild)
                {
                        Item* next = mFirstChild->mNextSibling;

                        delete mFirstChild;

                        mFirstChild = next;
                }
        }

        Item* find(char* aName)
        {

                if (strcmp(mName, aName) == 0)
                {
                        return this;
                }

                for (auto it = mFirstChild; it != nullptr; it = it->mNextSibling)
                {
                        Item* item = it->find(aName);

                        if (item)
                        {
                                return item;
                        }
                }

                return nullptr;
        }

        void print(int offset = 0)
        {
                const char* mTabs = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

                for (auto it = mFirstChild; it != nullptr; it = it->mNextSibling)
                {
                        printf("%.*s%s\n", offset, mTabs, it->mName);
                        it->print(offset+1);
                }
        }

        void printPath()
        {
                if (mParent)
                {
                        mParent->printPath();
                        cout << "->";
                }

                cout << mName;
        }

        void removeChild(char* aName)
        {
                auto it = mFirstChild;

                Item* prev = nullptr;

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
                        cout << aName << " not found!" << endl;

                        return;
                }
                if (prev)
                {
                        prev->mNextSibling = it->mNextSibling;
                }
                else
                {
                        mFirstChild = it->mNextSibling;
                }
        }

        Item* addChild(Item* aChild)
        {
                aChild->mParent = this;
                aChild->mNextSibling = nullptr;

                if (!mFirstChild)
                {
                        mFirstChild = aChild;

                        return aChild;
                }


                if (isLess(aChild->mName, mFirstChild->mName)) {

                        aChild->mNextSibling = mFirstChild;

                        mFirstChild = aChild;

                        return aChild;
                }

                Item* current;

                Item* prev = nullptr;

                for (current = mFirstChild ; current; current = current->mNextSibling)
                {
                        if (isLess(aChild->mName, current->mName ) )
                        {
                                break;
                        }

                        prev = current;
                }

                if (prev)
                {
                        prev->mNextSibling = aChild;

                        aChild->mNextSibling = current;

                }

                return aChild;
        }
};


Item root("");

void changeMgr(char *wrk, char *mgr)
{
        printf("\n change manager\n");

        Item* empl = root.find(wrk);

        if (!empl)
        {
                cout << wrk << " not found!" << endl;

                return;
        }


        Item* manager = root.find(mgr);

        if (!manager)
        {
                manager = root.addChild(new Item(mgr));
        }

        empl->mParent->removeChild(wrk);

        manager->addChild(empl);
}


void printMgrHire(char* emp)
{
        printf("\n print managerial hierarchy\n");

        Item* item = root.find(emp);

        if (!item)
        {
                cout << emp << " not found!" << endl;

                return;
        }

        item->printPath();

        cout << endl;
}

void addEmp(char *wrk, char *mgr)
{
        printf("\n add employee\n");

        Item* manager = root.find(mgr);

        if (!manager)
        {
                manager = root.addChild(new Item(mgr));
        }

        manager->addChild(new Item(wrk));

}

void printOrg()
{
        printf("\n print organization\n");

        printf("--------\n");
        root.print(0);
        printf("--------\n");

}

void delEmp(char* emp)
{
        printf("\n delete employee\n");

        Item* item = root.find(emp);

        if (!item)
        {
                cout << emp << " not found!" << endl;

                return;
        }
        item->mParent->removeChild(emp);

        delete item;
}

int menu()
{
        printf("\n 1. change manager");
        printf("\n 2. print managerial hierarchy");
        printf("\n 3. add employee");
        printf("\n 4. print organization");
        printf("\n 5. delete employee");
        printf("\n 6. exit");
        printf("\n What would you like to do: ");
        int i = _getch() - '0';
        return i;
}

void test()
{
        addEmp("amir", "asi");
        addEmp("dima", "asi");
        addEmp("vitaly", "asi");
        addEmp("elad", "amir");
        addEmp("goren", "amir");
        addEmp("david", "amir");

        changeMgr("asi", "pole");

        printMgrHire("elad");

        changeMgr("pole", "shlomi");

        printMgrHire("elad");

        printOrg();


}
int main(int argc, char* argv[])
{

        initml();
        char inp[100];
        char inp2[100];
        int i = 0;

        test();

        while (true)
        {
                i = menu();
                switch (i)
                {
                case 1:
                        printf("\nEnter employee: ");
                        scanf("%s", inp);
                        printf("\nEnter new manager: ");
                        scanf("%s", inp2);
                        changeMgr(inp, inp2);
                        break;
                case 2:
                        printf("\nEnter employee: ");
                        scanf("%s", inp);
                        printMgrHire(inp);
                        break;
                case 3:
                        printf("\nEnter new employee: ");
                        scanf("%s", inp);
                        printf("\nEnter manager: ");
                        scanf("%s", inp2);
                        addEmp(inp, inp2);
                        break;
                case 4:
                        printOrg();
                        break;
                case 5:
                        printf("\nEnter employee: ");
                        scanf("%s", inp);
                        delEmp(inp);
                        break;
                case 6:
                        return 0;
                }
        }
        return 0;
}
