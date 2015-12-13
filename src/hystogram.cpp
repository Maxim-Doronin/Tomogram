#include "hystogram.h"

Hystogram::Hystogram(Tomo_Data *&tD, QWidget *parent)
	: QWidget(parent)
{
	this->tD = tD;
	customPlot = new QCustomPlot(this);
	
	customPlot->xAxis->setRange(0.0, 3000.0);
	customPlot->yAxis->setRange(0.0, 4000.0);

	vbox = new QVBoxLayout(this);
	vbox->addWidget(customPlot);

	this->setMinimumHeight(300);
}

void Hystogram::get_hysto()
{
	if(tD->data_density){
		customPlot->clearPlottables();
	}

	tD->get_data_density();
	
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
	for (int i = 0; i < tD->lowIdx; i++)
		fossilData << tD->data_density[i];
	fossilData << 4000;
	for (int i = tD->lowIdx + 1; i < tD->hiIdx; i++)
		fossilData << tD->data_density[i];
	fossilData << 4000;
	for (int i = tD->hiIdx + 1; i < 4000; i++)
		fossilData << tD->data_density[i];

	fossil->setData(ticks, fossilData);
	customPlot->addPlottable(fossil);
	customPlot->replot();
}
