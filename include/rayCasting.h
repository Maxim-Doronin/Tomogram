#pragma once
#include "tomoData.h"
#include <cmath>
#include <omp.h>
#include <vector>
#include <algorithm>

#define M_PI 3.14159265358979323846

struct Vec3f {
	float x;
	float y;
	float z;
};

struct RGBA {
	uchar red;
	uchar green;
	uchar blue;
	float alpha;
};

struct Options
{
	unsigned int width;
	unsigned int height;
	float beta;
	float radius;
};

class RayCasting {
private:
	float eyeDist;
	float step;

	int width;
	int height;
	int depth;

	TomoData *data;
	Options *option;
	RGBA* rgba;
	
	RGBA* frameBuffer;
	RGBA* pixel;

	void getInc(int x, int y);
	
	RGBA& ray(Vec3f, Vec3f, Options*, int , int);
public:
	RayCasting(TomoData *data);
	void render();
};