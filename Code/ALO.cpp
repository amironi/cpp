/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <chrono>


using namespace std;

//////////////brute_forse_solution///////////////

bool isValidString ( const string& str )
{
  if (std::count (str.begin (), str.end (), 'L') > 2 )
      return true;

   if (str.find ("AA") != -1)
      return true;

  return false;
}

int brute_forse_solution( int nMax, const string& str = "", bool v = 0 )
{
  if (str.length() == nMax)
  {
      if (v)
          {
              //cout << str << endl;
              return 1;
          }
      return 0;
  }

  return
    brute_forse_solution( nMax, str + "A", v || isValidString (str + "A")) +
    brute_forse_solution( nMax, str + "L", v || isValidString (str + "L")) +
    brute_forse_solution( nMax, str + "O", v);
}


//////////////recursive_olution///////////////

int recursive_solution( const int N,  const string& aList = "" )
{
    if( isValidString( aList ) )
    {
        return pow(3,N - aList.size() );
    }
    
    if ( aList.length() == N )
    {
        return 0;
    }

    return  recursive_solution( N, aList + 'A' ) +
        recursive_solution( N, aList + 'L' ) +
        recursive_solution( N, aList + 'O' );
}




//Given 3 characters a, b, c. Find the number of strings of length n that can be formed from these 3 characters. Given that : we can use ‘a’ as many times as we want, ‘b’ maximum once, and ‘c’ maximum twice.

int abc( int N, int b=0, int c=0 )
{

    if( b == 3 || c == 2)
    { 
      return 0;
    }

    if( N == 0 )
    {
        return 1;
    }

    return abc( N-1, b, c ) +   //state a
    abc( N-1, b+1, c ) +  //state b
    abc( N-1, b, c+1 );// state c
}

int Amir( int N, int A=0, int L=0 )
{
    if( L == 3 || A == 2)
    {
        return pow(3, N);
    }
    
    if( N == 0 )
    {
        return 0;
    }

    return Amir( N-1, A+1, L ) +   //state A
    Amir( N-1, 0, L + 1 ) +  //state L
    Amir( N-1, 0, L );// state O

}


int LIS( int* a, int n, int x = 0)
{
    if( n == 0 ) return 0;
    
    
    if( *a <= x )  return LIS(a+1,n-1,x);

    return max(
      1 + LIS(a+1,n-1,*a),
      LIS(a+1,n-1,x) );
}

int main()
{
    int n = 8;
    
    string s;
    
    cout<<"n="<<n<<endl;
    
    auto t1 = chrono::high_resolution_clock::now();
    int n_MS = mashine_state_olution( n );
    auto t2 = chrono::high_resolution_clock::now() - t1;
    cout<<"MS="<<n_MS<<" time="<<t2.count()<<endl;
    
    t1 = chrono::high_resolution_clock::now();
    int n_amir = Amir( n );
    t2 = chrono::high_resolution_clock::now() - t1;
    cout<<"NA="<<n_amir<<" time="<<t2.count()<<endl;


    t1 = chrono::high_resolution_clock::now();
    int n_REC = recursive_solution( n, s );
    t2 = chrono::high_resolution_clock::now() - t1;
    cout<<"RS="<<n_REC<<" time="<<t2.count()<<endl;
    
    t1 = chrono::high_resolution_clock::now();
    int n_BF = brute_forse_solution( n );
    t2 = chrono::high_resolution_clock::now() - t1;
    cout<<"BF="<<n_BF<<" time="<<t2.count()<<endl;
    
    return 0;
}



