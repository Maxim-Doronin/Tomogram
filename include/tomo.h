#ifndef TOMO_H
#define TOMO_H

#include <QtGui/QWidget>
#include <qlabel.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include "tomo_data.h"


class tomo : public QWidget
{
	Q_OBJECT

public:
	tomo(QWidget *parent = 0);
	~tomo();
	void dumpEvent(QWheelEvent *we);
	virtual void wheelEvent(QWheelEvent *we);
private:
	int lay;
	Tomo_Data *t;
	QImage *img;
	QLabel *lbl;
};

#endif // TOMO_H
