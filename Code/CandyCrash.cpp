/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

void cc(char* aStr)
{
    char* pW = aStr;
    char* pR = aStr;
    char prevChar = 0;
    int prevCharCounter = 1;
    
    while( *pR )
    {
        
        if( *pR == prevChar )
        {
            prevCharCounter++;
        }
        else
        {
            if( prevCharCounter >= 3 )
            {
                pW -= prevCharCounter;
                prevCharCounter = 1;
                
                char* temp = pW;
                while( (temp > aStr) && (*(temp - 1) == *pR) )
                {
                    prevCharCounter++;
                    temp--;
                }
            }
            else
            {
                prevCharCounter = 1;
            }
            
        }
        prevChar = *pR;
        
        if( *pW != *pR )
        {
           *pW = *pR;
        }

        pW++;
        pR++;
    }
    
    if( prevCharCounter >= 3 )
    {
       pW -= prevCharCounter;
    }
    
    *pW = 0;
}

int main()
{
    char s[]="ABBBACCACCCAAC";

    
    cout<<s<<endl;
    cc( s );
    cout<<s<<endl;
    
    return 0;
}