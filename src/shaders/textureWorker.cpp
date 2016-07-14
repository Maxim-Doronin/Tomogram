#include "shaders\textureWorker.h"
#pragma comment(lib,"glew32.lib")
#include <windows.h>

TextureWorker::TextureWorker(int width, int height, int depth,
	int pixel_size, void* data3D, int idTexture,
	bool n_linear, int nt_type) :
	width(width),
	height(height),
	depth(depth),
	idTexture(idTexture),
	linear(n_linear), 
	t_type(nt_type)
{
	glActiveTexture(GL_TEXTURE0 + idTexture);
	glEnable(GL_TEXTURE_3D);
	arr = data3D;
	texture_target = GL_TEXTURE_3D;
	if (t_type == GL_FLOAT)
	{
		internal_format = GL_LUMINANCE32F_ARB;
		texture_format = GL_LUMINANCE;//GL_ALPHA;
	}
	else if (t_type == GL_UNSIGNED_BYTE)
		{
			internal_format = GL_LUMINANCE8;
			texture_format = GL_LUMINANCE;
		}
		else
		{
			internal_format = GL_LUMINANCE16F_ARB;
			texture_format = GL_LUMINANCE;
		}

	glGenTextures(1, &texture);
	setupTexture(texture);
	transferToTexture(texture);
	
	glDisable(GL_TEXTURE_3D);
	glActiveTexture(GL_TEXTURE0);
};

TextureWorker::~TextureWorker()
{
	glDeleteTextures(1, &texture);
	glDeleteFramebuffersEXT(1, &fb);
}

GLuint TextureWorker::GetTexture()
{
	return texture;
}

void TextureWorker::setupTexture(GLuint textureID)
{
	// make active and bind
	glBindTexture(texture_target, textureID);
	// turn off filtering and wrap modes

	glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP);
}

void TextureWorker::transferToTexture(GLuint textureID)
{
	if (textureID == -1)
		textureID = texture;
	glBindTexture(texture_target, textureID);

	static PFNGLTEXIMAGE3DPROC glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
	glTexImage3D(texture_target, 0, internal_format, width, height, depth, 0, texture_format, t_type, arr);
}

void TextureWorker::Upload1(int z1, int z2)
{
	glBindTexture(texture_target, texture);

	static PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)wglGetProcAddress("glTexSubImage3D");
	glTexSubImage3D(texture_target, 0, 0, 0, z1, 
			width, height, z2 - z1, texture_format, t_type, 
			((float*)arr) + width*height * 3 * z1);
}

void TextureWorker::Upload(int id, int num)
{
	int step = depth / num;
	Upload1(id*step, (id + 1)*step);
}









