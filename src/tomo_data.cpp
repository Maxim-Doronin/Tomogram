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

	data_lay	 = 0;
	data_density = 0;

	lay			 = 0;
	lowIdx		 = 1800;
	hiIdx		 = 2500;
}

Tomo_Data::~Tomo_Data()
{
	delete[] data;
	delete[] data_lay;
	delete[] data_density;
}

uchar* Tomo_Data::get_data_lay()
{
	if (data_lay) delete data_lay;
	data_lay = new uchar [data_size.x * data_size.y * data_size.z];
	
	if (lay < 0) lay = 0;
	if (lay > data_size.z - 1) lay = data_size.z - 1;
	this->transfer_function();
	return data_lay;
}

uchar* Tomo_Data::transfer_function()
{
	short tint;
	short pixel;
	
	for (int i=0; i<data_size.x * data_size.y; i++) {
		pixel = data[i + lay * data_size.x * data_size.y];
		
		if (pixel <= lowIdx) {
			data_lay[i] = 0x00;
			continue;
		}
		if (pixel >= hiIdx) {
			data_lay[i] = 0xFF;
			continue;
		}
		tint = ((double)(pixel - lowIdx)) / (hiIdx - lowIdx) *255; 
		data_lay[i] = tint ;
	}
	return data_lay;
}

void Tomo_Data::get_data_density()
{
	if (lay < 0) lay = 0;
	if (lay > data_size.z - 1) lay = data_size.z - 1;

	if (data_density) delete data_density;
	data_density = new short [2500];

	for (int i = 0; i < 2500; i++)
		data_density[i] = 0;
	for (int i = 0; i < data_size.x * data_size.y; i++)	{
		if (data[lay * data_size.x * data_size.y + i] < 2500)
			data_density[data[lay * data_size.x * data_size.y + i]]++;
	}
}