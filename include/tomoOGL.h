#pragma once

#include <QGLWidget>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>
#include <cstdlib>
#include "tomo_data.h"


class TomoOGL : public QGLWidget
{
	Q_OBJECT

public:
	TomoOGL(Tomo_Data*&, QWidget *parent = 0);
	~TomoOGL();

	void upd();
private:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	QPoint origin;
	QRubberBand *rubberBand;

	virtual void mouseMoveEvent(QMouseEvent *we); 
	virtual void mousePressEvent(QMouseEvent *we);
	virtual void mouseReleaseEvent(QMouseEvent *we);

	Tomo_Data *tD;
signals:
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);
};
