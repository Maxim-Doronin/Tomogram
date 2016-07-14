#pragma once

#ifdef _DEBUG
	#include "C:\Users\asus\Tomogram\glew\include\GL\glew.h"
#else
	#include "GL/glew.h"
#endif
#include "glm/glm.hpp"

#define PI180 57.295779f

class Camera {
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 top;
	glm::vec3 left;
	glm::vec3 center;
	float xRotation, yRotation;
	float distance;

	void updateVecs();

public:
	Camera();

	void SetupPosition();
	void SetupProjection(float angle, float mynear, float myfar, int x, int y, int width, int height);

	void SetCenter(glm::vec3 center);
	void SetDistance(float distantion);
	float GetDistance() const;

	glm::vec3 GetCenter();
	glm::vec3 GetPosition();
	glm::vec3 GetNav();
	glm::vec3 GetLeft();
	glm::vec3 GetTop();

	void SetXRot(float xRotation);
	void SetYRot(float yRotation);
	float GetXRot();
	float GetYRot();

	void Rotate(float rx, float ry);
};