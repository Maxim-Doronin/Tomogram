#include "hystogram.h"

Hystogram::Hystogram(char *file, QWidget *parent)
	: QWidget(parent)
{
	t = new Tomo_Data(file);
	customPlot = new QCustomPlot(this);
	
	vbox = new QVBoxLayout(this);
	vbox->addWidget(customPlot);
	
	customPlot->xAxis->setRange(0.0, 4000.0);
	customPlot->yAxis->setRange(0.0, 3000.0);

	this->resize(1200, 300);
}

void Hystogram::get_data()
{
	hysto_data = new int [4000];
	for (int i = 0; i < 4000; i++)
		hysto_data[i] = 0;
	for (int i = 0; i < t->data_size.x * t->data_size.y; i++)
	{
		if (t->data[lay * t->data_size.x * t->data_size.y + i] < 4000)
			hysto_data[t->data[lay * t->data_size.x * t->data_size.y + i]]++;
	}
}

void Hystogram::del_data()
{
	delete [] hysto_data;
}

void Hystogram::get_hysto(int _lay)
{
	this->lay = _lay;
	get_data();
	
	fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(fossil);
	
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

}