#include "tomo_data.h"

Tomo_Data::Tomo_Data(char* file)
{
	std::ifstream fs(file, std::ios::in | std::ios::binary);

	fs.read((char*)&data_size.x,sizeof(int));
	fs.read((char*)&data_size.y,sizeof(int));
	fs.read((char*)&data_size.z,sizeof(int));

	fs.read((char*)&scale.x,sizeof(float));
	fs.read((char*)&scale.y,sizeof(float));
	fs.read((char*)&scale.z,sizeof(float));

	data = new short[data_size.x * data_size.y * data_size.z];
	fs.read((char*)data,2*data_size.x * data_size.y * data_size.z);

	fs.close();

	minTint = data[0];
	maxTint = data[0];
	for (int i = 0; i<data_size.x * data_size.y * data_size.z; i++)
	{
		if (data[i] < minTint) minTint = data[i];
		if (data[i] > maxTint) maxTint = data[i];
	}

}

Tomo_Data::~Tomo_Data()
{
	delete[] data;
	delete[] data_pixels;
}

uchar* Tomo_Data::get_lay(int &lay, short lowIdx, short hiIdx)
{
	data_pixels = new uchar [data_size.x * data_size.y * data_size.z];
	
	if (lay < 0) lay = 0;
	if (lay > data_size.z - 1) lay = data_size.z - 1;
	this->transfer_function(lay, lowIdx, hiIdx);
	return data_pixels;
}

uchar* Tomo_Data::get_lay(int &lay)
{
	
	data_pixels = new uchar [data_size.x * data_size.y * data_size.z];
	
	if (lay < 0) lay = 0;
	if (lay > data_size.z - 1) lay = data_size.z - 1;
	for (int i=0; i<data_size.x * data_size.y; i++)
	{
		data_pixels[i] = (uchar)data[i + lay * data_size.x * data_size.y]&0xFF00>>8;
	}
	return data_pixels;
}

uchar* Tomo_Data::transfer_function(int lay, short lowIdx, short hiIdx)
{
	short tint;
	short pixel;
	
	for (int i=0; i<data_size.x * data_size.y; i++)
	{
		pixel = data[i + lay * data_size.x * data_size.y];
		
		if (pixel <= lowIdx)
		{
			data_pixels[i] = 0x00;
			continue;
		}
			
		if (pixel >= hiIdx)
		{
			data_pixels[i] = 0xFF;
			continue;
		}

		tint = ((double)(pixel - lowIdx)) / (hiIdx - lowIdx) *255; 
		data_pixels[i] = tint ;
	}
	return data_pixels;
}

void Tomo_Data::pixels_delete()
{
	delete [] data_pixels;
}

