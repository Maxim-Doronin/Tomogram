#include "tomo.h"
#include "tomo_data.h"

tomo::tomo(QLabel *parent)
	: QLabel(parent)
{

}

tomo::tomo(Tomo_Data *t)
{
	tmd = t;
}

tomo::~tomo()
{

}

void tomo::mousePressEvent(QMouseEvent* pe)
{
	dumpEvent(pe, "Mouse Pressed");
}

void tomo::dumpEvent(QMouseEvent *pe, const QString& strMsg)
{
	setText(QString::number(tmd->data_size.x)+" "+QString::number(tmd->data_size.y)+" "+QString::number(pe->x()));
}