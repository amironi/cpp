// organization.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
//#include "string.h"
#include "conio.h"
#include <iostream>
#include <time.h>
#include "memlk.h"
#include "Item.h"

using namespace std;


int maxSubArraySum(int a[], int size) 
{ 
    int max_so_far = INT_MIN, max_ending_here = 0; 
  
    for (int i = 0; i < size; i++) 
    { 
        max_ending_here = max_ending_here + a[i]; 
        
        max_so_far = max(max_so_far, max_ending_here) 
        max_ending_here = max(max_ending_here, 0) 
        
    } 
    return max_so_far; 
} 