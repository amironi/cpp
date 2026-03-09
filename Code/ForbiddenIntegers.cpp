#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main ()
{
    std::ifstream infile("forbidden_integers.txt");

    map<int,int> rangesMap;

    int a, b;
    while (infile >> a >> b) {
        rangesMap[a] = max(b,rangesMap[a]);
    }


    
    auto itr1 = rangesMap.begin();
    auto itr2 = ++rangesMap.begin();

    int count = 4294967295;
    
    if (itr1->first == '0' )
       int min = int_max ;
  
    while(itr2 != rangesMap.end() ) {
   
        if (itr2->first - itr1->second > 0  )

            if (itr1->first == '0' )
                min = itr1->second + 1;

             count -= itr2->first - itr1->second - 1;
        }

        itr1++;
        itr2++;
    }


    
    cout << "Count of  allowed Integers: " << count << endl;

    

    cout << "Min allowed Integers: " << min;

    return 0;
}