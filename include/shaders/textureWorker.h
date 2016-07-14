#pragma once

#include <vector>
#include <assert.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <ctime>

#include "GLShaders.h"

class TextureWorker {
public:
	TextureWorker(int width, int height, int depth, 
			int pixel_size, void* data3D, int idTexture,
			bool n_linear, int nt_type);
	~TextureWorker();
	GLuint GetTexture();
	
	int idTexture;
	
	void Upload1(int z1, int z2);
	void Upload(int id, int num);
private:
	void setupTexture(GLuint textureID);
	void transferToTexture(GLuint textureID = -1);
	
	GLuint texture;
	
	int width, height, depth;
	bool linear;

	void *arr;

	GLenum t_type;
	GLenum texture_target;
	GLenum internal_format;
	GLenum texture_format;

	GLuint fb;
};

// �������� 3� ��������
// w, h, d - �������
// pixel_size - ���-�� ������� (�� 1 �� 4, ������� ����� ������ ������ �������)
// data - �������� ������ 
// n_ID_to_use - ������ (�� 7) 
// n_linear - ������ �� �������� ���������� (������������)
// nt_type - ��� ������