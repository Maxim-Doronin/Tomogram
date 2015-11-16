#ifndef TOMO_H
#define TOMO_H

#include <QtGui/QWidget>
#include <qlabel.h>
#include <QMouseEvent>
#include "tomo_data.h"


class tomo : public QLabel
{
	Q_OBJECT

public:
	tomo(QLabel *parent = 0);
	tomo(Tomo_Data *t);
	~tomo();
	virtual void mousePressEvent(QMouseEvent *pe);
	void dumpEvent(QMouseEvent *pe, const QString& strMessage);
private:
	Tomo_Data *tmd;
};

#endif // TOMO_H
