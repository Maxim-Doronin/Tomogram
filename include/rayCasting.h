#pragma once
#include "tomoData.h"
#include <cmath>
#include <omp.h>
#include <vector>
#include <algorithm>
#include <qmath.h>

struct Vec3f {
	float x;
	float y;
	float z;
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
	int width;
	int height;
	int depth;

	TomoData *data;
	Options *option;
	RGBA* rgba;
	
	RGBA* frameBuffer;
	RGBA* pixel;

	void calculateOrigin(Vec3f &origin, float phi, float psi);
	RGBA& ray(Vec3f, Vec3f, RGBA&, Options*,  int , int);
public:
	RayCasting(TomoData *data);
	void render(float phi, float psi);
};