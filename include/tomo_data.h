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
private:
	uchar* transfer_function();
public: 
	Data_Size data_size;
	Scale scale;
	
	short* data;			//�������� ������
	uchar* data_lay;		//������������ ������� ������

	short* data_density;	//������ ��� ����������	

	int lay;
	int lowIdx;
	int hiIdx;
public:
	Tomo_Data(char*);
	virtual ~Tomo_Data();
	
	uchar* get_data_lay();

	void get_data_density();
};