#pragma once

#include <QGLWidget>
#include <cstdlib>
#include "tomo_data.h"

class TomoOGL : public QGLWidget
{
	Q_OBJECT

public:
	TomoOGL(char* file, QWidget *parent = 0);
	~TomoOGL();
	void upd(int _lay, int _lowIdx, int _hiIdx);

private:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	int lay;		//отображаемый слой
	int flag;
	int lowIdx;
	int hiIdx;
	Tomo_Data *t;

};
