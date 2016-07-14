#pragma once

#include "C:\Users\asus\Tomogram\glew\include\GL\glew.h"
//#include "GL/glew.h"
#include "glm/glm.hpp"
using namespace glm;
#define PI180 57.295779f

class Camera {
private:
	vec3 position;
	vec3 navigation;
	vec3 top;
	vec3 left;
	vec3 center;
	float xRotation, yRotation;
	float distantion;

	void updateVecs();

public:
	Camera();

	void SetupPosition();
	void SetupProjection(float angle, float mynear, float myfar, int x, int y, int width, int height);

	void SetCenter(vec3 center);
	void SetDistance(float distantion);
	float GetDistance() const;

	vec3 GetCenter();
	vec3 GetPosition();
	vec3 GetNav();
	vec3 GetLeft();
	vec3 GetTop();

	void SetXRot(float xRotation);
	void SetYRot(float yRotation);
	float GetXRot();
	float GetYRot();

	void Rotate(float rx, float ry);
};