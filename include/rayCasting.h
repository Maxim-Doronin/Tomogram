#pragma once
#include "tomoData.h"
#include <cmath>
#include <omp.h>

struct RGBA {
	uchar red;
	uchar green;
	uchar blue;
	float alpha;
};

struct Increments{
	float dx;
	float dy;
	float dz;
};

class RayCasting {
private:
	float eyeDist;
	float step;

	int width;
	int height;
	int depth;

	TomoData *data;
	RGBA* rgba;

	Increments* inc;
	void getInc(int x, int y);
	
	RGBA& ray(int x, int y);
public:
	RayCasting(TomoData *data);
	void render();
};