#include "hystogram.h"

Hystogram::Hystogram(QWidget *parent)
	: QWidget(parent)
{
	src			= 0;
	densityData = 0;
	lay			= 0;
	lowIdx		= minR;
	hiIdx		= 4000;
	fossil		= 0;
	customPlot  = new QCustomPlot(this);
	
	customPlot->xAxis->setRange(minR, maxR);
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
	densityData = new float [maxR - minR];

	for (int i = 0; i < maxR - minR; i++)
		densityData[i] = 0;
	int count = 0;
	for (int i = 0; i < width * height; i++)	{
		count++;
		if ((src[lay * width * height + i] > minR)&&(src[lay * width * height + i] < maxR))
			densityData[src[lay * width * height + i] - minR]++;
		
	}
	for (int i = minR; i < maxR; i++)
		densityData[i - minR] /= count;
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
	for (int i = minR; i < maxR; i++)
		ticks << i;

	QVector<double> fossilData;
	for (int i = minR; i < lowIdx; i++)
		fossilData << densityData[i - minR];
	fossilData << 4000;
	for (int i = lowIdx + 1; i < hiIdx; i++)
		fossilData << densityData[i - minR];
	fossilData << 4000;
	for (int i = hiIdx + 1; i < maxR; i++)
		fossilData << densityData[i - minR];

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