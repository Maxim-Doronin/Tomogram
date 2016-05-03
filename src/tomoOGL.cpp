#include "tomoOGL.h"

TomoOGL::TomoOGL(uchar*& src, int w, int h, QWidget *parent)
	: QGLWidget(parent)
{
	this->srcWB    = src;
	this->width  = w;
	this->height = h;
	setMinimumHeight(h + 300);
	setMinimumWidth(w + 300);
	rubberBand = 0;
	isColor = 0;
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
	RGBA pixel;
//#define POINTS
#ifdef POINTS
	glBegin(GL_POINTS);
		for (int i = 0; i < width; i++)	
			for (int j = 0; j < height; j++)
			{
				tint = srcWB[i * width + j];
				glColor3ub(tint, tint, tint);
				glVertex2i(j, height - i);
			}
	glEnd();
#else
	if (isColor)
		for (int j = 0; j < height - 1; j++){
			glBegin(GL_QUAD_STRIP);
				for (int i = 0; i < width * 2; i++) {
					pixel = srcRGBA[j * width + width * (i%2 == 0 ? 1 : 0) + i/2];
					glColor4ub(pixel.red, pixel.green, pixel.blue, 255 * pixel.alpha);
					glVertex2i(i/2, i%2 == 0 ? j+1 : j);
				}
			glEnd();
		}
	else
		for (int j = 0; j < height - 1; j++){
			glBegin(GL_QUAD_STRIP);
				for (int i = 0; i < width * 2; i++) {
					tint = srcWB[j * width + width * (i%2 == 0 ? 1 : 0) + i/2];
					glColor3ub(tint, tint, tint);
					glVertex2i(i/2, i%2 == 0 ? j+1 : j);
				}
			glEnd();
		}
#endif
}

void TomoOGL::upd(uchar*& src, int w, int h)
{
	isColor = 0;
	this->srcWB    = src;
	this->width  = w;
	this->height = h;
	updateGL();
}

void TomoOGL::upd(RGBA*& src, int w, int h)
{
	isColor = 1;
	this->srcRGBA = src;
	this->width = w;
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


