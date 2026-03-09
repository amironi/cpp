/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>

using namespace std;

const int N = 5;

int Board[N][N] = {
  {0,0,0,0,0},
  {0,0,0,0,2},
  {0,0,0,2,0},
  {0,0,2,0,0},
  {0,2,0,0,0}
};

int isWinner(int x, int y, int color)
{
   
    int countRows = 0;
    int countCols = 0;
    int countDiagUp = 0;
    int countDiagDown = 0;

    for(int i = 0; i < 4 ; i++) {
        if(x+i<N && color ==  Board[x+i][y]) countRows++;
        if(y+i<N && color ==  Board[x][y+i]) countCols++;
        if(x+i<N && y+i<N && color ==  Board[x+i][y+i]) countDiagUp++;
        if(x-i >= 0 && y+i<N && color ==  Board[x-i][y+i]) countDiagDown++;
    }
    if( countRows == 4 || 
      countCols == 4 ||
      countDiagUp == 4 ||
      countDiagDown == 4 )
       return color;
 
    return 0;
}


int getWinner()
{
    int val;
    
    for( int i = 0;i<N;i++)
       for( int j = 0;j<N;j++)
         if (val = isWinner(i,j, Board[i][j]) )
           return val;

    return 0;
} 

int main ()
{
  cout << "winner: " << getWinner ();

  return 0;
}