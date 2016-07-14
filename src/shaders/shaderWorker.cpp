#include "shaders\shaderWorker.h"

//#include "AllDef.h"
//#include "Draw.h"
#include <windows.h>
//#include "vec.h"
//#include "str.h"

extern int stereo_on;

ShaderWorker::ShaderWorker() :level(0), box1(0), box2(1),
		st(0), //st_i(0), st_n(0), 
		shader()
{
	draw_frame_is = 1;
	ReloadShader();
}

ShaderWorker::~ShaderWorker()
{
	delete shader;
	if (st) delete st;
	//if (st_i) delete st_i;
	//if (st_n) delete st_n;
}

void ShaderWorker::ReloadShader()
{
	if (shader)
		delete shader;
	shader = new GLShader();
	shader->loadFiles("../../../shaders/raycasting.vert", "../../../shaders/raycasting.frag");
	
	//SetAnag(0, 0);
	SetBoundingBox(box1, box2);
	SetLevel(level);
}

//установка положения и ориентации камеры
void ShaderWorker::ApplyCamera(Camera* camera)
{
	shader->use();
	shader->setUniform("pos", camera->GetPosition());
	shader->setUniform("nav", camera->GetNav());
	shader->release();
}

//загрузка исходных объёмных данных в GPU
void ShaderWorker::UploadFieldData(void* data, ivec3 size, int type, vec3 scale)
{
	this->scale = scale;
	box2 = scale;
	if (st)
		delete st;
	
	//в конструкторе происходит создание 3Д текстуры + её инициализация исходными данными
	st = new TextureWorker(size.x, size.y, size.z, 1, data, 1, true, type);
}


//установка изо-значения для изо-поверхности
void ShaderWorker::SetLevel(float level)
{
	this->level = clamp(0.0f, 1.0f, level);

	shader->use();
	shader->setUniform("level1", level);
	shader->release();
}

//установка координат концов ограничивающей коробки
void ShaderWorker::SetBoundingBox(vec3 a, vec3 b)
{
	box1 = a;
	box2 = b;

	shader->use();
	shader->setUniform("box1", box1);
	shader->setUniform("box2", box2);
	shader->setUniform("step_length", 0.003f);
	shader->release();
}

void ShaderWorker::Draw(Camera* camera)
{
	vec3 bb1 = box1, bb2 = box2;
	/*
	//вывод рамки
	if (stereo_on != 1 && draw_frame_is)
	{
		glColor3f(0.5f, 0.5f, 0.5f);
		glDisable(GL_LIGHTING);
		DrawCubeFrame(bb1, bb2);
		glEnable(GL_LIGHTING);
	} */

	shader->use();
	if (st)
		shader->setUniform("f_text", st->GetTexture());
	shader->setUniform("box1", box1);
	shader->setUniform("box2", box2);
	shader->setUniform("scale", scale);
	shader->setUniform("size", vec3(size.x, size.y, size.z));
	//DrawCube(bb1, bb2); //вывод коробки (т.е. наших объёмных данных, т.к. для закрашивания коробки используется шейдер трассировки луча)
	shader->release();
}

float ShaderWorker::GetMinLevel() 
{ 
	return level; 
}
vec3 ShaderWorker::GetMinBox() 
{ 
	return box1; 
}
vec3 ShaderWorker::GetMaxBox() 
{ 
	return box2; 
}


/*
//установить анаглиф типа s для левого(если left) или правого глаза
void ShaderWorker::SetAnag(int s, bool left)
{

	switch (s)
	{
	case 0: // no stereo
		anag[0].set(1, 0, 0);
		anag[1].set(0, 1, 0);
		anag[2].set(0, 0, 1);
		break;
	case 1: // True anaglyh
		if (left)
		{
			anag[0].set(0.299, 0.587, 0.114);
			anag[1].set(0, 0, 0);
			anag[2].set(0, 0, 0);
		}
		else
		{
			anag[0].set(0, 0, 0);
			anag[1].set(0, 0, 0);
			anag[2].set(0.299, 0.587, 0.114);
		}
		break;
	case 2: // Gray anaglyh
		if (left)
		{
			anag[0].set(0.299, 0.587, 0.114);
			anag[1].set(0, 0, 0);
			anag[2].set(0, 0, 0);
		}
		else
		{
			anag[0].set(0, 0, 0);
			anag[1].set(0.299, 0.587, 0.114);
			anag[2].set(0.299, 0.587, 0.114);
		}
		break;
	case 3: // Color anaglyh
		if (left)
		{
			anag[0].set(1, 0, 0);
			anag[1].set(0, 0, 0);
			anag[2].set(0, 0, 0);
		}
		else
		{
			anag[0].set(0, 0, 0);
			anag[1].set(0, 1, 0);
			anag[2].set(0, 0, 1);
		}
		break;
	case 4: // Half-Color anaglyh
		if (left)
		{
			anag[0].set(0.299, 0.587, 0.114);
			anag[1].set(0, 0, 0);
			anag[2].set(0, 0, 0);
		}
		else
		{
			anag[0].set(0, 0, 0);
			anag[1].set(0, 1, 0);
			anag[2].set(0, 0, 1);
		}
		break;
	case 5: // Optimized anaglyh
		if (left)
		{
			anag[0].set(0, 0.7, 0.3);
			anag[1].set(0, 0, 0);
			anag[2].set(0, 0, 0);
		}
		else
		{
			anag[0].set(0, 0, 0);
			anag[1].set(0, 1, 0);
			anag[2].set(0, 0, 1);
		}
		break;
	};

	ps->Use();
	ps->SetVar("RFrom", anag[0]);
	ps->SetVar("GFrom", anag[1]);
	ps->SetVar("BFrom", anag[2]);
	ps->UnUse();
}
*/