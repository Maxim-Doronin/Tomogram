#include "rayCasting.h"

inline float deg2rad(const float &deg) 
{ 
	return deg * pi / 180; 
} 

float Vec3f::getLength()
{
	return sqrt(x * x + y * y + z * z);
}

void Vec3f::normalize()
{
	float length = getLength();
	x /= length;
	y /= length;
	z /= length;
}

float Vec3f::scalarMult(const Vec3f &vec)
{
	return x*vec.x + y*vec.y + z*vec.z;
}

float Vec3f::getCos(Vec3f &vec)
{
	return scalarMult(vec) / (vec.getLength() * this->getLength());
}

RayCasting::RayCasting(TomoData *data, int minRange, int maxRange)
{
	this->data = data;
	
	width  = data->dataSize.x;
	height = data->dataSize.z;
	depth  = data->dataSize.y;
	
	option = new Options;
	option->width = width;
	option->height = depth;
	option->beta = 30;
	option->radius = 1000;
	option->minRange = minRange;
	option->maxRange = maxRange;

	rgba = new RGBA[6000];
	for (int i = 0; i < minRange; i++)
	{
		rgba[i].red = 0;
		rgba[i].green = 0;
		rgba[i].blue = 0;
		rgba[i].alpha = 0;
	}
	for (int i = maxRange; i < 6000; i++)
	{
		rgba[i].red = 0;
		rgba[i].green = 0;
		rgba[i].blue = 0;
		rgba[i].alpha = 0;
	}
	for (int i = minRange; i < maxRange; i++)
	{
		rgba[i].red = 255;
		rgba[i].green = 250;
		rgba[i].blue = 230;
		rgba[i].alpha = 1;
	}
	/*for (int i = 2090; i < 2093; i++)
	{
		rgba[i].red = 0xAE;
		rgba[i].green = 0x8A;
		rgba[i].blue = 0x9E;
		rgba[i].alpha = 0.05;
	}*/
}

RayCasting::~RayCasting()
{
	delete option;
	delete []rgba;
}

RGBA& RayCasting::ray(Vec3f origin, Vec3f dir, RGBA& color, Options* option, int x, int y)
{
	float curX = origin.x;
	float curY = origin.y;
	float curZ = origin.z / data->scale.z;

	float radius = option->radius;
	/*float D = 4*(dir.x * curX + dir.y * curY + dir.z * curZ) *
				(dir.x * curX + dir.y * curY + dir.z * curZ) - 
			  4*(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) * 
				(curX * curX + curY * curY + curZ * curZ - volumeRadius * volumeRadius);
	if (D < 0)  return color;
	float t1 = (-2*(dir.x * curX + dir.y * curY + dir.z * curZ) - sqrt(D)) / 
				(2 * (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
	curX += t1 * dir.x;
	curY += t1 * dir.y;
	curZ -= t1 * dir.z / data->scale.z; */
		
	int w = width / 2;
	int h = height / 2;
	int d = depth / 2;

	while  ((curX <= -w) || (curX >= w - 1) ||
			(curY <= -d) || (curY >= d - 1) ||
			(curZ <= -h) || (curZ >= h - 1))
	{
		curX += dir.x * 10;
		curY += dir.y * 10;
		curZ += dir.z * 10 / data->scale.z;
		if ((abs(curX) > radius) || (abs(curY) > radius) || (abs(curZ) > radius / data->scale.z))
			break;
	}

	while  ((curX > -w) && (curX < w - 1) &&
			(curY > -d) && (curY < d - 1) &&
			(curZ > -h) && (curZ < h - 1) &&
			(color.alpha < 1 ))
	{
		ushort voxel = interpolation(w-curX, d+curY, h+curZ);
		if (voxel >= option->minRange || voxel < option->maxRange)
		{
			Vec3f grad = calculateGradient(w-curX, d+curY, h+curZ); 
			float koef = abs(grad.getCos(dir)); 
			RGBA dens = rgba[voxel];
			color.red += dens.red * dens.alpha * koef;
			color.green += dens.green * dens.alpha * koef;
			color.blue += dens.blue * dens.alpha * koef;
			color.alpha += dens.alpha;
		}
		
		curX += dir.x;
		curY += dir.y;
		curZ += dir.z / data->scale.z;
	}
	return color;
}

inline ushort RayCasting::getDensity(int x, int y, int z)
{
	if (x < 0 || y < 0 || z < 0 || x >= width || y >= depth || z >= height)
		return 0;
	return data->data3D[depth * width * z + width * y + x];
}

ushort RayCasting::interpolation(float x, float y, float z)
{
	int x1 = (int)x;
	int x2 = x1 + 1;
	int y1 = (int)y;
	int y2 = y1 + 1;
	int z1 = (int)z;
	int z2 = z1 + 1;
	return getDensity(x1, y1, z1)*(x2-x)*(y2-y)*(z2-z)/(x2-x1)/(y2-y1)/(z2-z1) + 
		   getDensity(x1, y1, z2)*(x2-x)*(y2-y)*(z-z1)/(x2-x1)/(y2-y1)/(z2-z1) + 
		   getDensity(x1, y2, z1)*(x2-x)*(y-y1)*(z2-z)/(x2-x1)/(y2-y1)/(z2-z1) + 
		   getDensity(x1, y2, z2)*(x2-x)*(y-y1)*(z-z1)/(x2-x1)/(y2-y1)/(z2-z1) + 
		   getDensity(x2, y1, z1)*(x-x1)*(y2-y)*(z2-z)/(x2-x1)/(y2-y1)/(z2-z1) + 
		   getDensity(x2, y1, z2)*(x-x1)*(y2-y)*(z-z1)/(x2-x1)/(y2-y1)/(z2-z1) + 
		   getDensity(x2, y2, z1)*(x-x1)*(y-y1)*(z2-z)/(x2-x1)/(y2-y1)/(z2-z1) + 
		   getDensity(x2, y2, z2)*(x-x1)*(y-y1)*(z-z1)/(x2-x1)/(y2-y1)/(z2-z1) ;
}

Vec3f RayCasting::calculateGradient(float x, float y, float z)
{
	float x1 = x - 0.4;
	float x2 = x + 0.4;
	float y1 = y - 0.4;
	float y2 = y + 0.4;
	float z1 = z - 0.4 / data->scale.z;
	float z2 = z + 0.4 / data->scale.z;
	Vec3f result;
	result.x = interpolation(x2, y, z) - interpolation(x1, y, z);
	result.y = interpolation(x, y2, z) - interpolation(x, y1, z);
	result.z = interpolation(x, y, z2) - interpolation(x, y, z1);
	result.normalize();
	return result;
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
			dir.z = -sin(psi) - y * cos(psi);

			dir.normalize();
			dir.x /= 2;
			dir.y /= 2;
			dir.z /= 2;

			data->dataColor2D[j * depth + i] = ray(origin, dir, color, option, i, j);
		}
}

//!!!нормали, трилинейная интерполяция, цвет*косинус угла между вектором камеры и нормали = итоговый цвет.
//добавить на гистограмму кривые, задающие непрозрачность
//туда же можно добавить задание цвета
//построить изоповерхность? добавить слайдер-порог изоповерхности
//изоповерхности
//оптимизированная формула для градиента
//DONE TODO: fix non-maximum supresiion 
//TODO: non-recursive implementation in tracing edges