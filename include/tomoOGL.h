#pragma once

#include <QGLWidget>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>
#include <cstdlib>
#include "TomoData.h"


class TomoOGL : public QGLWidget
{
	Q_OBJECT

public:
	TomoOGL(uchar*&, int, int, QWidget *parent = 0);
	~TomoOGL();

	void upd(uchar*&, int, int);
private:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	QPoint origin;
	QRubberBand *rubberBand;

	virtual void mouseMoveEvent(QMouseEvent *we); 
	virtual void mousePressEvent(QMouseEvent *we);
	virtual void mouseReleaseEvent(QMouseEvent *we);

	uchar* src;
	int width;
	int height;
signals:
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);
};
