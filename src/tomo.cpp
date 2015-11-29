#include "tomo.h"

tomo::tomo(int _lay, char* file, QWidget *parent)
	: QWidget(parent)
{
	lay = 0;
	this->_lay = _lay;
	
	lowIdx = 0;
	hiIdx = 6000;
	tGL = new TomoOGL(file);
	//tPM = new TomoPIXMAP(file);


	lineLow = new QLineEdit;
	lineHi  = new QLineEdit;
	go		= new QPushButton("go!");
	go->setDefault(true);
	go->setEnabled(false);

	layout = new QHBoxLayout;
	layout->addWidget(lineLow);
	layout->addWidget(lineHi);
	layout->addWidget(go);
	layout->addWidget(tGL);
	//layout->addWidget(tPM);
	this->setLayout(layout);
	
	connect(lineLow, SIGNAL(textChanged(QString)), this, SLOT(lineLowChange(QString)));
	connect(lineHi, SIGNAL(textChanged(QString)), this, SLOT(lineHiChange(QString)));
	connect(go, SIGNAL(clicked()), this, SLOT(goClicked()));
}


tomo::~tomo()
{
	delete tPM;
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
	
	tGL->upd(lay, lowIdx, hiIdx);
	//tPM->upd(lay, lowIdx, hiIdx);
	
	this->update();
}