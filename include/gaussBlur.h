#pragma once
#include <cstdlib>
#include <cmath>
typedef unsigned char uchar;

class GaussBlur{
public:
	static void blur(uchar *&src, uchar *&dst, float sigma, int width, int height);
};