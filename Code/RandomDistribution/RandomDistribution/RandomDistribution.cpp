// RandomDistribution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <time.h>
#include <iostream>

typedef std::vector<int> IntVecor;
typedef std::vector<double> DoubleVecor;

class RandomDistribution
{
public:
	// don't use "&" : aP is changed in this function.
	RandomDistribution( DoubleVecor aP )
	{

		mSize = aP.size();

		double divSize = ( 1 / (double)aP.size());

		for( size_t j = 0 ; j < aP.size() ; j++ )
		{
			if ( aP[j] > divSize )
			{
				mLarge.push_back( j );
			}
			else
			{
				mSmall.push_back( j );
			}
		}

		int s = mSmall.size();
		int l = mLarge.size();

		mProb.resize(aP.size());
		mAlias.resize(aP.size());

		while( s != 0 && l != 0 )
		{
			int j = mSmall[--s];
			int k = mLarge[--l];

			mProb[j] = mSize * aP[j];
			mAlias[j] = k;

			aP[k] += aP[j] - divSize;

			if( aP[k] > divSize )
			{
				mLarge[l++] = k; 
				
			}
			else
			{
				mSmall[s++] = k;
			}
		}
		while ( s > 0)
		{
			{
				mProb[mSmall[--s]] = 1;
			}
		}

		while ( l > 0)
		{
			{
				mProb[mLarge[--l]] = 1;
			}
		}

	}

	int linear_rand()
	{
		double u = rand()/(double)(RAND_MAX+1)*(mSize);

		int j = (int)u;

		if ( u - j <= mProb[j])
		{
			return j;
		}
		else
		{
			return mAlias[j];
		}

	}
	DoubleVecor mProb;
	IntVecor mAlias;

	IntVecor mLarge;
	IntVecor mSmall;

	int mSize;


};

int count  = 1000000;

void test1()
{
 	double testNumbers[] = {
 		0.01,
 		0.08,
 		0.6,
 		0.0021,
 		0.0079,
 		0.2,
 		0.1, };

	DoubleVecor vec(testNumbers, testNumbers + sizeof(testNumbers) / sizeof(double) );


	RandomDistribution rd(vec);

	IntVecor results(sizeof(testNumbers) / sizeof(double));

	for( int i = 0; i < count; ++i )
	{
		results[rd.linear_rand()]++ ;
	}

	for( int i = 0; i < sizeof(testNumbers) / sizeof(double); ++i )
	{
		std::cout << "result : " << (double)results[i]/count << ", should be: " << testNumbers[i] << std::endl ;
	}

}


// Generic random unit Test
void test2()
{

	// Easy to change this value for tests
	DoubleVecor vec(50);

	double sum =  0;

	for( size_t i = 0 ; i<vec.size(); ++i )
	{
		vec[i] =  rand()/(double)(RAND_MAX+1);

		sum += vec[i];
	}

	//just for test
	double testSum1 = 0;

	for( size_t i = 0 ; i<vec.size(); ++i )
	{
		vec[i] = vec[i]/sum;

		testSum1 += vec[i];
	}

	RandomDistribution rd(vec);

	IntVecor results(vec.size());


	for( int i = 0; i < count; ++i )
	{
		results[rd.linear_rand()]++ ;
	}

	for( size_t i = 0; i < vec.size(); ++i )
	{
		std::cout << "result : " << (double)results[i]/count << ", should be(original): " << vec[i] << " Delta : "<< abs( (double)results[i]/count - vec[i] )  <<   std::endl ;
	}

}

int _tmain(int argc, _TCHAR* argv[])
{

	srand( (unsigned)time( NULL ) );


	//test1();
	//return 0;
	test2();
	

	return 0;
}

