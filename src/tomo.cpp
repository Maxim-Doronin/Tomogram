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

	sliderLeft = new QSlider(Qt::Horizontal);
	sliderLeft->setRange(0, 3000);
	sliderLeft->setValue(0);
	
	sliderRight = new QSlider(Qt::Horizontal);
	sliderRight->setRange(0, 3000);
	sliderRight->setValue(3000);
	
	layout = new QHBoxLayout;
	layout->addWidget(lineLow);
	layout->addWidget(lineHi);
	layout->addWidget(go);
	layout->addWidget(tGL);

	sliders = new QVBoxLayout;
	sliders->addWidget(sliderLeft);
	sliders->addWidget(sliderRight);
	sliders->setMargin(20);
	marginLeft = new QVBoxLayout;
	marginLeft->setMargin(15);
	margin = new QHBoxLayout;
	margin->addLayout(marginLeft);
	margin->addLayout(sliders);

	mainBox = new QVBoxLayout;
	mainBox->addLayout(layout);
	mainBox->addWidget(hysto);
	mainBox->addLayout(margin);
	this->setLayout(mainBox);
	
	connect(lineLow, SIGNAL(textChanged(QString)), this, SLOT(lineLowChange(QString)));
	connect(lineHi, SIGNAL(textChanged(QString)), this, SLOT(lineHiChange(QString)));
	connect(go, SIGNAL(clicked()), this, SLOT(goClicked()));
	connect(sliderLeft, SIGNAL(valueChanged(int)), this, SLOT(setRangeLeft(int)));
	connect(sliderRight, SIGNAL(valueChanged(int)), this, SLOT(setRangeRight(int)));
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
		tomoData->lowIdx = _lowIdx;
		sliderLeft->setValue(_lowIdx);}
	else
		go->setEnabled(false);
}

void tomo::lineHiChange(QString str)
{
	int _hiIdx = str.toInt();
	if (tomoData->lowIdx <= _hiIdx){
		go->setEnabled(true);
		tomoData->hiIdx = _hiIdx;
		sliderRight->setValue(_hiIdx);}
	else
		go->setEnabled(false);
}

void tomo::goClicked()
{
	dumpEvent();
}

void tomo::setRangeLeft(int _lowIdx)
{
	if (_lowIdx < tomoData->hiIdx)
		tomoData->lowIdx = _lowIdx;
	else {
		sliderLeft->setValue(tomoData->hiIdx - 1);
		tomoData->lowIdx = tomoData->hiIdx - 1;
	}
	QString str;
	lineLow->setText(str.setNum(tomoData->lowIdx));
	dumpEvent();
}

void tomo::setRangeRight(int _hiIdx)
{
	if (_hiIdx > tomoData->lowIdx)
		tomoData->hiIdx = _hiIdx;
	else {
		sliderRight->setValue(tomoData->lowIdx + 1);
		tomoData->hiIdx = tomoData->lowIdx + 1;
	}
	QString str;
	lineHi->setText(str.setNum(tomoData->hiIdx));
	dumpEvent();
}

void tomo::dumpEvent(QWheelEvent *we)
{
	if(we != 0)	tomoData->lay+=(we->delta()/ (120/_lay));
	
	hysto->get_hysto();
	tGL->upd();
	
	//tPM->upd(lay, lowIdx, hiIdx);
	
	this->update();
}