/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <string>

#define ABS(x) (x > 0 ? x : -1 * x )

using namespace std;

bool isSumExist(int* arr, int n, int sum)
{
    if( sum == 0) 
        return true;
    
    if(n == 0)
        return false;

    return isSumExist(arr +1,  n-1, sum - *arr) ||
           isSumExist(arr +1,  n-1, sum );
}

int maxSum(int* arr, int n)
{
    int sum = 0;
    int maxsum = 0;
    for(int i = 0 i < n; i++) {
        sum += arr[i];
        maxsum = max(maxsum, sum);
        if( sum < 0) {
            sum = 0;
        }
    }
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





