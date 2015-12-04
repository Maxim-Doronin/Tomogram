#include "hystogram.h"

Hystogram::Hystogram(char *file, QWidget *parent)
	: QWidget(parent)
{
	hysto_data = 0;
	t = new Tomo_Data(file);
	customPlot = new QCustomPlot(this);
	
	vbox = new QVBoxLayout(this);
	vbox->addWidget(customPlot);
	
	customPlot->xAxis->setRange(0.0, 3000.0);
	customPlot->yAxis->setRange(0.0, 4000.0);

	this->setMinimumHeight(400);
}

void Hystogram::get_data()
{
	hysto_data = new int [4000];
	for (int i = 0; i < 4000; i++)
		hysto_data[i] = 0;
	for (int j = 0; j < t->data_size.x * t->data_size.y; j++)
	{
		if (t->data[lay * t->data_size.x * t->data_size.y + j] < 4000)
			hysto_data[t->data[lay * t->data_size.x * t->data_size.y + j]]++;
	}
}

void Hystogram::del_data()
{
	delete [] hysto_data;
}

void Hystogram::get_hysto(int _lay)
{
	this->lay = _lay;
	if (_lay < 0) lay = 0;
	if (_lay > t->data_size.z - 1) lay = t->data_size.z - 1;
		
	if(hysto_data){
		del_data();
		customPlot->clearPlottables();
	}

	get_data();
	fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    
	
	QPen pen;
	pen.setWidthF(1.0);
	pen.setColor(QColor(0, 0, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(0, 0, 0));

	QVector<double> ticks;
	for (int i = 0; i < 4000; i++)
		ticks << i;

	QVector<double> fossilData;
	for (int i = 0; i < 4000; i++)
		fossilData << hysto_data[i];

	fossil->setData(ticks, fossilData);
	customPlot->addPlottable(fossil);
	customPlot->replot();
}