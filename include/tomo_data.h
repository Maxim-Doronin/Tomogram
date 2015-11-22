#pragma once
#include <fstream>
#include <qglobal.h>

struct Data_Size
{
	int x;
	int y;
	int z;
};

struct Scale
{ 
	float x;
	float y;
	float z;
};

class Tomo_Data
{
public:
	Data_Size data_size;
	Scale scale;
	short* data;		//данные
	uchar* data_pixels; //отображаемые цвета

	short minTint;
	short maxTint;
public:
	Tomo_Data(char*);
	~Tomo_Data();
	uchar* get_lay(int &lay, short lowIdx, short hiIdx);
	uchar* get_lay(int &lay);
	uchar* transfer_function(int lay, short lowIdx, short hiIdx);

	
	void pixels_delete();

};