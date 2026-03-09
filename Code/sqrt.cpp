/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <string>

#define ABS(x) (x > 0 ? x : -1 * x )

using namespace std;

double sqrt2(double num, double x=1)
{
    double d = x*x - num;
    if( ABS(d) < 0.00000000001 ) 
        return x;
    
    double g=  x  + num / x;
    
    return sqrt2(num, g/2);
}

double sqrt3(double num, double x=1)
{
    double d = x*x*x - num;
    if( ABS(d) < 0.00000000001 ) 
        return x;
    
    double g=  x  + num / (x * x);
    
    return sqrt3(num, g/2);
}

int main(int argc, char * argv[])
{
    srand(time(NULL));

    cout << sqrt2(9) << endl;
    cout << sqrt2(2) << endl;
    cout << sqrt2(2.5) << endl;


    cout << sqrt3(27) << endl;
    cout << sqrt3(9) << endl;
    cout << sqrt3(10.5) << endl;
    
    return 0;
}





