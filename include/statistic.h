#pragma once
#include "TomoData.h"
#include <cstdlib>

class Stats{
public:
	Stats(TomoData*&);
	~Stats(){};

	int averageDensity(int x1, int y1, int x2, int y2);
	int expectedValue(int x1, int y1, int x2, int y2);
	int dispersion();
	int meanSquareDeviation();

private:
	TomoData *tD;
	double expVal;
	double disperVal;
};
