#pragma once

#include "camera.h"

#include "textureWorker.h"
#include "GLShaders.h"
class GLShader;

class ShaderWorker {
public:
	ShaderWorker();
	~ShaderWorker();
	void ReloadShader();
	void ApplyCamera(Camera* c);
	void UploadFieldData(void* data, ivec3 size, int type, vec3 scale);
	void SetLevel(float level);
	void SetBoundingBox(vec3 a, vec3 b);

	void Draw(Camera* c);
	
	//	void UpdateUniforms();
	
	float GetMinLevel();
	vec3 GetMinBox();
	vec3 GetMaxBox();

	//void SetAnag(int s, bool left);

	bool draw_frame_is;
	ivec3 size;
private:

	vec3 anag[3];
	
	GLShader *shader;

	TextureWorker* st;
	//TextureWorker* st_i;
	//TextureWorker* st_n;

	float level;
	vec3 box1, box2;
	vec3 scale;
};