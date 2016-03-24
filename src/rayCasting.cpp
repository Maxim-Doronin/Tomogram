#include "rayCasting.h"

inline float clamp(const float &low, const float &hi, const float &v) 
{ 
	return std::max(low, std::min(hi, v)); 
} 

inline float deg2rad(const float &deg) 
{ 
	return deg * M_PI / 180; 
} 

RayCasting::RayCasting(TomoData *data)
{
	this->data = data;
	
	width  = data->dataSize.x;
	height = data->dataSize.z;
	depth  = data->dataSize.y;
	
	option = new Options;
	option->width = width;
	option->height = depth;
	option->alpha = 20;

	inc = new Increments;

	rgba = new RGBA[5000];
	for (int i = 0; i < 2200; i++)
	{
		rgba[i].red = 0;
		rgba[i].green = 0;
		rgba[i].blue = 0;
		rgba[i].alpha = 0;
	}
	for (int i = 3300; i < 5000; i++)
	{
		rgba[i].red = 0;
		rgba[i].green = 0;
		rgba[i].blue = 0;
		rgba[i].alpha = 0;
	}
	for (int i = 2200; i < 3300; i++)
	{
		rgba[i].red = 255;
		rgba[i].green = 255;
		rgba[i].blue = 255;
		rgba[i].alpha = 0.01;
	}
}

RGBA& RayCasting::ray(Vec3f origin, Vec3f dir, Options* option, int x, int y)
{
	float curX = option->width - x;
	float curY = 0;
	float curZ = y / data->scale.z;
	
	RGBA color;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
	color.alpha = 0;

	while (((curX < width-1) &&(curX >= 0) &&
			(curY < depth-1) &&(curY >= 0) &&
			(curZ < height-1)&&(curZ >= 0) &&
			(color.alpha < 1 )))
	{
		int idX = floor(curX + 0.5);
		int idY = floor(curY + 0.5);
		int idZ = floor(curZ + 0.5);
		int id = depth * width * idZ + width * idY + idX;
		short vertex = data->data3D[id];
		if (vertex < 5000)
		{
			RGBA ver = rgba[vertex];
			color.red += ver.red * ver.alpha;
			color.green += ver.green * ver.alpha;
			color.blue += ver.blue * ver.alpha;
			color.alpha += ver.alpha;
		}
		
		curX -= dir.x;
		curY += dir.z;
		curZ -= dir.y / data->scale.z;
	}
	return color;
}

void RayCasting::render()
{
	frameBuffer = new RGBA[option->height * option->height];
	pixel = frameBuffer;
	float scale = tan(deg2rad(option->alpha * 0.5)); 
    float imageAspectRatio = option->width / (float)option->height; 

	Vec3f origin;
	origin.x = 0; origin.y = 0; origin.z = 0;
#pragma omp parallel for
	for (int j = 0; j < option->height; j++)
		for (int i = 0; i < option->width; i++)
		{
			float x = (2 * (i + 0.5) / (float)option->width - 1) * imageAspectRatio * scale; 
            float y = (1 - 2 * (j + 0.5) / (float)option->height) * scale;
			Vec3f dir; 
			float dirLength = sqrt(x*x + y*y + 1);
			dir.x = x / dirLength, dir.y = y / dirLength; dir.z = 1 / dirLength;
			//*(pixel++) = ray(origin, dir, option);
			data->data2D[j * depth + i] = ray(origin, dir, option, i, j).red;
		}
			
}