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
#include "tomoPIXMAP.h"
#include "tomoOGL.h"


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

	TomoOGL *tGL;
	TomoPIXMAP *tPM;
	QHBoxLayout *layout;
	QLineEdit *lineLow;
	QLineEdit *lineHi;
	QPushButton *go;

private slots:
	void goClicked();
	void lineLowChange(QString str);
	void lineHiChange(QString str);
};

