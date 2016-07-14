#pragma once

#include "camera.h"
#include "textureWorker.h"
#include "GLShaders.h"

class ShaderWorker {
public:
	ShaderWorker();
	~ShaderWorker();
	void ReloadShader();
	void ApplyCamera(Camera* c);
	void UploadFieldData(void* data, glm::ivec3 size, int type, glm::vec3 scale);
	void SetLevel(float level);
	void SetBoundingBox(glm::vec3 a, glm::vec3 b);

	void Draw(Camera* c);
	
	//	void UpdateUniforms();
	
	float GetMinLevel();
	glm::vec3 GetMinBox();
	glm::vec3 GetMaxBox();

	//void SetAnag(int s, bool left);

	bool draw_frame_is;
	glm::ivec3 size;
private:

	glm::vec3 anag[3];
	
	GLShader *shader;

	TextureWorker* st;
	//TextureWorker* st_i;
	//TextureWorker* st_n;

	float level;
	glm::vec3 box1, box2;
	glm::vec3 scale;
};