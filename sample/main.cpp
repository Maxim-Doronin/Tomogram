#include "tomo.h"
#include "tomoOGL.h"
#include "hystogram.h"
#include <QtGui/QApplication>
#include <TomoData.h>
#include <QLabel>
#define GLEW_STATIC

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//������ �������� -- ���������� ����� � ���������
	//������ -- ��� �����
	
	tomo w(6, "../../../resources/head.bin");
	w.show();

	return a.exec();
}
