#pragma once

#include <QGLWidget>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>
#include <cstdlib>
#include "TomoData.h"

#define scaleWindow 100

class TomoOGL : public QGLWidget
{
	Q_OBJECT

public:
	TomoOGL(uchar*&, int, int, QWidget *parent = 0);
	~TomoOGL();

	void upd(uchar*&, int, int);
	void upd(RGBA*&, int, int);
private:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	QPoint origin;
	QRubberBand *rubberBand;

	virtual void mouseMoveEvent(QMouseEvent *we); 
	virtual void mousePressEvent(QMouseEvent *we);
	virtual void mouseReleaseEvent(QMouseEvent *we);

	uchar* srcWB;
	RGBA* srcRGBA;
	int width;
	int height;
	int isColor;
signals:
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);
};
