#include "statistic.h"

Stats::Stats(Tomo_Data *&tD)
{
	this->tD = tD;
}

int Stats::averageDensity(int x, int y)
{
	int layShift = tD->lay * tD->data_size.x * tD->data_size.y;
	int nw = tD->data[layShift + (y+1)*tD->data_size.x + (x-1)];
	int n = tD->data[layShift + (y+1)*tD->data_size.x + x];
	int ne = tD->data[layShift + (y+1)*tD->data_size.x + (x+1)];
	int e = tD->data[layShift + y*tD->data_size.x + (x+1)];
	int se = tD->data[layShift + (y-1)*tD->data_size.x + (x+1)];
	int s = tD->data[layShift + (y-1)*tD->data_size.x + x];
	int sw = tD->data[layShift + (y-1)*tD->data_size.x + (x-1)];
	int w = tD->data[layShift + y*tD->data_size.x + (x-1)];

	return (nw+n+ne+e+se+s+se+w)/8;
}

int Stats::dispersion(int x, int y){
	return 0;
}