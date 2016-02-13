#include "cannyOperator.h"

uchar* gradMap(int *&src, int width, int height);
void CannyOperator::sobel(uchar *&src, uchar *&dst, float *&ga, int width, int height)
{
	int G;
	float dir;
	int Gx, Gy;

	if ((dst)&&(src != dst)) delete []dst;
	if (ga)  delete []ga;
	
	int* tmp = new int [height * width];
	ga  = new float [height * width];

	for (int y = 1; y < height - 1; y++)
		for (int x = 1; x < width - 1; x++)
		{
			uchar p2 = src[width*(y-1) + x];
			uchar p3 = src[width*(y-1) + x+1];
			uchar p4 = src[width* y    + x+1];
			uchar p5 = src[width*(y+1) + x+1];
			uchar p6 = src[width*(y+1) + x];
			uchar p7 = src[width*(y+1) + x-1];
			uchar p8 = src[width* y    + x-1];
			uchar p9 = src[width*(y-1) + x-1];

			Gx = (p7 + 2*p6 + p5) - (p9 + 2*p2 + p3);
			Gy = (p3 + 2*p4 + p5) - (p9 + 2*p8 + p7);
			
			G  = sqrt((float)Gx*Gx + Gy*Gy);

			dir = (float)(fmod(atan2((float)Gy,Gx) + pi, pi) / pi) * 8;

			tmp[width * y + x] = G;
			ga[width * y + x]  = dir;
		}
	dst = gradMap(tmp, width, height);
	delete [] tmp;
}

uchar* gradMap(int *&src, int width, int height)
{
	short tint;
	short pixel;
	short maxTint = 0;
	uchar *tmp = new uchar [width * height];

	for (int i=0; i < width * height; i++)
		if (src[i] > maxTint) maxTint = src[i];
	
	for (int i=0; i < width * height; i++) {
		pixel = src[i];
		tint = ((double)pixel) / maxTint *255; 
		tmp[i] = tint ;
	}
	return tmp;
}

void CannyOperator::nonMaxSuppression(uchar *&src, float *&ga, uchar *&dst, int width, int height)
{
	if(!src) return;
	if(!ga)  return;

	if ((dst)&&(src != dst)) delete []dst;
	if (src != dst) dst = new uchar [width * height];

	for (int y = 1; y < height - 1; y++)
		for (int x = 1; x < width - 1; x++)
		{
			int c  = width* y    + x;
			int c2 = width*(y-1) + x;
			int c3 = width*(y-1) + x+1;
			int c4 = width* y    + x+1;
			int c5 = width*(y+1) + x+1;
			int c6 = width*(y+1) + x;
			int c7 = width*(y+1) + x-1;
			int c8 = width* y    + x-1;
			int c9 = width*(y-1) + x-1;

			if (((ga[c] <= 1 || ga[c] > 7) && src[c] > src[c8] && src[c] > src[c4]) || // 0 deg
				((ga[c] > 1 && ga[c] <= 3) && src[c] > src[c3] && src[c] > src[c7]) || // 45 deg
				((ga[c] > 3 && ga[c] <= 5) && src[c] > src[c2] && src[c] > src[c6]) || // 90 deg
				((ga[c] > 5 && ga[c] <= 7) && src[c] > src[c9] && src[c] > src[c5]))   // 135 deg
				dst[c] = src[c];
			else
				dst[c] = 0;
		}
}

void CannyOperator::doubleTresholding(uchar *&src, uchar *&dst, float low_pr, float hi_pr, int width, int height)
{
	uchar down = low_pr * 255;
	uchar up   = hi_pr  * 255;

	if ((dst)&&(src != dst)) delete []dst;
	if (src != dst) dst = new uchar [width * height];

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			int c = y * width + x;
			if (src[c] >= up) dst[c] = 255;
			else 
				if (src[c] <= down) dst[c] = 0;
				else 
					dst[c] = 127;
		}
}

void checkNeighbours(uchar *&dst, int c, int width, int height)
{
	if ((c < 0)||(c >= width * height)||(dst[c] != 127)) return;
	dst[c] = 255;
	int c2 = c - width;
	int c9 = c2 - 1;
	int c3 = c2 + 1;
	int c8 = c - 1;
	int c4 = c + 1;
	int c6 = c + width;
	int c7 = c6 - 1;
	int c5 = c6 + 1;
	checkNeighbours(dst, c2, width, height);
	checkNeighbours(dst, c9, width, height);
	checkNeighbours(dst, c3, width, height);
	checkNeighbours(dst, c8, width, height);
	checkNeighbours(dst, c4, width, height);
	checkNeighbours(dst, c6, width, height);
	checkNeighbours(dst, c7, width, height);
	checkNeighbours(dst, c5, width, height);
	
}

void CannyOperator::tracingEdges(uchar *&src, uchar *&dst, int width, int height)
{
	if ((dst)&&(src != dst)) delete []dst;
	if (src != dst) {
		dst = new uchar [width * height];
		for (int y = 1; y < height - 1; y++)
			for (int x = 1; x < width - 1; x++)
				dst[y * width + x] = src[y * width + x];
	}

	for (int y = 1; y < height - 1; y++)
		for (int x = 1; x < width - 1; x++)
		{
			int c = y * width + x;
			if (dst[c] == 255)
			{
				int c2 = c - width;
				int c9 = c2 - 1;
				int c3 = c2 + 1;
				int c8 = c - 1;
				int c4 = c + 1;
				int c6 = c + width;
				int c7 = c6 - 1;
				int c5 = c6 + 1;
				checkNeighbours(dst, c2, width, height);
				checkNeighbours(dst, c9, width, height);
				checkNeighbours(dst, c3, width, height);
				checkNeighbours(dst, c8, width, height);
				checkNeighbours(dst, c4, width, height);
				checkNeighbours(dst, c6, width, height);
				checkNeighbours(dst, c7, width, height);
				checkNeighbours(dst, c5, width, height);
			}
		}

	for (int y = 1; y < height - 1; y++)
		for (int x = 1; x < width - 1; x++)
		{
			int c = y * width + x;
			if (dst[c] == 127) dst[c] = 0;
		}
}