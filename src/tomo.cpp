#include "tomo.h"

tomo::tomo(int _lay, char* file, QWidget *parent)
	: QWidget(parent)
{
	this->_lay = _lay;
	
	tomoData = new Tomo_Data(file);
	tomoData->get_data_lay();
	tGL		 = new TomoOGL(tomoData);
	hysto    = new Hystogram(tomoData);
	hysto->get_hysto();
	stats    = new Stats(tomoData);
	gB		 = new GaussBlur(tomoData, 1);

	pos = new QLabel(this);

	lineLow = new QLineEdit;
	lineHi  = new QLineEdit;
	go		= new QPushButton("go!");
	go->setDefault(true);
	go->setEnabled(false);

	gaussCheckBox = new QCheckBox("Gauss Blur");

	sliderLeft = new QSlider(Qt::Horizontal);
	sliderLeft->setRange(1800, 2500);
	sliderLeft->setValue(1800);
	
	sliderRight = new QSlider(Qt::Horizontal);
	sliderRight->setRange(1800, 2500);
	sliderRight->setValue(2500);

	layout = new QHBoxLayout;
	layout->addWidget(lineLow);
	layout->addWidget(lineHi);
	layout->addWidget(go);

	statistic = new QVBoxLayout;
	statistic->addLayout(layout);
	statistic->addWidget(gaussCheckBox);
	statistic->addWidget(pos);

	image = new QHBoxLayout;
	image->addLayout(statistic);
	image->addWidget(tGL);

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
	mainBox->addLayout(image);
	mainBox->addWidget(hysto);
	mainBox->addLayout(margin);
	this->setLayout(mainBox);
	
	connect(lineLow, SIGNAL(textChanged(QString)), this, SLOT(lineLowChange(QString)));
	connect(lineHi, SIGNAL(textChanged(QString)), this, SLOT(lineHiChange(QString)));
	connect(go, SIGNAL(clicked()), this, SLOT(goClicked()));
	connect(sliderLeft, SIGNAL(valueChanged(int)), this, SLOT(setRangeLeft(int)));
	connect(sliderRight, SIGNAL(valueChanged(int)), this, SLOT(setRangeRight(int)));
	connect(tGL, SIGNAL(mousePressed(int, int)), this, SLOT(setMousePosition(int, int)));
	connect(gaussCheckBox, SIGNAL(stateChanged(int)), this, SLOT(gaussCheckChanged(int)));
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

void tomo::setMousePosition(int x, int y)
{
	QString str;
	QString xStr;
	QString yStr;
	QString statStr;
	xStr.setNum(x);
	yStr.setNum(y);
	statStr.setNum(stats->averageDensity(x,y));
	str = xStr + ' ' + yStr + ' ' + statStr;
	pos->setText(str);
}

void tomo::gaussCheckChanged(int flag) 
{
	dumpEvent();
}

void tomo::dumpEvent(QWheelEvent *we)
{
	if(we != 0)	tomoData->lay += (we->delta()/ (120/_lay));
	tomoData->get_data_lay();
	if (gaussCheckBox->isChecked())
		gB->blur();
	hysto->get_hysto();
	tGL->upd();
	
	this->update();
}