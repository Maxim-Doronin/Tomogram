#include "TomoData.h"

TomoData::TomoData(char* file)
{
	std::ifstream fs(file, std::ios::in | std::ios::binary);

	fs.read((char*)&dataSize.x,sizeof(int));
	fs.read((char*)&dataSize.y,sizeof(int));
	fs.read((char*)&dataSize.z,sizeof(int));

	fs.read((char*)&scale.x,sizeof(float));
	fs.read((char*)&scale.y,sizeof(float));
	fs.read((char*)&scale.z,sizeof(float));

	data3D = new short[dataSize.x * dataSize.y * dataSize.z];
	fs.read((char*)data3D, 2*dataSize.x * dataSize.y * dataSize.z);

	fs.close();

	data2D	 = 0;
	dataDensityRectangle = 0;

	lay			 = 0;
	lowIdx		 = 1900;
	hiIdx		 = 2250;
}

TomoData::~TomoData()
{
	delete[] data3D;
	delete[] data2D;
	delete[] dataDensityRectangle;
}

uchar* TomoData::getData2D()
{
	if (data2D) delete data2D;
	data2D = new uchar [dataSize.x * dataSize.y];
	
	if (lay < 0) lay = 0;
	if (lay > dataSize.z - 1) lay = dataSize.z - 1;
	this->transferFunction();
	return data2D;
}

uchar* TomoData::transferFunction()
{
	short tint;
	short pixel;
	
	for (int i=0; i<dataSize.x * dataSize.y; i++) {
		pixel = data3D[i + lay * dataSize.x * dataSize.y];
		
		if (pixel <= lowIdx) {
			data2D[i] = 0x00;
			continue;
		}
		if (pixel >= hiIdx) {
			data2D[i] = 0xFF;
			continue;
		}
		tint = ((double)(pixel - lowIdx)) / (hiIdx - lowIdx) *255; 
		data2D[i] = tint ;
	}
	return data2D;
}

void TomoData::getDataDensity(int x1, int y1, int x2, int y2)
{
	using namespace std;
	if (lay < 0) lay = 0;
	if (lay > dataSize.z - 1) lay = dataSize.z - 1;

	if (dataDensityRectangle) delete dataDensityRectangle;
	dataDensityRectangle = new double [25000];

	for (int i = 0; i < 25000; i++)
		dataDensityRectangle[i] = 0;
	int count = 0;
	for (int j = 0; j < dataSize.y; j++)
		for (int i = 0; i < dataSize.x; i++)
			if ((j >= min(y1, y2)) && (j <= max(y1, y2)) &&
				(i >= min(x1, x2)) && (i <= max(x1, x2))) {
				dataDensityRectangle[data3D[lay * dataSize.x * dataSize.y + j*dataSize.x + i]]++;
				count++;
			}	
	for (int i = 0; i < 25000; i++)
		dataDensityRectangle[i] /= count;
}