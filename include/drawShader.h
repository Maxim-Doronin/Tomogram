#pragma once

#include "shaders\GLShaders.h"
#include "shaders\camera.h"
#include "shaders\shaderWorker.h"
#include <QGLWidget>
#include "TomoData.h"
#define pi 3.14159265358979323846

class DrawShader : public QGLWidget {
	
public:
	DrawShader(TomoData *data, int width, int height, QWidget *parent = 0);
	~DrawShader();

	void upd(float xRot, float yRot, float dist);

private:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	int width;
	int height;

	float xRot, yRot;
	float dist;

	TomoData *data;
	Camera camera;
	ShaderWorker *shaderWorker;
};