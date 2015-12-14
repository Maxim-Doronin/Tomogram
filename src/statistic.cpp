#include "statistic.h"

Stats::Stats(Tomo_Data *&tD)
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
	for (int i = min(y1, y2); i < max(y1, y2); i++)
		for (int j = min(x1, x2); j < max(x1, x2); j++){
			densityCount += tD->data[tD->data_size.x * tD->data_size.y * tD->lay +
									 tD->data_size.x * i + j];
			pixelsCount++;
		}
	return densityCount/pixelsCount;
}

int Stats::dispersion(int x, int y){
	return 0;
}