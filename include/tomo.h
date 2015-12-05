#pragma once

#include <QtGui/QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include "tomo_data.h"
#include "tomoOGL.h"
#include "hystogram.h"


class tomo : public QWidget
{
	Q_OBJECT

public:
	tomo(int _lay, char*, QWidget *parent = 0);
	~tomo();
protected:
	int _lay;			//���������� ����� � ���������

	Tomo_Data *tomoData;
	TomoOGL *tGL;
	Hystogram *hysto;

	QHBoxLayout *layout;
	QVBoxLayout *mainBox;
	QLineEdit *lineLow;
	QLineEdit *lineHi;
	QPushButton *go;

	void dumpEvent(QWheelEvent *we = 0);
	virtual void wheelEvent(QWheelEvent *we);

private slots:
	void goClicked();
	void lineLowChange(QString str);
	void lineHiChange(QString str);
};

