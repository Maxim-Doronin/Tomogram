#include "tomoOGL.h"

TomoOGL::TomoOGL(Tomo_Data *&tD, QWidget *parent)
	: QGLWidget(parent)
{
	this->tD = tD;
	setMinimumHeight(513);
	setMinimumWidth(513);
}

TomoOGL::~TomoOGL()
{
}

void TomoOGL::initializeGL()
{
	qglClearColor(Qt::black);
	glEnable(GL_CULL_FACE);
	glEnable(GL_POLYGON_SMOOTH);
}

void TomoOGL::resizeGL(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0.0, tD->data_size.x, 0.0, tD->data_size.y, -1.0, 1.0);
	glViewport(0, 0, (GLint)width, (GLint)height);
}

void TomoOGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 

	uchar tint;

	tD->get_data_lay();
	/*glBegin(GL_POINTS);
		for (int i = 0; i < tD->data_size.y; i++)	
			for (int j = 0; j < tD->data_size.x; j++)
			{
				tint = tD->data_lay[i * tD->data_size.x + j];
				glColor3ub(tint, tint, tint);
				glVertex2i(j, tD->data_size.y - i);
			}
	glEnd();*/

	for (int i = 0; i < tD->data_size.x - 1; i++){
		glBegin(GL_QUAD_STRIP);
			for (int j = 0; j < tD->data_size.y * 2; j++) {
				tint = tD->data_lay[i * tD->data_size.x + tD->data_size.x * (j%2 == 0 ? 1 : 0) + j/2];
				glColor3ub(tint, tint, tint);
				glVertex2i(j/2, j%2 == 0 ? i+1 : i);
			}
		glEnd();
	}
}

void TomoOGL::upd()
{
	updateGL();
}

