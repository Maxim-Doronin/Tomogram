#pragma once
#include "TomoData.h"
#include <cstdlib>

class Stats{
public:
	Stats();
	~Stats(){};

	void setData(short *&data, int xSize, int ySize, int zSize);
	void setLay(int lay);
	void setArea(int x1, int y1, int x2, int y2);

	int averageDensity();
	int expectedValue();
	int dispersion();
	int meanSquareDeviation();

private:
	short *data3D;

	int xSize, ySize, zSize;
	int lay;

	bool layValidity;
	bool posValidity;
	bool dataValidity;

	int x1, x2, y1, y2;

	float *dataDensityRectangle;
	void getDataDensity();

	double expVal;
	double disperVal;
};
