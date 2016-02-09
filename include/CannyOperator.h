#pragma once
#include <cmath>
#include "tomo_data.h"

class SobelOperator{
public: 
	static void sobel(Tomo_Data *&tD);
};