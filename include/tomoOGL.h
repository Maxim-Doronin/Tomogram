#pragma once

#include <QGLWidget>
#include "tomo_data.h"

class TomoOGL : public QGLWidget
{
	Q_OBJECT

public:
	TomoOGL(char* file, QWidget *parent = 0);
	~TomoOGL(){};
	void upd(int _lay);

private:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	int lay;		//������������ ����
	int flag;
	Tomo_Data *t;

};
