#include "tomo_data.h"
#include <cstdlib>

class GaussBlur{
public:
	GaussBlur(Tomo_Data *&, double sigma);
	~GaussBlur(){};
	void blur();

private:
	Tomo_Data *tD;
	double sigma;

	int size;
	double *window;
};