/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

void f(int* s, int *e)
{
    int* w = s;
    int prev = 0;
        
    while( s < e )
    {
        if( *s == 0 )
        {
            s++;
            continue;
        }

        *w = *s
                
        if( prev == *s )

            prev = 0;
            *w *= 2; 
        }
        else        
            prev = *s;

        s++;
        w++;
    }
    
    while( w < e )
    {
       *w = 0; w++;
    }
}

int main()
{
    char s[]="ABBBACCACCCAAC";

    
    cout<<s<<endl;
    f( s );
    cout<<s<<endl;
    
    return 0;
}