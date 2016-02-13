#pragma once

#include <QtGui/QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QString>
#include "TomoData.h"
#include "tomoOGL.h"
#include "hystogram.h"
#include "statistic.h"
#include "gaussBlur.h"
#include "cannyOperator.h"


class tomo : public QWidget
{
	Q_OBJECT

public:
	tomo(int _lay, char*, QWidget *parent = 0);
	~tomo();
protected:
	int _lay;			//���������� ����� � ���������
	float sigma;			//sigma-����. ��� ������� ������
	float leftThreshold;
	float rightThreshold;
	
	TomoData *tomoData;
	TomoOGL *tGL;
	Hystogram *hysto;
	Stats *stats;

	QHBoxLayout *layout;
	QVBoxLayout *statistic;
	QHBoxLayout *image;
	QVBoxLayout *sliders;
	QVBoxLayout *mainBox;
	QHBoxLayout *margin;
	QVBoxLayout *marginLeft;
	
	QLineEdit *lineLow;
	QLineEdit *lineHi;
	QPushButton *go;
	QCheckBox *gaussCheckBox;
	QLineEdit *gaussLineEdit;
	QCheckBox *sobelCheckBox;
	QCheckBox *nonMaxSuppBox;
	QCheckBox *dbTresholdBox;
	QLineEdit *dbTresholdLR;
	QLineEdit *dbTresholdRR;
	QCheckBox *tracingEdgBox;
	QLabel *posPressed;
	QPoint pointPressed;
	QLabel *posReleased;
	QLabel *averDensity;
	QLabel *expValue;
	QLabel *dispValue;
	QLabel *meanSquareDev;

	QSlider *sliderLeft;
	QSlider *sliderRight;

	void dumpEvent(QWheelEvent *we = 0);
	virtual void wheelEvent(QWheelEvent *we);

private slots:
	void goClicked();
	void lineLowChange(QString str);
	void lineHiChange(QString str);

	void setRangeLeft(int);
	void setRangeRight(int);
	void setMousePressPosition(int, int);
	void setMouseReleasePosition(int, int);

	void gaussCheckChanged(int);
	void gaussLineChanged(QString str);
	void sobelCheckChanged(int);
	void nonMaxSuppChanged(int);
	void dbTresholdChanged(int);
	void dbTresholdLRChanged(QString str);
	void dbTresholdRRChanged(QString str);
	void tracingEdgChanged(int);
};

