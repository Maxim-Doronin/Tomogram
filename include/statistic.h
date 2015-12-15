#pragma once
#include "tomo_data.h"
#include <cstdlib>

class Stats{
public:
	Stats(Tomo_Data*&);
	~Stats(){};

	int averageDensity(int x1, int y1, int x2, int y2);
	int expectedValue(int x1, int y1, int x2, int y2);
	int dispersion();
	int meanSquareDeviation();

private:
	Tomo_Data *tD;
	double expVal;
	double disperVal;
};
