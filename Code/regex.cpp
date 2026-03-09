/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

bool Check(string s, string regex)
{
   if( s.length() == 0 && regex.length() == 0)
        return true;
    
   if( s.length() == 0 || regex.length() == 0)
         return false;

    if( regex[0] == '?' || regex[0] == s[0] )
        return Check(s.substr(1), regex.substr(1));

    if ( regex[0] == '*')
        {
            return Check(s.substr(1), regex.substr(1)) ||  
                Check(s.substr(1), regex);  
        }
        
    return false;
}

int
main ()
{
  //cout<<"Hello World";


  cout << Check ("amir", "??ir") << endl;

  return 0;
}