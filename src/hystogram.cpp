#include "hystogram.h"

Hystogram::Hystogram(QWidget *parent)
	: QWidget(parent)
{
	src			= 0;
	densityData = 0;
	lay			= 0;
	lowIdx		= 1800;
	hiIdx		= 2500;
	fossil		= 0;
	customPlot  = new QCustomPlot(this);
	
	customPlot->xAxis->setRange(1800.0, 2500.0);
	customPlot->yAxis->setRange(0.0, 0.013);

	vbox = new QVBoxLayout(this);
	vbox->addWidget(customPlot);

	this->setMinimumHeight(250);
}

Hystogram::~Hystogram()
{
	delete [] densityData;
	delete customPlot;
	delete vbox;
}

void Hystogram::getDensityData(short *&src, int width, int height)
{
	if (densityData) delete []densityData;
	densityData = new float [1200];

	for (int i = 0; i < 700; i++)
		densityData[i] = 0;
	int count = 0;
	for (int i = 0; i < width * height; i++)	{
		count++;
		if ((src[lay * width * height + i] > 1800)&&(src[lay * width * height + i] < 3000))
			densityData[src[lay * width * height + i] - 1800]++;
		
	}
	for (int i = 1800; i < 3000; i++)
		densityData[i - 1800] /= count;
}

void Hystogram::get_hysto(short *&src, int width, int height)
{
	if(!src) return;
	if(densityData)
		customPlot->clearPlottables();

	getDensityData(src, width, height);
	
	fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    	
	QPen pen;
	pen.setWidthF(1.0);
	pen.setColor(QColor(0, 0, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(0, 0, 0));
	
	QVector<double> ticks;
	for (int i = 1800; i < 3000; i++)
		ticks << i;

	QVector<double> fossilData;
	for (int i = 1800; i < lowIdx; i++)
		fossilData << densityData[i - 1800];
	fossilData << 4000;
	for (int i = lowIdx + 1; i < hiIdx; i++)
		fossilData << densityData[i - 1800];
	fossilData << 4000;
	for (int i = hiIdx + 1; i < 3000; i++)
		fossilData << densityData[i - 1800];

	fossil->setData(ticks, fossilData);
	customPlot->addPlottable(fossil);
	customPlot->replot();
}

void Hystogram::setLay(int lay)
{
	this->lay = lay;
}

void Hystogram::setLowIdx(int lowIdx)
{
	this->lowIdx = lowIdx;
}

void Hystogram::setHiIdx(int hiIdx)
{
	this->hiIdx = hiIdx;
}