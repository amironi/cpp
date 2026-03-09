
#include "stdafx.h"
#include <stdio.h>

#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <map>
#include <windows.h>
#include <stdlib.h>
#include <assert.h>     /* assert */

#pragma comment(lib, "Ws2_32.lib")

#define NDEBUG 

using namespace std;

struct validateTime_float {
	void operator()(float T) {
		assert(T >= 0.00 && T < 24.00);
	}
};

// used template for supporting int and float
template <typename time_t, typename validateTime_t = validateTime_float>
class RushHours
{
public:
	void AddTimeSpan(time_t start_time, time_t end_time) {
		validateTime(start_time);
		validateTime(end_time);

		//merge intervals_
		if (!intervals_.empty())
		{
			auto c = intervals_.upper_bound(start_time);

			if (c != intervals_.begin()) 
				--c;

			if (c->second < start_time) 
				++c;

			//merge the overlapped intervals
			while( c != intervals_.end() && 
				c->first <= end_time ) 
			{
				start_time = min(start_time, c->first);
				end_time = max(end_time, c->second);
				c = intervals_.erase(c);
			}
		}

		intervals_[start_time] = end_time;
	}

	bool IsRushHour(time_t time) {
		validateTime(time);

		auto c = intervals_.upper_bound(time);

		if (c != intervals_.begin())
			--c;

		if (c != intervals_.end() &&
			c->first <= time &&
			c->second >= time) {
			return true;
		}
			
		return false;
	}

private:

	validateTime_t validateTime;
	map<time_t, time_t> intervals_;
};

void unit_test()
{
	RushHours<float> rushHours;

	assert(false == rushHours.IsRushHour(3.00));
	assert(false == rushHours.IsRushHour(5.00));
//	assert(true == rushHours.IsRushHour(4.50));

	rushHours.AddTimeSpan(2.00, 4.00);
	assert(true == rushHours.IsRushHour(3.00));
	assert(false == rushHours.IsRushHour(5.00));

	rushHours.AddTimeSpan(7.00, 9.00);
	assert(true == rushHours.IsRushHour(3.00));
	assert(false == rushHours.IsRushHour(5.00));
	assert(true == rushHours.IsRushHour(7.00));
	assert(false == rushHours.IsRushHour(11.00));

	rushHours.AddTimeSpan(8.00, 12.00);
	assert(true == rushHours.IsRushHour(3.00));
	assert(false == rushHours.IsRushHour(5.00));
	assert(true == rushHours.IsRushHour(7.00));
	assert(true == rushHours.IsRushHour(11.00));

	rushHours.AddTimeSpan(5.00, 10.00);
	assert(true == rushHours.IsRushHour(3.00));
	assert(true == rushHours.IsRushHour(5.00));
	assert(true == rushHours.IsRushHour(7.00));
	assert(true == rushHours.IsRushHour(11.00));
	assert(false == rushHours.IsRushHour(4.50));

	rushHours.AddTimeSpan(0.00, 23.00);
	assert(true == rushHours.IsRushHour(3.00));
	assert(true == rushHours.IsRushHour(5.00));
	assert(true == rushHours.IsRushHour(7.00));
	assert(true == rushHours.IsRushHour(11.00));
	
}

char ALPHABET[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

void toBase58(int i, char* sb) {

	if (i == 0) {
	
		*sb = 0;

		return;
	}

	*sb = ALPHABET[i % 58];
	
	toBase58(i / 58, ++sb);
}
void reverse(char* sb) {

	char* i = sb;
	char* j = sb + strlen(sb)-1;
	
	while (i < j ) {
		swap(*i, *j);
		i++;
		j--;
	}
}

void main()
{

	char s[58];

	u_long n = htonl(123456);

	toBase58(n,s);
	reverse(s);

	cout << s;
	cout << s;
	//unit_test();



	return;



}






