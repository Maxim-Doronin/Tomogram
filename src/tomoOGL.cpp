#include "tomoOGL.h"

TomoOGL::TomoOGL(uchar*& src, int w, int h, QWidget *parent)
	: QGLWidget(parent)
{
	this->src    = src;
	this->width  = w;
	this->height = h;
	setMinimumHeight(h);
	setMinimumWidth(w);
	rubberBand = 0;
}

TomoOGL::~TomoOGL()
{
	delete rubberBand;
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
	
	glOrtho(0.0, this->width, 0.0, this->height, -1.0, 1.0);
	glViewport(0, 0, (GLint)width, (GLint)height);
}

void TomoOGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 

	uchar tint;

	
	/*glBegin(GL_POINTS);
		for (int i = 0; i < width; i++)	
			for (int j = 0; j < height; j++)
			{
				tint = data3D[i * width + j];
				glColor3ub(tint, tint, tint);
				glVertex2i(j, height - i);
			}
	glEnd();*/

	for (int i = 0; i < height - 1; i++){
		glBegin(GL_QUAD_STRIP);
			for (int j = 0; j < width * 2; j++) {
				tint = src[i * width + width * (j%2 == 0 ? 1 : 0) + j/2];
				glColor3ub(tint, tint, tint);
				glVertex2i(j/2, j%2 == 0 ? i+1 : i);
			}
		glEnd();
	}
}

void TomoOGL::upd(uchar*& src, int w, int h)
{
	this->src    = src;
	this->width  = w;
	this->height = h;
	updateGL();
}

void TomoOGL::mouseMoveEvent(QMouseEvent *we)
{
	rubberBand->setGeometry(QRect(origin, we->pos()).normalized());
}

void TomoOGL::mousePressEvent(QMouseEvent *we)
{	
	origin = we->pos();
	if (!rubberBand)
		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	rubberBand->hide();
	//QPalette palette;
	//palette.setBrush(QPalette::Window, QBrush(QColor(0,0,0,0)));
	//palette.setBrush(QPalette::Highlight, QBrush(QColor(1,1,1,1)));
	//palette.setBrush(QPalette::Base, QBrush(Qt::red));
	//rubberBand->setPalette(palette);
	//rubberBand->setWindowOpacity(0.0);
	//rubberBand->setWindowFlags(Qt::ToolTip);
	rubberBand->setGeometry(QRect(origin, QSize()));
	rubberBand->show();
	emit mousePressed(we->x(), we->y());
}

void TomoOGL::mouseReleaseEvent(QMouseEvent *we)
{
	rubberBand->hide();
	emit mouseReleased(we->x(), we->y());
}


