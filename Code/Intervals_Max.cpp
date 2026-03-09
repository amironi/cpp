#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

typedef pair<int,int> Point;

//comp: O(n*lg(n))
vector<Point> FindMaxPeekIntervals( vector<Point>& intervals ) {
    map<int, int > sorted_points;
    map<int, list<func> > rooms;

    // Fill the helper sorted_points
    for(auto i : intervals) {
        sorted_points[i.first]++;
        sorted_points[i.second]--;

        rooms[i.first].push_back( malloc() );
        rooms[i.second].push_back( free() );
    }

    //Find Max peek
    int max_peek = 0;
    int counter = 0;
    for(auto i : sorted_points) {
        counter += i.second;
        max_peek = max(max_peek, counter);
    }

    //Assign
    for(auto i : rooms) {
        runAll(i.second);
    }

    //Fill Max vector ranges
    counter = 0;
    vector<Point> ret;
    int start = 0;

    for (auto i : sorted_points) {
        counter += i.second;
        
        if(max_peek == counter) {
            start = i.first;
        }  else if (start != 0)  {
            ret.push_back( Point(start, i.first) );
            start = 0;
        }
    }

    return ret;
}



set<Point> findMaxGuests(set<Point>& intervals)  {
         // Sort arrival and exit arrays 
   // guests_in indicates number of guests at a time 
   int guests_in = 1, max_guests = 1, time = arrl[0]; 
   int i = 1, j = 0; 
  
   // Similar to merge in merge sort to process 
   // all events in sorted order 
   while (i < intervals.size() && j < n) 
   { 
      // If next event in sorted order is arrival, 
      // increment count of guests 
      if (arrl[i] <= exit[j]) 
      { 
          guests_in++; 
  
          // Update max_guests if needed 
          if (guests_in > max_guests) 
          { 
              max_guests = guests_in; 
              time = arrl[i]; 
          } 
          i++;  //increment index of arrival array 
      } 
      else // If event is exit, decrement count 
      {    // of guests. 
          guests_in--; 
          j++; 
      } 
   } 
  
   cout << "Maximum Number of Guests = " << max_guests 
        << " at time " << time; 
} 

int main (int argc, char **argv) {
    //9:00----10:00
    //  9:30---------------------------- 14:00
    //              11:00------13:00
    //                  12:00--------------------15:00
    
    vector<Point> v;
    v.push_back(Point(900, 1000));
    v.push_back(Point(930, 1400));
    v.push_back(Point(1100, 1300));
    v.push_back(Point(1100, 1500));
    v.push_back(Point(1200, 1500));
    v.push_back(Point(1330, 1500));

    v = FindMaxPeekIntervals( v );

    return 0;
}