#pragma once
#include <cmath>
#include "tomoData.h"
#define pi 3.141592
typedef unsigned char uchar;

class CannyOperator {
public: 
	static void sobel(uchar *&src, uchar *&dst, float *&ga, int width, int height);
	static void sobel3D(TomoData *&src);
	static uchar* gradMap(int *&src, int width, int height);
	static void nonMaxSuppression(uchar *&src, float *&ga, uchar *&dst, int width, int height);
	static void doubleTresholding(uchar *&src, uchar *&dst, float low_pr, float hi_pr, int width, int height);
	static void tracingEdges(uchar *&src, uchar *&dst, int width, int height);
};