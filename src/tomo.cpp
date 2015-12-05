#include "tomo.h"

tomo::tomo(int _lay, char* file, QWidget *parent)
	: QWidget(parent)
{
	this->_lay = _lay;
	
	tomoData = new Tomo_Data(file);
	tGL		 = new TomoOGL(tomoData);
	hysto    = new Hystogram(tomoData);
	hysto->get_hysto();

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

	mainBox = new QVBoxLayout;
	mainBox->addLayout(layout);
	mainBox->addWidget(hysto);
	this->setLayout(mainBox);
	
	connect(lineLow, SIGNAL(textChanged(QString)), this, SLOT(lineLowChange(QString)));
	connect(lineHi, SIGNAL(textChanged(QString)), this, SLOT(lineHiChange(QString)));
	connect(go, SIGNAL(clicked()), this, SLOT(goClicked()));
}


tomo::~tomo()
{
	delete tomoData;
}

void tomo::wheelEvent(QWheelEvent *we)
{
	dumpEvent(we);
}

void tomo::lineLowChange(QString str)
{
	int _lowIdx = str.toInt();
	if (_lowIdx <= tomoData->hiIdx){ 
		go->setEnabled(true);
		tomoData->lowIdx = _lowIdx;}
	else
		go->setEnabled(false);
}

void tomo::lineHiChange(QString str)
{
	int _hiIdx = str.toInt();
	if (tomoData->lowIdx <= _hiIdx){
		go->setEnabled(true);
		tomoData->hiIdx = _hiIdx; }
	else
		go->setEnabled(false);
}

void tomo::goClicked()
{
	dumpEvent();
}

void tomo::dumpEvent(QWheelEvent *we)
{
	if(we != 0)	{
		tomoData->lay+=(we->delta()/ (120/_lay));
		hysto->get_hysto();
	}
	
	tGL->upd();
	
	//tPM->upd(lay, lowIdx, hiIdx);
	
	this->update();
}