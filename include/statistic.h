#pragma once
#include "tomo_data.h"

class Stats{
public:
	Stats(Tomo_Data*&);
	~Stats(){};

	int averageDensity(int x, int y);
	int dispersion(int x, int y);

private:
	Tomo_Data *tD;
	
};
