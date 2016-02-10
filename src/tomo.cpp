#include "tomo.h"

tomo::tomo(int _lay, char* file, QWidget *parent)
	: QWidget(parent)
{
	this->_lay = _lay;
	
	tomoData = new TomoData(file);
	tomoData->getData2D();
	tGL		 = new TomoOGL(tomoData->data2D, tomoData->dataSize.x, tomoData->dataSize.y);
	hysto    = new Hystogram();
	hysto->setLay(0);
	hysto->setLowIdx(1900);
	hysto->setHiIdx(2250);
	hysto->get_hysto(tomoData->data3D, tomoData->dataSize.x, tomoData->dataSize.y);
	stats    = new Stats(tomoData);

	posPressed    = new QLabel(this);
	posReleased   = new QLabel(this);
	averDensity   = new QLabel(this);
	expValue	  = new QLabel(this);
	dispValue     = new QLabel(this);
	meanSquareDev = new QLabel(this);

	lineLow = new QLineEdit;
	lineHi  = new QLineEdit;
	go		= new QPushButton("go!");
	go->setDefault(true);
	go->setEnabled(false);

	gaussCheckBox = new QCheckBox("Gauss Blur");
	
	sobelCheckBox = new QCheckBox("Sobel Operator");
	
	nonMaxSuppBox = new QCheckBox("Non-maximum suppression");
	nonMaxSuppBox->setDisabled(true);
	
	dbTresholdBox = new QCheckBox("Double thresholding");
	dbTresholdBox->setDisabled(true);
	
	tracingEdgBox = new QCheckBox("Tracing edges");
	tracingEdgBox->setDisabled(true);

	sliderLeft = new QSlider(Qt::Horizontal);
	sliderLeft->setRange(1800, 2500);
	sliderLeft->setValue(1900);
	
	sliderRight = new QSlider(Qt::Horizontal);
	sliderRight->setRange(1800, 2500);
	sliderRight->setValue(2250);

	layout = new QHBoxLayout;
	layout->addWidget(lineLow);
	layout->addWidget(lineHi);
	layout->addWidget(go);

	statistic = new QVBoxLayout;
	statistic->addLayout(layout);
	statistic->addWidget(gaussCheckBox);
	statistic->addWidget(sobelCheckBox);
	statistic->addWidget(nonMaxSuppBox);
	statistic->addWidget(dbTresholdBox);
	statistic->addWidget(tracingEdgBox);

	statistic->addWidget(posPressed);
	statistic->addWidget(posReleased);
	statistic->addWidget(averDensity);
	statistic->addWidget(expValue);
	statistic->addWidget(dispValue);
	statistic->addWidget(meanSquareDev);

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
	connect(tGL, SIGNAL(mousePressed(int, int)), this, SLOT(setMousePressPosition(int, int)));
	connect(tGL, SIGNAL(mouseReleased(int, int)), this, SLOT(setMouseReleasePosition(int, int)));
	connect(gaussCheckBox, SIGNAL(stateChanged(int)), this, SLOT(gaussCheckChanged(int)));
	connect(sobelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(sobelCheckChanged(int)));
	connect(nonMaxSuppBox, SIGNAL(stateChanged(int)), this, SLOT(nonMaxSuppChanged(int)));
	connect(dbTresholdBox, SIGNAL(stateChanged(int)), this, SLOT(dbTresholdChanged(int)));
	connect(tracingEdgBox, SIGNAL(stateChanged(int)), this, SLOT(tracingEdgChanged(int)));
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
	if (_lowIdx < tomoData->hiIdx){
		tomoData->lowIdx = _lowIdx;
		hysto->setLowIdx(tomoData->lowIdx);
	}
	else {
		sliderLeft->setValue(tomoData->hiIdx - 1);
		tomoData->lowIdx = tomoData->hiIdx - 1;
		hysto->setLowIdx(tomoData->lowIdx);
	}
	QString str;
	lineLow->setText(str.setNum(tomoData->lowIdx));
	dumpEvent();
}

void tomo::setRangeRight(int _hiIdx)
{
	if (_hiIdx > tomoData->lowIdx){
		tomoData->hiIdx = _hiIdx;
		hysto->setHiIdx(tomoData->hiIdx);
	}
	else {
		sliderRight->setValue(tomoData->lowIdx + 1);
		tomoData->hiIdx = tomoData->lowIdx + 1;
		hysto->setHiIdx(tomoData->hiIdx);
	}
	QString str;
	lineHi->setText(str.setNum(tomoData->hiIdx));
	dumpEvent();
}

void tomo::setMousePressPosition(int x, int y)
{
	pointPressed.setX(x);
	pointPressed.setY(y);
	QString str;
	QString xStr;
	QString yStr;
	xStr.setNum(x);
	yStr.setNum(y);
	str = xStr + ' ' + yStr;
	posPressed->setText(str);
}

void tomo::setMouseReleasePosition(int x, int y)
{
	QString str;
	QString xStr;
	QString yStr;
	QString averVal;
	QString expVal;
	QString dispVal;
	QString meanSquareVal;
	
	xStr.setNum(x);
	yStr.setNum(y);
	str = xStr + ' ' + yStr;
	
	averVal.setNum(stats->averageDensity(x, y, pointPressed.x(), pointPressed.y()));
	averVal = "Average density: " + averVal;
	
	expVal.setNum(stats->expectedValue(x, y, pointPressed.x(), pointPressed.y()));
	expVal  = "Expected value: " + expVal;
	
	dispVal.setNum(stats->dispersion());
	dispVal = "Dispersion: " + dispVal;

	meanSquareVal.setNum(stats->meanSquareDeviation());
	meanSquareVal = "Mean square deviation: " + meanSquareVal;
	
	posReleased->setText(str);
	averDensity->setText(averVal);
	expValue->setText(expVal);
	dispValue->setText(dispVal);
	meanSquareDev->setText(meanSquareVal);
}

void tomo::gaussCheckChanged(int flag) 
{
	dumpEvent();
}

void tomo::sobelCheckChanged(int flag)
{
	if (flag)
		nonMaxSuppBox->setDisabled(false);
	if (!flag) {
		nonMaxSuppBox->setDisabled(true);
		dbTresholdBox->setDisabled(true);
		tracingEdgBox->setDisabled(true);
	}
	dumpEvent();
}

void tomo::nonMaxSuppChanged(int flag)
{
	if (flag)
		dbTresholdBox->setDisabled(false);
	if (!flag){
		dbTresholdBox->setDisabled(true);
		tracingEdgBox->setDisabled(true);
	}
	dumpEvent();
}

void tomo::dbTresholdChanged(int flag)
{
	if (flag)
		tracingEdgBox->setDisabled(false);
	if (!flag)
		tracingEdgBox->setDisabled(true);
	dumpEvent();
}

void tomo::tracingEdgChanged(int)
{
	dumpEvent();
}

void tomo::dumpEvent(QWheelEvent *we)
{	
	int w = tomoData->dataSize.x;
	int h = tomoData->dataSize.y;

	if(we != 0)	tomoData->lay += (we->delta()/ (120/_lay));
	tomoData->getData2D();
	hysto->setLay(tomoData->lay);
	hysto->get_hysto(tomoData->data3D, w, h);

	uchar *src = tomoData->data2D;

	float *ga = 0;

	if (gaussCheckBox->isChecked())
		GaussBlur::blur(src, src, 1.4, w, h);
	if (sobelCheckBox->isChecked())
		CannyOperator::sobel(src, src, ga, w, h);
	if ((nonMaxSuppBox->isChecked())&&(nonMaxSuppBox->isEnabled()))
		CannyOperator::nonMaxSuppression(src, ga, src, w, h);
	if ((dbTresholdBox->isChecked())&&(dbTresholdBox->isEnabled()))
		CannyOperator::doubleTresholding(src, src, 0.55, 0.6, w, h);
	if ((tracingEdgBox->isChecked())&&(tracingEdgBox->isEnabled()))
		CannyOperator::tracingEdges(src, src, w, h);

	tGL->upd(src, w, h);
	
	this->update();

	if (!ga) delete []ga;
}