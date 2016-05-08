#pragma once
#include "tomoData.h"
#include <cmath>
#include <omp.h>
#include <vector>
#include <algorithm>

#define pi 3.14159265358979323846

class Vec3f {
public:
	float x;
	float y;
	float z;
	float alpha;
	
	float getLength();
	void normalize();
	float scalarMult(const Vec3f &vec);
	float getCos(Vec3f &vec);
};

struct Options
{
	unsigned int width;
	unsigned int height;
	float beta;
	float radius;
	int minRange;
	int maxRange;
};

class RayCasting {
private:
	int width;
	int height;
	int depth;
	int volumeRadius;

	TomoData *data;
	Options *option;
	RGBA* rgba;

	void calculateOrigin(Vec3f &origin, float phi, float psi);
	Vec3f calculateGradient(float x, float y, float z);
	inline ushort getDensity(int x, int y, int z);
	ushort interpolation(float x, float y, float z);
	RGBA& ray(Vec3f, Vec3f, RGBA&, Options*,  int , int);

public:
	RayCasting(TomoData *data, int minRange, int maxRange);
	~RayCasting();
	void render(float phi, float psi);
};