#include "shaders/camera.h"

#include <cmath>
using namespace glm;


Camera::Camera() 
{
	distance = 0;
	center = vec3(0);
	top = vec3(0, 0, 1);
	left = vec3(-1, 0, 0);
	xRotation = 0;
	yRotation = 0;
	updateVecs();
}

void Camera::SetupPosition()
{
	glLoadIdentity();
	glTranslated(0.0, 0.0, -distance);
	glRotated(-xRotation * PI180, 1.0, 0.0, 0.0);
	glRotated(-yRotation * PI180 + 180, 0.0, 1.0, 0.0);
	glTranslated(-center.x, -center.y, -center.z);
}

void Camera::SetupProjection(float angle, float near, float far, int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, width / (double)height, near, far);
	glMatrixMode(GL_MODELVIEW);
}

void Camera::SetCenter(vec3 center)
{
	this->center = center;
	updateVecs();
}
void Camera::SetDistance(float distantion)
{
	this->distance = distantion;
	updateVecs();
}
float Camera::GetDistance() const
{
	return distance;
}

vec3 Camera::GetCenter() 
{ 
	return center; 
}
vec3 Camera::GetPosition() 
{ 
	return position; 
}
vec3 Camera::GetNav() 
{ 
	return direction;
}
vec3 Camera::GetLeft() 
{ 
	return left; 
}
vec3 Camera::GetTop() 
{ 
	return top; 
}

void Camera::SetXRot(float xRotation)
{ 
	this->xRotation = xRotation;
	updateVecs(); 
}
void Camera::SetYRot(float yRotation)
{ 
	this->yRotation = yRotation;
	updateVecs();
}
float Camera::GetXRot()
{ 
	return xRotation; 
}
float Camera::GetYRot()
{ 
	return yRotation; 
}

void Camera::Rotate(float rx, float ry)
{
	vec3 old_pos = position;
	yRotation += ry;
	xRotation += rx;
	updateVecs();
	center += old_pos - position;
	position = old_pos;
}

void Camera::updateVecs()
{
	float sinx = sin(xRotation);
	float cosx = cos(xRotation);
	float siny = sin(yRotation);
	float cosy = cos(yRotation);
/*
	direction = vec3(sinx * cosy, cosx * cosy, siny);
	position = direction * (-distance);
	position += center;
	*/
	
	direction = vec3(siny * cosx, sinx, cosy * cosx);
	top = vec3(-siny * sinx, cosx, -cosy * sinx);
	left = vec3(-cosy, 0, siny);
	
	position = direction * (-distance);
	position += center;
	
}


