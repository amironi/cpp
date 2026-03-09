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
int exit[] = {10,9,9,8,7,6,5,4,2,1};

void findMaxGuests(int arrl[], int exit[], int n) 
{ 
   // Sort arrival and exit arrays 
   // guests_in indicates number of guests at a time 
   map<int, int> time_;
   
   int guests_in = 1, max_guests = 1, time = arrl[0]; 
   int i = 1, j = 0; 
  
   // Similar to merge in merge sort to process 
   // all events in sorted order 
   while (i < n && j < n) 
   { 
       time_[arrl[i]]++;
       time_[exit[i]]--;
   } 
  
   cout << "Maximum Number of Guests = " << max_guests 
        << " at time " << time; 
} 