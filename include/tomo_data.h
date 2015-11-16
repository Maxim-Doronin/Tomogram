#pragma once
#include <fstream>
#include <qglobal.h>

struct Data_Size
{
	int x;
	int y;
	int z;
};

struct Scale
{ 
	int x;
	int y;
	int z;
};

class Tomo_Data
{
public:
	Data_Size data_size;
	Scale scale;
	short* data;
	uchar* data_pixels;
	int mSizeOfDataArray;
public:
	Tomo_Data(char*);
	uchar* pixels();

};