#include "drawShader.h"
#include <qdebug.h>
#include <QTextStream>

inline float deg2rad(const float &deg)
{
	return deg * pi / 180;
}

DrawShader::DrawShader(TomoData *data, int width, int height, QWidget *parent)
	: QGLWidget(parent), xRot(0), yRot(0), dist(1.5f)
{
	this->data = data;
	this->width = width;
	this->height = height;
	setMinimumHeight(height);
	setMinimumWidth(width);
}

DrawShader::~DrawShader() 
{
	delete shaderWorker;
}

void DrawShader::initializeGL()
{
	
	//glewExperimental = GL_TRUE;
	glClearColor(0, 0, 0, 0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shaderWorker = new ShaderWorker();
	
	camera.SetupProjection(15, 0.1f, 60.0f, 0.0, 0.0, width, height);
	camera.SetDistance(1.5f);
	camera.SetCenter(glm::vec3(0.5f));
	
	
	int type = GL_SHORT;
	glm::vec3 scale = glm::vec3(1, data->dataSize.y / float(data->dataSize.x), data->dataSize.z / float(data->dataSize.x));
	glm::ivec3 size(data->dataSize.x, data->dataSize.y, data->dataSize.z);

	shaderWorker->size = size;
	shaderWorker->UploadFieldData(data->data3D, size, type, scale);
	shaderWorker->SetBoundingBox(glm::vec3(0), scale);

	shaderWorker->SetLevel(0.06f);
	
}

void DrawShader::resizeGL(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(0.0, this->width, 0.0, this->height, -1.0, 1.0);
	//glViewport(0, 0, (GLint)width, (GLint)height);
}

void DrawShader::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.SetXRot(xRot);
	camera.SetYRot(yRot);
	camera.SetDistance(dist);
	camera.SetupPosition();
	shaderWorker->ApplyCamera(&camera);
	shaderWorker->Draw(&camera);
}

void DrawShader::upd(float xRot, float yRot, float dist)
{
	this->xRot = yRot*0.01;
	this->yRot = xRot*0.01;
	this->dist = dist;
	updateGL();
}