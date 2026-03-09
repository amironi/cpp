#include "StdAfx.h"
#include "Canvas.h"

CCanvas::CCanvas(int maxX, int maxY) :
	mArray(maxX, vector<bool>(maxY, false))
{
	mMaxX = maxX;
	mMaxY = maxY;
	Load();
}

CCanvas::~CCanvas(void)
{
	
}

bool CCanvas::get(int x, int y)
{
	if(inBounds(x,y)) 
	{
		return mArray[x][y];
	}

	return false;
}

// returns 0 on success, -1 on failure (e.g out of bounds etc)
int CCanvas::set(int x, int y, bool value)
{
	if(!inBounds(x,y)) {
		return -1;
	}
	
	mArray[x][y] = value;

	return 0;
}

bool CCanvas::inBounds(int x, int y)
{
	return (x >= 0 && x < mMaxX) && (y >= 0 && y < mMaxY);
}

int CCanvas::getMaxX(void)
{
	return mMaxX;
}

int CCanvas::getMaxY(void)
{
	return mMaxY;
}

void CCanvas::Save(void)
{
	HANDLE hFile;
	int x, y;
	BYTE b;
	DWORD written;

	hFile = CreateFile(CANVAS_FILE, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return;
	for(x = 0; x < getMaxX(); x++) {
		for(y = 0; y < getMaxY(); y++) {
			b = get(x, y) ? 1 : 0;
			WriteFile(hFile, &b, 1, &written, NULL);
		}
	}
	CloseHandle(hFile);
}

void CCanvas::Load(void)
{
	HANDLE hFile;
	int x, y;
	BYTE b;
	DWORD read;

	hFile = CreateFile(CANVAS_FILE, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return;
	for(x = 0; x < getMaxX(); x++) {
		for(y = 0; y < getMaxY(); y++) {
			ReadFile(hFile, &b, 1, &read, NULL);
			set(x, y, b ? true : false);			
		}
	}
	CloseHandle(hFile);	
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Implement here:
//
 void CCanvas::fill(int x, int y)
 {
 	set(x, y, !get(x,y));
 }

