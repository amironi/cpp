#include <iostream>
#include <string>

using namespace std;

//int v[] = {2, 3, 1 ,1 ,2, 1, 2, 2, 2 ,1,1 ,3, 1};
//int v[] = {1, 2, 3 ,1 ,2, 1, 1, 2, 3 ,1,1 ,3, 1};
//int v[] = {3, 3, 3 ,3 ,3, 3, 1, 2, 2 ,1,1 ,1, 1};

const int N  = 50;
int v[N];


void sort3(int v[], int n)
{
    int w1 = 0;
    int w3 = n-1;
    
    for(int r = 0; r <= w3 ; r++)
    {
        if (v[r] == 3)
        {
            swap(v[r--], v[w3--]);
        }
        else if (v[r] == 1)
        {
            swap(v[r], v[w1++]);
        }
    }
}

int main(int argc, const char * argv[])
{
    
    for(int i =0 ; i<N ; i++)
        v[i] = i%3+1;
    
    v[5] = v[15] = 1;
    
    sort3(v,N);
    
    for(int i =0 ; i<N ; i++)
        printf("%d",v[i]);
    
    return 0;

}
