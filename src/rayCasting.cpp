#include "rayCasting.h"

RayCasting::RayCasting(TomoData *data)
{
	this->data = data;
	eyeDist = 10000000;
	step = 1;
	width  = data->dataSize.x;
	height = data->dataSize.z;
	depth  = data->dataSize.y;

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

void RayCasting::getInc(int x, int y)
{
	float ctgB = (float)eyeDist / abs((float)depth / 2 - y);
	float ctgP = abs((float)width / 2 - y) / abs((float)depth / 2 - x);

	if (abs((float)depth / 2 - y) == 0) {
		inc->dz = 0;
		float alpha  = std::atan2(abs((float)depth / 2 - x), (float)eyeDist);
		inc->dx = step * sin(alpha);
		inc->dy = step * cos(alpha);
		return;
	}
	if (abs((float)depth / 2 - x) == 0) {
		inc->dx = 0;
		float alpha  = std::atan2(abs((float)depth / 2 - y), (float)eyeDist);
		inc->dz = step * sin(alpha);
		inc->dy = step * cos(alpha);
		return;
	}
	inc->dz = sqrt(step*step/(1 + ctgP*ctgP + ctgB*ctgB));
	inc->dx = (ctgP * inc->dz);
	inc->dy = (ctgB * inc->dz);
	
}

RGBA& RayCasting::ray(int x, int y)
{
	getInc(x, y);
	float curX = x;
	float curY = 0;
	float curZ = y / data->scale.z;
	RGBA color;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
	color.alpha = 0;

	int navX = (x < width / 2)  ? -1 : 1;
	int navZ = (y < height / 2) ? -1 : 1;

	while (!((curX >= width-1) ||(curX < 0) ||
			(curY >= depth-1) ||(curY < 0) ||
			(curZ >= height-1)||(curZ < 0) ||
			(color.alpha >= 1 )))
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
		
		curX += inc->dx * navX;
		curY += inc->dy;
		curZ += inc->dz * navZ / data->scale.z;
	}
	return color;
}

void RayCasting::render()
{
	#pragma omp parallel for
	for (int i = 0; i < depth; i++)
		for (int j = 0; j < width; j++)
			data->data2D[i * depth + j] = ray(j, i).red;
}