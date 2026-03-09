#pragma once
#include <vector>
using namespace std;
#define CANVAS_FILE _T("canvas.bin")

class CCanvas
{
	vector< vector<bool> > mArray;
	int mMaxX;
	int mMaxY;
	bool inBounds(int x, int y);

public:
	CCanvas(int maxX, int maxY);
	~CCanvas(void);
	bool get(int x, int y);
	// returns 0 on success, -1 on failure (e.g out of bounds etc)
	int set(int x, int y, bool value);
	int getMaxX(void);
	int getMaxY(void);
	void Save(void);
	void Load(void);
	void fill(int x, int y);	
};
