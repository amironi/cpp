#include "stdafx.h"
#include <vector>
#include <set>

#include "stdlib.h"

typedef std::vector<char> CharVector;
typedef std::set<double> DoubleSet;

struct Formula
{
	Formula(){};
	Formula( char x )
	{
		mNumbers.push_back( x );
	}
	Formula removeBegin()
	{
		Formula newFormula = *this;

		newFormula.mNumbers.erase( newFormula.mNumbers.begin() );
		newFormula.mSymbols.erase( newFormula.mSymbols.begin() );

		return newFormula;
	}

	Formula removeEnd()
	{
		Formula newFormula = *this;

		newFormula.mNumbers.erase( --newFormula.mNumbers.end() );
		newFormula.mSymbols.erase( --newFormula.mSymbols.end() );

		return newFormula;
	}

	CharVector mNumbers;
	CharVector mSymbols;

};

CharVector remove( CharVector aVec, int aPos )
{
	CharVector CharVectorTemp = aVec;
	int i = 0;
	CharVector::iterator itr;
	for( itr = CharVectorTemp.begin(); 
		itr != CharVectorTemp.end();
		itr++ )
	{
		if ( i == aPos )
		{
			CharVectorTemp.erase ( itr );
			return 	CharVectorTemp;
		}
		i++;
	}

	return 	CharVectorTemp;
}

double calcPair( double left, char symbol, double right )
{
	double res = left;

	switch (symbol)
	{
	case '*' : 
		res *= right;
		break;
	case '/' : 
		res /= right;
		break;
	case '+' :
		res += right;
		break;
	case '-' :
		res -= right;
		break;
	}

	return res;
}
DoubleSet calcPair( DoubleSet left, char symbol, DoubleSet right )
{
	DoubleSet res;

	DoubleSet::iterator numberLeftIter;
	for( numberLeftIter = left.begin();
		numberLeftIter != left.end();
		numberLeftIter++)
	{
		DoubleSet::iterator numberRightIter;
		for( numberRightIter = right.begin();
			numberRightIter != right.end();
			numberRightIter++)
		{
			res.insert( calcPair( *numberLeftIter,  symbol, *numberRightIter ) );
		}
	}
	return res;

}
DoubleSet calcFormula( Formula aFormulaLeft, char aSymbol, Formula aFormulaRight )
{
	DoubleSet left1;
	DoubleSet left2;
	DoubleSet right1;
	DoubleSet right2;

	if( aFormulaLeft.mSymbols.size() == 0 )
	{
		left1.insert( aFormulaLeft.mNumbers[0] - '0' );
	}
	else {
		left1 = calcFormula( 
			aFormulaLeft.mNumbers[0], 
			aFormulaLeft.mSymbols[0],
			aFormulaLeft.removeBegin() );

		left2 = calcFormula( 
			aFormulaLeft.removeEnd(), 
			aFormulaLeft.mSymbols[aFormulaLeft.mSymbols.size() -1],
			aFormulaLeft.mNumbers[aFormulaLeft.mNumbers.size() -1] - '0' );

	}
	if( aFormulaRight.mSymbols.size() == 0 )
	{
		right1.insert( aFormulaRight.mNumbers[0] - '0' );
	}
	else {

		right1 = calcFormula( 
			aFormulaRight.mNumbers[0], 
			aFormulaRight.mSymbols[0],
			aFormulaRight.removeBegin() );

		right2 = calcFormula( 
			aFormulaRight.removeEnd(), 
			aFormulaRight.mSymbols[aFormulaRight.mSymbols.size() -1],
			aFormulaRight.mNumbers[aFormulaRight.mNumbers.size() -1] - '0');
	}

	DoubleSet res;

	DoubleSet s1 = calcPair(left1, aSymbol, right1 );
	DoubleSet s2 = calcPair(left1, aSymbol, right2 );
	DoubleSet s3 = calcPair(left2, aSymbol, right1 );
	DoubleSet s4 = calcPair(left2, aSymbol, right2 );
	
	res.insert( s1.begin(), s1.end() );
	res.insert( s2.begin(), s2.end() );
	res.insert( s3.begin(), s3.end() );
	res.insert( s4.begin(), s4.end() );

	return res;
}

void gen4Pair( CharVector num4, CharVector op4)
{
	CharVector resultNum;
	CharVector resultOp;

	resultNum.resize( 4 );
	resultOp.resize( 3 );

	CharVector::iterator itr4;
	for( int i = 0 ; i < 4 ; ++i )
	{
		resultNum[0] = num4[i];
		CharVector num3 = remove( num4,  i);

		for( int j = 0 ; j < 3 ; ++j )
		{
			resultNum[1] = num3[j];

			CharVector num2 = remove( num3,  j);

			for( int k = 0 ; k < 2 ; ++k )
			{
				resultNum[2] = num2[k];

				resultNum[3] = remove( num2, k)[0];

				for( int ii = 0 ; ii < 4 ; ++ii )
				{
					resultOp[0] = op4[ii];

					for( int jj = 0 ; jj < 4 ; ++jj )
					{
						resultOp[1] = op4[jj];

						for( int kk = 0 ; kk < 4 ; ++kk )
						{
							resultOp[2] = op4[kk];

							Formula f1;

							f1.mNumbers = resultNum;
							f1.mSymbols = resultOp;

							DoubleSet r = calcFormula( f1, '+', '0' );
							DoubleSet::iterator f = r.find( 21 );
							if( f != r.end() )
							{
								
								std::cout << resultNum[0];
								
								for(int i = 0 ; i< 3; i++)
								{
									std::cout << resultOp[i] << resultNum[i + 1] ;
								}

								std::cout << std::endl;
							}
						}
					}
				}
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	CharVector num4;

	num4.push_back('1'); 
	num4.push_back('5');
	num4.push_back('6');
	num4.push_back('7');

	CharVector op4;

	op4.push_back('*');
	op4.push_back('/');
	op4.push_back('-');
	op4.push_back('+');

	gen4Pair( num4, op4);

	return 0;
}

