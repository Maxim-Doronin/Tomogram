#include "tomo.h"
#include "tomo_data.h"

tomo::tomo(int _lay, char* file, QWidget *parent)
	: QWidget(parent)
{
	lay = 0;
	this->_lay = _lay;
	
	t = new Tomo_Data(file);

	img = new QImage(t->pixels(lay), t->data_size.x, t->data_size.y, QImage::Format_Indexed8);
	lbl = new QLabel(this);
	lbl->setPixmap(QPixmap::fromImage(*img));
	
	this->resize(t->data_size.x, t->data_size.y);
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

void tomo::dumpEvent(QWheelEvent *we)
{
	lay+=(we->delta()/ (120/_lay));
	t->pixels_delete();
	delete [] img;
	img = new QImage(t->pixels(lay), t->data_size.x, t->data_size.y, QImage::Format_Indexed8);
//	img->loadFromData(t->pixels(lay), QImage::Format_Indexed8);
	
	lbl->setPixmap(QPixmap::fromImage(*img));
	this->update();
}