//
//  Isrta
//  Client
//
//  Created by Amir Mironi on 10/23/17.
//  Copyright © 2017 Amir Mironi. All rights reserved.
//

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>

using namespace std;


int arr1[] = {1,2,3,4,5,6,7,8,9,10};
int arr2[] = {10,9,9,8,7,6,5,4,2,3,1,2,3,4,5,6,7,8,9,10};

void OddFunc(int* array, int n)
{
    for(int i = 1 ; i <n ; ++i)
    {
        int sign = i%2 ? 1 : -1;
        
        if(  sign * (array[i-1] - array[i]) > 0 )
            swap(array[i-1],array[i]);
    }
    
}


void findMaxRange(list<pair<int, int>>& timesList)
{
    map<int, int> startMap;
    map<int, int> endMap;
    
    for (auto i : timesList) {
        
        startMap[i.first]++;
        startMap[i.second];
        
        endMap[i.first];
        endMap[i.second]++;
    }
    
    int counter = 0;
    int max_counter = 0;
    pair<int, int> max_range;
    
    auto s = startMap.begin();
    auto e = endMap.begin();
		
    while (s != startMap.end()) {
        
        for (auto i = 0; i < s->second; i++) {
            if ( counter++ == max_counter  ) {
                max_range.first = s->first;
                max_counter = counter;
            }
        }
        
        for (auto i = 0; i < e->second; i++) {
            if (counter-- == max_counter) {
                max_range.second = e->first;
            }
        }
        
        s++; e++;
    }
    
    cout << "Max counter: " << max_counter << endl;
    cout << "Range Start: " << max_range.first << endl;
    cout << "Range end: " << max_range.second << endl;
}

class Room
{
public:
    Room() {}
    Room(int x ) { id =x; }
    int id;
};

map<int, list<Room*> > endTimeToRoom;
typedef  list<Room*> listRoomType;


void tryToFreeRoomsThatEnded(
                             list<pair<int, int>>::iterator start,
                             list<pair<int, int>>::iterator end,
                             listRoomType& freeRooms){
    
    
    if (endTimeToRoom.size() == 0) return;
    
    if (start->first < endTimeToRoom.begin()->first)
        return;
    
    for( auto i: endTimeToRoom.begin()->second)
        freeRooms.push_front(i);

    endTimeToRoom.erase(endTimeToRoom.begin());

}


void assignRoom(
                list<pair<int, int>>::iterator start,
                list<pair<int, int>>::iterator end,
                listRoomType& freeRooms)
{
    if (start == end)
        // Done
        return;
    
    
    tryToFreeRoomsThatEnded(start,end, freeRooms);
    
    //get room the list
    if (freeRooms.size() > 0) {
        
        endTimeToRoom[start->second].push_back(freeRooms.front());
        
        cout << "Meeting Start: " << start->first << "-"<<start->second << " got " << freeRooms.front()->id <<  endl;
    
        freeRooms.pop_front();
        
        assignRoom(++start, end, freeRooms);
        
        return;
    }
    

}

int main (int argc, char **argv)
{
    //Q1
    OddFunc(arr1,10);
    OddFunc(arr2,17);

    for(int i = 0 ; i <10 ; ++i)
    {
        cout << arr1[i] << " ";
    }
    cout << endl;
    
    for(int i = 0 ; i <17 ; ++i)
    {
        cout << arr2[i] << " ";
    }
    cout << endl;
    
    list<pair<int, int>> timesList;
    //9:00----10:00
    //  9:30---------------------------- 14:00
    //              11:00------13:00
    //                  12:00--------------------15:00
    
    //1100 = 11:00
    
    timesList.push_back(pair<int, int>(900, 1000));
    timesList.push_back(pair<int, int>(930, 1400));
    timesList.push_back(pair<int, int>(1100, 1300));
    timesList.push_back(pair<int, int>(1100, 1500));
    timesList.push_back(pair<int, int>(1200, 1500));
    timesList.push_back(pair<int, int>(1330, 1500));
    
    //Q2
    findMaxRange(timesList);

    listRoomType freeRooms;

    freeRooms.push_back(new Room(1));
    freeRooms.push_back(new Room(2));
    freeRooms.push_back(new Room(3));
    freeRooms.push_back(new Room(4));

    //Q3
    assignRoom(
               timesList.begin(),
               timesList.end(),
               freeRooms);
    
    
}    //
