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
public: 
	DataSize dataSize;
	Scale scale;
	
	short* data3D;				//исходные данные
	uchar* data2D;				//отображаемые оттенки серого

	int lay;
	int lowIdx;
	int hiIdx;

private:
	uchar* transferFunction();
public:
	TomoData(char* file);
	virtual ~TomoData();
	
	int getLay() const;
	int getLowIdx()const;
	int getHiIdx()const;

	void setLay(int lay);
	void setLowIdx(int lowIdx);
	void setHiIdx(int hiIdx);

	uchar* getData2D();
};