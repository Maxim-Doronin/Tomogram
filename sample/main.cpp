#include "tomo.h"
#include <QtGui/QApplication>
#include <tomo_data.h>
#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//Tomo_Data t("Resources/head.bin");

	//QImage img(t.pixels(), t.data_size.x, t.data_size.y, QImage::Format_Indexed8);
	//QLabel lbl;
	//lbl.setPixmap(QPixmap::fromImage(img));
	//lbl.show();
	
	tomo w;
	w.show();

	return a.exec();
}
