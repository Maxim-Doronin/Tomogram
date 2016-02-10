#pragma once
#include <fstream>
#include <cstdlib>
#include <qglobal.h>

struct DataSize
{
	int x;
	int y;
	int z;
};

struct Scale
{ 
	float x;
	float y;
	float z;
};

class TomoData
{
private:
	uchar* transferFunction();
public: 
	DataSize dataSize;
	Scale scale;
	
	short* data3D;				//исходные данные
	uchar* data2D;				//отображаемые оттенки серого
	double* dataDensityRectangle;

	int lay;
	int lowIdx;
	int hiIdx;
public:
	TomoData(char*);
	virtual ~TomoData();
	
	uchar* getData2D();

	void getDataDensity(int x1, int y1, int x2, int y2);
};