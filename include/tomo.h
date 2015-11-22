#ifndef TOMO_H
#define TOMO_H

#include <QtGui/QWidget>
#include <qlabel.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <qpushbutton.h>
#include <qstring.h>
#include "tomo_data.h"


class tomo : public QWidget
{
	Q_OBJECT

public:
	tomo(int _lay, char*, QWidget *parent = 0);
	~tomo();
	void dumpEvent(QWheelEvent *we = 0);
	virtual void wheelEvent(QWheelEvent *we);
private:
	int lay;			//отображаемый слой
	int _lay;			//количество слоев в прокрутке
	int lowIdx;
	int hiIdx;
	Tomo_Data *t;
	QImage *img;
	QLabel *lbl;
	QHBoxLayout *layout;
	QLineEdit *lineLow;
	QLineEdit *lineHi;
	QPushButton *go;

private slots:
	void goClicked();
	void lineLowChange(QString str);
	void lineHiChange(QString str);
};

#endif // TOMO_H
