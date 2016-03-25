#include "tomo.h"

tomo::tomo(int _lay, char* file, QWidget *parent)
	: QWidget(parent)
{
	this->_lay = _lay;
	src = 0;
	
	tomoData = new TomoData(file);
	tomoData->getData2D();
	tGL		 = new TomoOGL(tomoData->data2D, tomoData->dataSize.x, tomoData->dataSize.y);
	hysto    = new Hystogram();
	hysto->setLay(0);
	hysto->setLowIdx(1900);
	hysto->setHiIdx(2250);
	hysto->get_hysto(tomoData->data3D, tomoData->dataSize.x, tomoData->dataSize.y);
	stats    = new Stats();
	stats->setData(tomoData->data3D, tomoData->dataSize.x, tomoData->dataSize.y, tomoData->dataSize.z);
	stats->setLay(0);
	rc = new RayCasting(tomoData);
	rayCastingBox = new QCheckBox("Ray Casting");
	phi = 0;
	psi = 0;

	posPressed    = new QLabel(this);
	posReleased   = new QLabel(this);
	averDensity   = new QLabel(this);
	expValue	  = new QLabel(this);
	dispValue     = new QLabel(this);
	meanSquareDev = new QLabel(this);

	lineLow = new QLineEdit;
	lineLow->setText("1900");
	lineHi  = new QLineEdit;
	lineHi->setText("2250");
	go		= new QPushButton("go!");
	go->setDefault(true);
	go->setEnabled(false);

	gaussCheckBox = new QCheckBox("Gauss Blur");
	gaussLineEdit = new QLineEdit("1.4");
	sigma = 1.4;
	gaussLineEdit->setFixedWidth(50);
	
	sobelCheckBox = new QCheckBox("Sobel Operator");
	
	nonMaxSuppBox = new QCheckBox("Non-maximum suppression");
	nonMaxSuppBox->setDisabled(true);
	
	dbTresholdBox = new QCheckBox("Double thresholding");
	dbTresholdBox->setDisabled(true);
	dbTresholdLR  = new QLineEdit("0.55");
	leftThreshold = 0.55;
	dbTresholdRR  = new QLineEdit("0.6");
	rightThreshold = 0.6;
	dbTresholdLR->setFixedWidth(50);
	dbTresholdRR->setFixedWidth(50);
	
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
	statistic->addWidget(gaussLineEdit);
	statistic->addWidget(sobelCheckBox);
	statistic->addWidget(nonMaxSuppBox);
	statistic->addWidget(dbTresholdBox);
	statistic->addWidget(dbTresholdLR);
	statistic->addWidget(dbTresholdRR);
	statistic->addWidget(tracingEdgBox);

	statistic->addWidget(posPressed);
	statistic->addWidget(posReleased);
	statistic->addWidget(averDensity);
	statistic->addWidget(expValue);
	statistic->addWidget(dispValue);
	statistic->addWidget(meanSquareDev);

	statistic->addWidget(rayCastingBox);

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
	connect(gaussLineEdit, SIGNAL(textChanged(QString)), this, SLOT(gaussLineChanged(QString)));
	connect(sobelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(sobelCheckChanged(int)));
	connect(nonMaxSuppBox, SIGNAL(stateChanged(int)), this, SLOT(nonMaxSuppChanged(int)));
	connect(dbTresholdBox, SIGNAL(stateChanged(int)), this, SLOT(dbTresholdChanged(int)));
	connect(dbTresholdLR, SIGNAL(textChanged(QString)), this, SLOT(dbTresholdLRChanged(QString)));
	connect(dbTresholdRR, SIGNAL(textChanged(QString)), this, SLOT(dbTresholdRRChanged(QString)));
	connect(tracingEdgBox, SIGNAL(stateChanged(int)), this, SLOT(tracingEdgChanged(int)));
	connect(rayCastingBox, SIGNAL(stateChanged(int)), this, SLOT(rayCastingChanged(int)));
}


tomo::~tomo()
{
	if((src)&&(src != tomoData->data2D))
		delete []src;
	delete tomoData;
}

void tomo::wheelEvent(QWheelEvent *we)
{
	dumpEvent(we);
}

void tomo::keyPressEvent(QKeyEvent *pe)
{
	switch (pe->key()) {
	case Qt::Key_D : { phi += 10; break; }
	case Qt::Key_A : { phi -= 10; break; }
	case Qt::Key_W : { psi += 10; break; }
	case Qt::Key_S : { psi -= 10; break; }
	
	default: return;
	}
	dumpEvent();
}

void tomo::lineLowChange(QString str)
{
	int _lowIdx = str.toInt();
	if (_lowIdx <= tomoData->getHiIdx()){ 
		go->setEnabled(true);
		tomoData->setLowIdx(_lowIdx);
		sliderLeft->setValue(_lowIdx);}
	else
		go->setEnabled(false);
}

void tomo::lineHiChange(QString str)
{
	int _hiIdx = str.toInt();
	if (tomoData->getLowIdx() <= _hiIdx){
		go->setEnabled(true);
		tomoData->setHiIdx(_hiIdx);
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
	if (_lowIdx < tomoData->getHiIdx()){
		tomoData->setLowIdx(_lowIdx);
		hysto->setLowIdx(_lowIdx);
	}
	else {
		sliderLeft->setValue(tomoData->getHiIdx() - 1);
		tomoData->setLowIdx(tomoData->getHiIdx() - 1);
		hysto->setLowIdx(tomoData->getLowIdx());
	}
	QString str;
	lineLow->setText(str.setNum(tomoData->getLowIdx()));
	dumpEvent();
}

void tomo::setRangeRight(int _hiIdx)
{
	if (_hiIdx > tomoData->getLowIdx()){
		tomoData->setHiIdx(_hiIdx);
		hysto->setHiIdx(tomoData->getHiIdx());
	}
	else {
		sliderRight->setValue(tomoData->getLowIdx() + 1);
		tomoData->setHiIdx(tomoData->getLowIdx() + 1);
		hysto->setHiIdx(tomoData->getHiIdx());
	}
	QString str;
	lineHi->setText(str.setNum(tomoData->getHiIdx()));
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
	
	stats->setArea(x, y, pointPressed.x(), pointPressed.y());
	stats->setLay(tomoData->lay);

	averVal.setNum(stats->averageDensity());
	averVal = "Average density: " + averVal;
	
	expVal.setNum(stats->expectedValue());
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

void tomo::gaussLineChanged(QString str)
{
	float _sigma = str.toFloat();
	if ((_sigma < 0) || (_sigma > 50))
	{
		gaussLineEdit->setText("1.4");
		_sigma = 1.4;
	}
	sigma = _sigma;
	dumpEvent();
}

void tomo::sobelCheckChanged(int flag)
{
	if (flag) {
		nonMaxSuppBox->setDisabled(false);
		nonMaxSuppChanged(nonMaxSuppBox->isChecked());
	}
	if (!flag) {
		nonMaxSuppBox->setDisabled(true);
		dbTresholdBox->setDisabled(true);
		tracingEdgBox->setDisabled(true);
	}
	dumpEvent();
}

void tomo::nonMaxSuppChanged(int flag)
{
	if (flag){
		dbTresholdBox->setDisabled(false);
		dbTresholdChanged(dbTresholdBox->isChecked());
	}
	if (!flag){
		dbTresholdBox->setDisabled(true);
		tracingEdgBox->setDisabled(true);
	}
	dumpEvent();
}

void tomo::dbTresholdChanged(int flag)
{
	if (flag){
		tracingEdgBox->setDisabled(false);
		tracingEdgChanged(tracingEdgBox->isChecked());
	}
	if (!flag)
		tracingEdgBox->setDisabled(true);
	dumpEvent();
}

void tomo::dbTresholdLRChanged(QString str)
{
	float _leftThreshold = str.toFloat();
	if (_leftThreshold < 0)
	{
		dbTresholdLR->setText("0");
		_leftThreshold = 0;
	}
	if (_leftThreshold > rightThreshold)
		_leftThreshold = rightThreshold;
	leftThreshold = _leftThreshold;
	dumpEvent();
}

void tomo::dbTresholdRRChanged(QString str)
{
	float _rightThreshold = str.toFloat();
	if (_rightThreshold > 1)
	{
		dbTresholdRR->setText("1.0");
		_rightThreshold = 1;
	}
	if (_rightThreshold < leftThreshold)
		_rightThreshold = leftThreshold;
	rightThreshold = _rightThreshold;
	dumpEvent();
}

void tomo::tracingEdgChanged(int)
{
	dumpEvent();
}

void tomo::rayCastingChanged(int flag)
{
	dumpEvent();
}

void tomo::dumpEvent(QWheelEvent *we)
{	
	int w = tomoData->dataSize.x;
	int h = tomoData->dataSize.y;

	if(we != 0)	
		tomoData->setLay(tomoData->getLay() + (we->delta()/ (120/_lay)));

	hysto->setLay(tomoData->getLay());
	hysto->get_hysto(tomoData->data3D, w, h);

	if ((src) && (src != tomoData->data2D)) 
		delete []src;
	src = tomoData->getData2D();

	float *ga = 0;

	if (gaussCheckBox->isChecked())
		GaussBlur::blur(src, src, sigma, w, h);
	if (sobelCheckBox->isChecked())
		CannyOperator::sobel(src, src, ga, w, h);
	if ((nonMaxSuppBox->isChecked())&&(nonMaxSuppBox->isEnabled()))
		CannyOperator::nonMaxSuppression(src, ga, src, w, h);
	if ((dbTresholdBox->isChecked())&&(dbTresholdBox->isEnabled()))
		CannyOperator::doubleTresholding(src, src, leftThreshold, rightThreshold, w, h);
	if ((tracingEdgBox->isChecked())&&(tracingEdgBox->isEnabled()))
		CannyOperator::tracingEdges(src, src, w, h);
	if (rayCastingBox->isChecked())
	{
		rc->render(phi, psi);
		tGL->upd(tomoData->dataColor2D, w, h);
	}
	else
		tGL->upd(src, w, h);
	
	this->update();

	if (ga) delete []ga;
}