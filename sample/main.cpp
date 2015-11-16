#include "tomo.h"
#include <QtGui/QApplication>
#include <tomo_data.h>
#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//первый аргумент -- количество слоев в прокрутке
	//второй -- имя файла
	
	tomo w(6, "Resources/head.bin");
	w.show();

	return a.exec();
}
