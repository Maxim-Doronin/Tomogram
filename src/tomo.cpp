#include "tomo.h"
#include "tomo_data.h"

tomo::tomo(int _lay, char* file, QWidget *parent)
	: QWidget(parent)
{
	lay = 0;
	this->_lay = _lay;
	
	
	t = new Tomo_Data(file);
	lowIdx = t->minTint;
	hiIdx = t->maxTint;

	img = new QImage(t->get_lay(lay), t->data_size.x, t->data_size.y, QImage::Format_Indexed8);
	lbl = new QLabel(this);
	lbl->setPixmap(QPixmap::fromImage(*img));

	lineLow = new QLineEdit;
	lineHi  = new QLineEdit;
	go		= new QPushButton("go!");
	go->setDefault(true);
	go->setEnabled(false);

	layout = new QHBoxLayout;
	layout->addWidget(lineLow);
	layout->addWidget(lineHi);
	layout->addWidget(go);
	layout->addWidget(lbl);
	this->setLayout(layout);
	this->resize(t->data_size.x, t->data_size.y);

	connect(lineLow, SIGNAL(textChanged(QString)), this, SLOT(lineLowChange(QString)));
	connect(lineHi, SIGNAL(textChanged(QString)), this, SLOT(lineHiChange(QString)));
	connect(go, SIGNAL(clicked()), this, SLOT(goClicked()));
}


tomo::~tomo()
{
	delete t;
	delete img;
	delete lbl;
}

void tomo::wheelEvent(QWheelEvent *we)
{
	dumpEvent(we);
}

void tomo::lineLowChange(QString str)
{
	int _lowIdx = str.toInt();
	if (_lowIdx <= hiIdx){ 
		go->setEnabled(true);
		lowIdx = _lowIdx;}
	else
		go->setEnabled(false);
}

void tomo::lineHiChange(QString str)
{
	int _hiIdx = str.toInt();
	if (lowIdx <= _hiIdx){
		go->setEnabled(true);
		hiIdx = _hiIdx; }
	else
		go->setEnabled(false);
}

void tomo::goClicked()
{
	dumpEvent();
}

void tomo::dumpEvent(QWheelEvent *we)
{
	if(we != 0)	lay+=(we->delta()/ (120/_lay));
	
	t->pixels_delete();
	delete [] img;
	img = new QImage(t->get_lay(lay, lowIdx, hiIdx), t->data_size.x, t->data_size.y, QImage::Format_Indexed8);
	
	lbl->setPixmap(QPixmap::fromImage(*img));
	layout->addWidget(lbl);
	this->setLayout(layout);
	this->update();
}