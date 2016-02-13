#include "statistic.h"

Stats::Stats()
{
	data3D = 0;
	lay	   = 0;
	dataDensityRectangle = 0;
	dataValidity = false;
	posValidity  = false;
	layValidity  = false;
}

Stats::~Stats()
{
	if (dataDensityRectangle)
		delete [] dataDensityRectangle;
}

void Stats::setData(short *&data, int xSize, int ySize, int zSize)
{
	this->data3D = data;
	this->xSize  = xSize;
	this->ySize  = ySize;
	this->zSize  = zSize;
}

void Stats::setLay(int lay)
{
	if((lay < 0) || (lay >= zSize))
	{
		layValidity = false;
		return;
	}
	this->lay = lay;
	layValidity = true;
}

void Stats::setArea(int _x1, int _y1, int _x2, int _y2)
{
	using namespace std;
	this->x1 = min(_x1, _x2);
	this->x2 = max(_x1, _x2);
	this->y1 = min(_y1, _y2);
	this->y2 = max(_y1, _y2);
	
	if (x1 < 0) x1 = 0;
	if (x1 >= xSize) x1 = xSize - 1;
	if (x2 < 0) x2 = 0;
	if (x2 >= xSize) x2 = xSize - 1;
	if (y1 < 0) y1 = 0;
	if (y1 >= ySize) y1 = ySize - 1;
	if (y2 < 0) y2 = 0;
	if (y2 >= ySize) y2 = ySize - 1;
	
	posValidity = true;
}

void Stats::getDataDensity()
{
	if(!posValidity)
	{
		dataValidity = false;
		return;
	}

	if (dataDensityRectangle) delete dataDensityRectangle;
	dataDensityRectangle = new float [25000];
	for (int i = 0; i < 25000; i++)
		dataDensityRectangle[i] = 0;

	int count = 0;
	for (int i = y1; i <= y2; i++)
		for (int j = x1; j <=x2; j++)
		{
			dataDensityRectangle[data3D[xSize * ySize * lay +
									 xSize * (ySize - i) + j]]++;
			count++;
		}	
	for (int i = 0; i < 25000; i++)
		dataDensityRectangle[i] /= count;
	dataValidity = true;
}



int Stats::averageDensity()
{
	if (!data3D) return 0;

	int pixelsCount  = 0;
	int densityCount = 0;
	if ((x1==x2)&&(y1==y2)){
		x1--; x2++;	y1--; y2++;
	}
	for (int i = y1; i <= y2; i++)
		for (int j = x1; j <= x2; j++){
			densityCount += data3D[xSize * ySize * lay +
									 xSize * (ySize - i) + j];
			pixelsCount++;
		}
	return densityCount/pixelsCount;
}

int Stats::expectedValue()
{
	getDataDensity();
	if ((!dataValidity)||(!data3D)) return 0;
	if ((x1==x2)&&(y1==y2)){
		x1--; x2++;	y1--; y2++;
	}
	expVal = 0;
	for (int i = 0; i < 25000; i++){
		expVal += i * dataDensityRectangle[i];
	}
	return int(expVal);
}

int Stats::dispersion()
{
	if ((!dataValidity)||(!data3D)) return 0;
	disperVal = 0;
	for (int i = 0; i < 25000; i++){
		disperVal += i*i * dataDensityRectangle[i];
	}
	disperVal -= expVal * expVal;
	return (int)disperVal;
}

int Stats::meanSquareDeviation()
{
	if ((!dataValidity)||(!data3D)) return 0;
	return sqrt(disperVal);
}