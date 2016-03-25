#include "rayCasting.h"

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
	option->beta = 10;
	option->radius = 3000;

	rgba = new RGBA[5000];
	for (int i = 0; i < 2300; i++)
	{
		rgba[i].red = 0;
		rgba[i].green = 0;
		rgba[i].blue = 0;
		rgba[i].alpha = 0;
	}
	for (int i = 3800; i < 5000; i++)
	{
		rgba[i].red = 0;
		rgba[i].green = 0;
		rgba[i].blue = 0;
		rgba[i].alpha = 0;
	}
	for (int i = 2300; i < 3800; i++)
	{
		rgba[i].red = 255;
		rgba[i].green = 255;
		rgba[i].blue = 255;
		rgba[i].alpha = 0.01;
	}
}

RGBA& RayCasting::ray(Vec3f origin, Vec3f dir, RGBA& color, Options* option, int x, int y)
{
	float curX = origin.x;
	float curY = origin.y;
	float curZ = origin.z / data->scale.z;

	float radius = option->radius;
	
	while  ((curX <= -width / 2) || (curX >= width / 2 - 1) ||
			(curY <= -depth / 2) || (curY >= depth / 2 - 1) ||
			(curZ <= -height / 2) || (curZ >= height / 2 - 1))
	{
		curX += dir.x * 5;
		curY += dir.y * 5;
		curZ -= dir.z * 5 / data->scale.z;
		if ((abs(curX) > radius) || (abs(curY) > radius) || (abs(curZ) > radius / data->scale.z))
			break;
	}

	while  ((curX > -width / 2) && (curX < width / 2 -1) &&
			(curY > -depth / 2) && (curY < depth / 2 - 1) &&
			(curZ > -height / 2) && (curZ < height / 2 - 1) &&
			(color.alpha < 1 ))
	{
		int idX = width / 2 - floor(curX + 0.5);
		int idY = depth / 2 + floor(curY + 0.5);
		int idZ = height / 2 +floor(curZ + 0.5);
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
		
		curX += dir.x;
		curY += dir.y;
		curZ -= dir.z / data->scale.z;
	}
	return color;
}

void RayCasting::calculateOrigin(Vec3f &origin, float phi, float psi)
{
	float r = option->radius;
	origin.x = r * sin(phi) * cos(psi);
	origin.y = -r * cos(phi) * cos(psi); 
	origin.z = r * sin(psi);
}

void RayCasting::render(float phi, float psi)
{
	frameBuffer = new RGBA[option->height * option->height];
	pixel = frameBuffer;
	
	float scale = tan(deg2rad(option->beta * 0.5)); 
	float imageAspectRatio = option->width / (float)option->height; 
	
	phi = deg2rad(phi);
	psi = deg2rad(psi);
	Vec3f origin;
	calculateOrigin(origin, phi, psi);
	
	#pragma omp parallel for
	for (int j = 0; j < option->height; j++)
		#pragma omp parallel for
		for (int i = 0; i < option->width; i++)
		{
			RGBA color;
			color.red = 0;
			color.green = 0;
			color.blue = 0;
			color.alpha = 0;

			float x = (2 * (i + 0.5) / (float)option->width - 1) * imageAspectRatio * scale; 
            float y = (1 - 2 * (j + 0.5) / (float)option->height) * scale;
			
			Vec3f dir; 

			dir.x = x * cos(phi) - 1 * sin(phi) * cos(psi) + y * sin(phi) * sin(psi);
			dir.y = x * sin(phi) + 1 * cos(phi) * cos(psi) - y * cos(phi) * sin(psi); 
			dir.z = sin(psi) + y * cos(psi);

			float dirLength = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
			dir.x /= dirLength;
			dir.y /= dirLength;
			dir.z /= dirLength;

			//*(pixel++) = ray(origin, dir, option);
			data->data2D[j * depth + i] = ray(origin, dir, color, option, i, j).red;
		}
}