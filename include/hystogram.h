#pragma once

#include <QtGui/QWidget>
#include "qcustomplot.h"
#include "tomo_data.h"
#include <QVBoxLayout>

class Hystogram : public QWidget
{
	Q_OBJECT

public:
	Hystogram(Tomo_Data*&, QWidget *parent = 0);
	~Hystogram(){};
private:
	Tomo_Data *tD;

	QCustomPlot *customPlot;
	QCPBars *fossil;
	QVBoxLayout *vbox;

public:
	void get_hysto();
};