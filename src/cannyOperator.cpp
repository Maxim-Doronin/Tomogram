#include "sobelOperator.h"
#define pi 3.141592

void SobelOperator::sobel(Tomo_Data *&tD)
{
	float G;
	int theta;
	int Gx, Gy;
	int height = tD->data_size.y;
	int width  = tD->data_size.x;

	uchar* map = new uchar [height * width];

	for (int y = 1; y < height; y++)
		for (int x = 1; x < width; x++)
		{
			uchar p2 = tD->data_lay[width*(y-1) + x];
			uchar p3 = tD->data_lay[width*(y-1) + x+1];
			uchar p4 = tD->data_lay[width* y    + x+1];
			uchar p5 = tD->data_lay[width*(y+1) + x+1];
			uchar p6 = tD->data_lay[width*(y+1) + x];
			uchar p7 = tD->data_lay[width*(y+1) + x-1];
			uchar p8 = tD->data_lay[width* y    + x-1];
			uchar p9 = tD->data_lay[width*(y-1) + x-1];

			Gx = (p7 + 2*p6 + p5) - (p9 + 2*p2 + p3);
			Gy = (p3 + 2*p4 + p5) - (p9 + 2*p8 + p7);

			G  = sqrt((float)Gx*Gx + Gy*Gy);
			if (Gx) theta = (int)57,2958*((atan((float)Gy / Gx) / (pi/4)) * (pi/4) - (pi/2));
			if ((!Gx)&&(Gy))  theta = 90;
			if ((!Gx)&&(!Gy)) theta = 0;

			map[width * y + x] = G;
		}
	for (int i = 0; i < width*height; i++)
		tD->data_lay[i] = map[i];
	delete [] map;
}