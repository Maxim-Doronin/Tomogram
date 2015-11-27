#include "tomo.h"
#include "tomoOGL.h"
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

	//TomoOGL t("Resources/head.bin");
	//t.resize(513, 513);
	//t.show();

	return a.exec();
}
