#include "tomoOGL.h"

TomoOGL::TomoOGL(char* file, QWidget *parent)
	: QGLWidget(parent)
{
	t = new Tomo_Data(file);
	lay = 0;
	flag = 0;
	lowIdx = 0;
	hiIdx = 6000;
	setFixedHeight(513);
	setFixedWidth(513);

}

TomoOGL::~TomoOGL()
{
	delete t;
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
	
	glOrtho(0.0, t->data_size.x, 0.0, t->data_size.y, -1.0, 1.0);
	glViewport(0, 0, (GLint)width, (GLint)height);
}

void TomoOGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 

	uchar tint;

	if (flag) t->pixels_delete();
	t->get_lay(lay, lowIdx, hiIdx);
	/*glBegin(GL_POINTS);
		for (int i = 0; i < t->data_size.y; i++)	
			for (int j = 0; j < t->data_size.x; j++)
			{
				tint = t->data_pixels[i * t->data_size.x + j];
				glColor3ub(tint, tint, tint);
				glVertex2i(j, t->data_size.y - i);
			}
	glEnd();*/

	for (int i = 0; i < t->data_size.x - 1; i++)
	{
		glBegin(GL_QUAD_STRIP);
			for (int j = 0; j < t->data_size.y * 2; j++)
			{
				tint = t->data_pixels[i * t->data_size.x + t->data_size.x * (j%2 == 0 ? 1 : 0) + j/2];
				glColor3ub(tint, tint, tint);
				glVertex2i(j/2, j%2 == 0 ? i+1 : i);
			}
		glEnd();
	}

	flag = 1;
}

void TomoOGL::upd(int _lay, int _lowIdx, int _hiIdx)
{
	this->lay = _lay;
	this->lowIdx = _lowIdx;
	this->hiIdx = _hiIdx;
	updateGL();
}