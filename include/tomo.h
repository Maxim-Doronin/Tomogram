#pragma once

#include <QtGui/QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include "tomo_data.h"
#include "tomoOGL.h"
#include "hystogram.h"
#include "statistic.h"


class tomo : public QWidget
{
	Q_OBJECT

public:
	tomo(int _lay, char*, QWidget *parent = 0);
	~tomo();
protected:
	int _lay;			//количество слоев в прокрутке

	Tomo_Data *tomoData;
	TomoOGL *tGL;
	Hystogram *hysto;
	Stats *stats;

	QHBoxLayout *layout;
	QVBoxLayout *statistic;
	QHBoxLayout *image;
	QVBoxLayout *sliders;
	QVBoxLayout *mainBox;
	QHBoxLayout *margin;
	QVBoxLayout *marginLeft;
	
	QLineEdit *lineLow;
	QLineEdit *lineHi;
	QPushButton *go;
	QLabel *pos;

	QSlider *sliderLeft;
	QSlider *sliderRight;

	void dumpEvent(QWheelEvent *we = 0);
	virtual void wheelEvent(QWheelEvent *we);

private slots:
	void goClicked();
	void lineLowChange(QString str);
	void lineHiChange(QString str);

	void setRangeLeft(int);
	void setRangeRight(int);
	void setMousePosition(int, int);
};

