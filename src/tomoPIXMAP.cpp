#include "tomoPIXMAP.h"

TomoPIXMAP::TomoPIXMAP(char* file, QWidget *parent)
	: QWidget(parent)
{
	t = new Tomo_Data(file);
	lay = 0;

	img = new QImage(t->get_lay(lay), t->data_size.x, t->data_size.y, QImage::Format_Indexed8);
	lbl = new QLabel(this);
	lbl->setPixmap(QPixmap::fromImage(*img));
	layout = new QHBoxLayout;
	layout->addWidget(lbl);
	this->setLayout(layout);
}

TomoPIXMAP::~TomoPIXMAP()
{
	delete layout;
	delete lbl;
	delete img;
	delete t;
}

void TomoPIXMAP::upd(int _lay, int lowIdx, int hiIdx)
{
	t->pixels_delete();
	delete [] img;
	
	lay = _lay;
	img = new QImage(t->get_lay(lay, lowIdx, hiIdx), t->data_size.x, t->data_size.y, QImage::Format_Indexed8);
	lbl->setPixmap(QPixmap::fromImage(*img));

	this->update();
}