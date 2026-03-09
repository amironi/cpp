N=6

Board = [
[1,0,0,0,0,2],
[0,2,0,0,2,0],
[0,0,1,2,0,0],
[0,0,2,1,0,0],
[0,2,0,1,0,0],
[0,2,0,1,0,0]
]

def isWinner( x, y, p):
    countRows = 0
    countCols = 0
    countUpDiag = 0
    countDownDiag = 0

    for i in xrange(4):
        if x+i < N and p ==  Board[x+i][y]: countRows += 1
        if y+i < N and p ==  Board[x][y+i]: countCols += 1
        if x+i < N and y+i < N and p ==  Board[x+i][y+i]: countUpDiag += 1
        if x-i >= 0 and y+i < N and p ==  Board[x-i][y+i]: countDownDiag += 1

    if countRows == 4 or countCols == 4 or countUpDiag == 4 or countDownDiag == 4:
        return p

    return 0

def getWinner():
   #val = 0
   for i, j  in[(i,j) for i in range(N) for j in range(N)]:
          val = isWinner(i,j,Board[x][y])
          if val:
              return val
   return 0

def main ():
  print "winner: ",  getWinner()

if __name__ == "__main__":
    main()