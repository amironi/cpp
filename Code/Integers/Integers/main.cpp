#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

int main ()
{
    std::ifstream infile("forbidden_integers.txt");

	if(!infile.is_open()) return -1;

    map<int,int> rangesMap;

	std::string line;
	while (getline(infile, line))
	{
		istringstream iss(line);
		int a, b;
		iss >> a >> b;

		rangesMap[a] = abs(b);
	}

    map<int,int> new_map;

    map<int,int>::iterator itr = rangesMap.begin();
    new_map[itr->first] = itr->second ;
    
    map<int,int>::iterator itr2 = new_map.find( itr->first );
    itr++;

    for( ; itr != rangesMap.end() ; itr++ ) {
        if (itr2->second <= itr->first )
            itr2->second = itr->second;
        else {
            new_map[itr->first] = itr->second ;
            itr2 = new_map.find( itr->first );
          }
    }

    int count = 4294967295;

    for( map<int,int>::iterator i = new_map.begin(); i != new_map.end() ; i++ )
        count -= i->second - i->first;

    cout << "Count of  allowed Integers: " << count << endl;

    int min = 0 ;
    if (new_map.begin()->first == '0' )
        min = new_map.begin()->second + 1;

    cout << "Min allowed Integers: " << min;

    return 0;
}