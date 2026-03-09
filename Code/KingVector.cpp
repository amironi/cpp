
#include <iosrteam>
#include <stdio.h>

int arr1[] = {1,2,3,4,5,6,7,8,9,10};
int arr2[] = {10,9,9,8,7,6,5,4,2,3,1,2,3,4,5,6,7,8,9,10};

void OddFunc(int* array, int n)
{ 
    int sign = 1;

    for(int i = 1 ; i <n ; ++i)
    {
        sign *= -1;
        
        if(  sign * (array[i-1] - array[i]) > 0 )
            swap(array[i-1],array[i]);
    }
    
}
int main()
{
    OddFunc();
}


//	{[*}((*{*}){}({{*})
