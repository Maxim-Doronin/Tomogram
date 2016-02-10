#include "statistic.h"

Stats::Stats(TomoData *&tD)
{
	this->tD = tD;
}

int Stats::averageDensity(int x1, int y1, int x2, int y2)
{
	using namespace std;
	int pixelsCount  = 0;
	int densityCount = 0;
	if ((x1==x2)&&(y1==y2)){
		x1--; x2++;	y1--; y2++;
	}
	for (int i = min(y1, y2); i <= max(y1, y2); i++)
		for (int j = min(x1, x2); j <= max(x1, x2); j++){
			densityCount += tD->data3D[tD->dataSize.x * tD->dataSize.y * tD->lay +
									 tD->dataSize.x * (tD->dataSize.y - i) + j];
			pixelsCount++;
		}
	return densityCount/pixelsCount;
}

int Stats::expectedValue(int x1, int y1, int x2, int y2)
{
	if ((x1==x2)&&(y1==y2)){
		x1--; x2++;	y1--; y2++;
	}
	expVal = 0;
	tD->getDataDensity(x1, y1, x2, y2);
	for (int i = 0; i < 25000; i++){
		expVal += i * tD->dataDensityRectangle[i];
	}
	return int(expVal);
}

int Stats::dispersion(){
	disperVal = 0;
	for (int i = 0; i < 25000; i++){
		disperVal += i*i * tD->dataDensityRectangle[i];
	}
	disperVal -= expVal * expVal;
	return (int)disperVal;
}

int Stats::meanSquareDeviation()
{
	return sqrt(disperVal);
}