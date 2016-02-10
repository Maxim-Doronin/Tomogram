#include "gaussBlur.h"

void getWindow(float sigma, int &size, double *&window)
{
	float s2 = 2*sigma*sigma;

	size = ceil(3*sigma);
	window = new double [2*size + 2];
	window[size] = 1;
	for (int i = size + 1; i < 2*size + 1; i++){
		double tmp;
		tmp = exp(-(i-size) * (i-size)/s2);
		window[i] = exp(-(i-size) * (i-size)/s2);
		window[2*size-i] = window[i];
	}
}

void GaussBlur::blur(uchar *&src, uchar *&dst, float sigma, int width, int height)
{
	int size;
	double *window;
	getWindow(sigma, size, window);
	
	double sum;
	int pix;
	int tmppix;
	int neighbor;

	if ((dst)&&(src != dst)) delete []dst;
	if (src != dst) dst = new uchar [width * height];

	uchar *tmp = new uchar [width];

	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			sum = 0;
			pix = 0;
			for (int k = -size; k < size; k++){
				neighbor = i + k;
				if ((neighbor >= 0)&&(neighbor < width)){
					tmppix = src[j*width + neighbor];
					pix += tmppix * window[k + size];
					sum = sum + window[k + size];
				}
			}
			pix = (double)pix/sum;
			tmp[i] = pix;
		}
		for (int m = 0; m < width; m++)
			dst[j * width + m] = tmp[m];
	}

	delete []tmp;
	tmp = new uchar [height];

	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			sum = 0;
			pix = 0;
			for (int k = -size; k < size; k++){
				neighbor = j + k;
				if ((neighbor >= 0)&&(neighbor < width)){
					tmppix = src[neighbor * height + i];
					pix += tmppix * window[k + size];
					sum = sum + window[k + size];
				}
			}
			pix = (double)pix/sum;
			tmp[j] = pix;
		}
		for (int m = 0; m < height; m++)
			dst[m * height + i] = tmp[m];
	}
	delete []tmp;
	delete []window;
}