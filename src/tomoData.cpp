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

	data2D	= 0;

	lay		= 0;
	lowIdx	= 1900;
	hiIdx	= 2250;
}

TomoData::~TomoData()
{
	delete[] data3D;
	delete[] data2D;
}

int TomoData::getLay() const
{
	return lay;
}
	
int TomoData::getLowIdx() const
{
	return lowIdx;
}
	
int TomoData::getHiIdx() const
{
	return hiIdx;
}

void TomoData::setLay(int lay)
{
	if (lay < 0)
	{ 
		this->lay = 0;
		return;
	}
	if (lay > dataSize.z - 1) 
	{
		this->lay = dataSize.z - 1;
		return;
	}
	this->lay = lay;
}
	
void TomoData::setLowIdx(int lowIdx)
{
	this->lowIdx = lowIdx;
}
	
void TomoData::setHiIdx(int hiIdx)
{
	this->hiIdx = hiIdx;
}

uchar* TomoData::getData2D()
{
	if (!data2D)
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
