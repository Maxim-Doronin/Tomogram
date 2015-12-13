#pragma once

#include <QGLWidget>
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

	Tomo_Data *tD;
};
