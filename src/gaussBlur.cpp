#include "gaussBlur.h"

GaussBlur::GaussBlur(Tomo_Data *&tD, double sigma)
{
	this->tD = tD;
	this->sigma = sigma;
	double s2 = 2*sigma*sigma;

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

void GaussBlur::blur()
{
	double sum;
	int pix;
	int tmppix;
	int neighbor;

	uchar *tmp = new uchar [tD->data_size.x];

	for (int j = 0; j < tD->data_size.y; j++){
		for (int i = 0; i < tD->data_size.x; i++){
			sum = 0;
			pix = 0;
			for (int k = -size; k < size; k++){
				neighbor = i + k;
				if ((neighbor >= 0)&&(neighbor < tD->data_size.x)){
					tmppix = tD->data_lay[j*tD->data_size.x + neighbor];
					pix += tmppix * window[k + size];
					sum = sum + window[k + size];
				}
			}
			pix = (double)pix/sum;
			tmp[i] = pix;
		}
		for (int m = 0; m < tD->data_size.x; m++)
			tD->data_lay[j * tD->data_size.x + m] = tmp[m];
	}

	delete []tmp;
	tmp = new uchar [tD->data_size.y];

	for (int i = 0; i < tD->data_size.x; i++){
		for (int j = 0; j < tD->data_size.y; j++){
			sum = 0;
			pix = 0;
			for (int k = -size; k < size; k++){
				neighbor = j + k;
				if ((neighbor >= 0)&&(neighbor < tD->data_size.x)){
					tmppix = tD->data_lay[neighbor * tD->data_size.y + i];
					pix += tmppix * window[k + size];
					sum = sum + window[k + size];
				}
			}
			pix = (double)pix/sum;
			tmp[j] = pix;
		}
		for (int m = 0; m < tD->data_size.y; m++)
			tD->data_lay[m * tD->data_size.y + i] = tmp[m];
	}
	delete []tmp;
}