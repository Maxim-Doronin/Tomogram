#pragma once

#include <QtGui/QWidget>
#include "qcustomplot.h"
#include "TomoData.h"
#include <QVBoxLayout>
#include <QSlider>

class Hystogram : public QWidget
{
	Q_OBJECT

public:
	Hystogram(QWidget *parent = 0);
	~Hystogram();
private:
	short *src;
	float *densityData;

	QCustomPlot *customPlot;
	QCPBars *fossil;
	QVBoxLayout *vbox;

	void getDensityData(short *&src, int width, int height);

public:
	int lay;
	int lowIdx;
	int hiIdx;

	void setLay(int lay);
	void setLowIdx(int lowIdx);
	void setHiIdx(int hiIdx);
	void get_hysto(short *&src, int width, int height);
};