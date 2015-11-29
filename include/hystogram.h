#pragma once

#include <QtGui/QWidget>
#include "qcustomplot.h"
#include "tomo_data.h"
#include <QVBoxLayout>

class Hystogram : public QWidget
{
	Q_OBJECT

public:
	Hystogram(char *file, QWidget *parent = 0);
	~Hystogram(){};
private:
	Tomo_Data *t;

	QCustomPlot *customPlot;
	QCPBars *fossil;
	QVBoxLayout *vbox;

	int lay;
	int *hysto_data;
	void get_data();
	void del_data();
public:
	void get_hysto(int _lay);
};