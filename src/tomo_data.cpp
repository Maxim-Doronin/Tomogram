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
	data_density_rectangle = 0;

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
	data_density = new double [2500];

	for (int i = 0; i < 2500; i++)
		data_density[i] = 0;
	int count = 0;
	for (int i = 0; i < data_size.x * data_size.y; i++)	{
		if (data[lay * data_size.x * data_size.y + i] < 2500){
			data_density[data[lay * data_size.x * data_size.y + i]]++;
			count++;
		}
	}
	for (int i = 0; i < 2500; i++)
		data_density[i] /= count;
}

void Tomo_Data::get_data_density(int x1, int y1, int x2, int y2)
{
	using namespace std;
	if (lay < 0) lay = 0;
	if (lay > data_size.z - 1) lay = data_size.z - 1;

	if (data_density_rectangle) delete data_density_rectangle;
	data_density_rectangle = new double [25000];

	for (int i = 0; i < 25000; i++)
		data_density_rectangle[i] = 0;
	int count = 0;
	for (int j = 0; j < data_size.y; j++)
		for (int i = 0; i < data_size.x; i++)
			if ((j >= min(y1, y2)) && (j <= max(y1, y2)) &&
				(i >= min(x1, x2)) && (i <= max(x1, x2))) {
				data_density_rectangle[data[lay * data_size.x * data_size.y + j*data_size.x + i]]++;
				count++;
			}	
	for (int i = 0; i < 25000; i++)
		data_density_rectangle[i] /= count;
}