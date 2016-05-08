#include "cannyOperator.h"

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

			Gx = (p3 + 2*p4 + p5) - (p9 + 2*p8 + p7);
			Gy = (p9 + 2*p2 + p3) - (p7 + 2*p6 + p5);
			
			G  = sqrt((float)Gx*Gx + Gy*Gy);

			dir = (atan2((float)Gy,Gx) / pi) * 180.0f;
			if (( (dir < 22.5) && (dir >= -22.5) ) || (dir >= 157.5) || (dir < -157.5) )
				dir = 0;
			if (( (dir >= 22.5) && (dir < 67.5) ) || ( (dir < -112.5) && (dir >= -157.5) ) )
				dir = 45;
			if (( (dir >= 67.5) && (dir < 112.5) ) || ( (dir < -67.5) && (dir >= -112.5) ) )
				dir = 90;
			if (( (dir >= 112.5) && (dir < 157.5) ) || ( (dir < -22.5) && (dir >= -67.5) ) )
				dir = 135;

			tmp[width * y + x] = G;
			ga[width * y + x]  = dir;
		}
	dst = CannyOperator::gradMap(tmp, width, height);
	delete [] tmp;
}

uchar* CannyOperator::gradMap(int *&src, int width, int height)
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

			if (((ga[c] == 0)   && (src[c] >= src[c8]) && (src[c] >= src[c4])) || // 0 deg
				((ga[c] == 45)  && (src[c] >= src[c3]) && (src[c] >= src[c7])) || // 45 deg
				((ga[c] == 90)  && (src[c] >= src[c2]) && (src[c] >= src[c6])) || // 90 deg
				((ga[c] == 135) && (src[c] >= src[c9]) && (src[c] >= src[c5])))   // 135 deg
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

/*void CannyOperator::sobel3D(TomoData *&src)
{
	int G;
	float dir;
	int Gx, Gy, Gz;
	int height = src->dataSize.z;
	int width  = src->dataSize.x;
	int depth  = src->dataSize.y;

	
	short* tmp = new short [height * width * depth];
	int *n = new int[26];

	for (int z = 1; z < height - 1; z++)
		for (int y = 1; y < depth - 1; y++)
			for (int x = 1; x < width - 1; x++)
			{
				n[0]  = src->data3D[depth * width * (z+1) + width * (y-1) + x+1];
				n[1]  = src->data3D[depth * width * (z+1) + width * (y )  + x+1];
				n[2]  = src->data3D[depth * width * (z+1) + width * (y+1) + x+1];
				n[3]  = src->data3D[depth * width * (z+1) + width * (y-1) + x  ];
				n[4]  = src->data3D[depth * width * (z+1) + width * (y  ) + x  ];
				n[5]  = src->data3D[depth * width * (z+1) + width * (y+1) + x  ];
				n[6]  = src->data3D[depth * width * (z+1) + width * (y-1) + x-1];
				n[7]  = src->data3D[depth * width * (z+1) + width * (y  ) + x-1];
				n[8]  = src->data3D[depth * width * (z+1) + width * (y+1) + x-1];

				n[9]  = src->data3D[depth * width * (z  ) + width * (y-1) + x+1];
				n[10] = src->data3D[depth * width * (z  ) + width * (y  ) + x+1];
				n[11] = src->data3D[depth * width * (z  ) + width * (y+1) + x+1];
				n[12] = src->data3D[depth * width * (z  ) + width * (y-1) + x  ];
				n[13] = src->data3D[depth * width * (z  ) + width * (y+1) + x  ];
				n[14] = src->data3D[depth * width * (z  ) + width * (y-1) + x-1];
				n[15] = src->data3D[depth * width * (z  ) + width * (y  ) + x-1];
				n[16] = src->data3D[depth * width * (z  ) + width * (y+1) + x-1];

				n[17] = src->data3D[depth * width * (z-1) + width * (y-1) + x+1];
				n[18] = src->data3D[depth * width * (z-1) + width * (y  ) + x+1];
				n[19] = src->data3D[depth * width * (z-1) + width * (y+1) + x+1];
				n[20] = src->data3D[depth * width * (z-1) + width * (y-1) + x  ];
				n[21] = src->data3D[depth * width * (z-1) + width * (y  ) + x  ];
				n[22] = src->data3D[depth * width * (z-1) + width * (y+1) + x  ];
				n[23] = src->data3D[depth * width * (z-1) + width * (y-1) + x-1];
				n[24] = src->data3D[depth * width * (z-1) + width * (y  ) + x-1];
				n[25] = src->data3D[depth * width * (z-1) + width * (y+1) + x-1];

				for (int i = 0; i < 26; i++)
				{
					if (n[i] <= src->lowIdx)
					{
						n[i] = 0x00;
						continue;
					}
					if (n[i] >= src->hiIdx)
					{
						n[i] = src->hiIdx;
						continue;
					}
				}

				Gx = (n[6] + 3*n[7] + n[8] + 3*n[14] + 6*n[15] + 3*n[16] + n[23] + 3*n[24] + n[25]) 
					-(n[0] + 3*n[1] + n[2] + 3*n[9] + 6*n[10] + 3*n[11] + n[17] + 3*n[18] + n[19]);
				Gy = (n[0] + 3*n[3] + n[6] + 3*n[9] + 6*n[12] + 3*n[14] + n[17] + 3*n[20] + n[23]) 
					-(n[2] + 3*n[5] + n[8] + 3*n[11] + 6*n[13] + 3*n[16] + n[19] + 3*n[22] + n[25]);
				Gz = (n[0] + 3*n[1] + n[2] + 3*n[3]  + 6*n[4]  + 3*n[5]  + n[6]  + 3*n[7]  + n[8] ) 
					-(n[17] + 3*n[18] + n[19] + 3*n[20] + 6*n[21] + 3*n[22] + n[23] + 3*n[24] + n[25]);
				G  = sqrt((float)Gx*Gx + Gy*Gy + Gz*Gz);

				tmp[depth * width * (z) + width * (y) + x] = G;
			}
	for (int z = 1; z < height - 1; z++)
		for (int y = 1; y < depth - 1; y++)
			for (int x = 1; x < width - 1; x++)
				src->data3D[depth * width * (z) + width * (y) + x] = tmp[depth * width * (z) + width * (y) + x];
	delete []tmp;
	delete []n;
}*/