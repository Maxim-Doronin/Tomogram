#include "tomo.h"
#include "tomoOGL.h"
#include "hystogram.h"
#include <QtGui/QApplication>
#include <tomo_data.h>
#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//������ �������� -- ���������� ����� � ���������
	//������ -- ��� �����
	
	tomo w(6, "Resources/head.bin");
	w.show();

	//Hystogram h("Resources/head.bin");
	//h.get_hysto(5);
	//h.show();

	return a.exec();
}
