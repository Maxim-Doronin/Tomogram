#pragma once
#include <fstream>
#include <cstdlib>

typedef unsigned char uchar;
#define minR 0
#define maxR 4000

struct RGBA {
	uchar red;
	uchar green;
	uchar blue;
	float alpha;
};

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
	
	short* data3D;			//исходные данные
	uchar* data2D;			//отображаемые оттенки серого
	RGBA*  dataColor2D;		//отображемое цветная картинка

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