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
#include <QKeyEvent>
#include "TomoData.h"
#include "tomoOGL.h"
#include "hystogram.h"
#include "statistic.h"
#include "gaussBlur.h"
#include "cannyOperator.h"
#include "rayCasting.h"


class tomo : public QWidget
{
	Q_OBJECT

public:
	tomo(int _lay, char*, QWidget *parent = 0);
	~tomo();
protected:
	int _lay;				//количество слоев в прокрутке
	float sigma;			//sigma-коэф. для фильтра Гаусса
	float leftThreshold;
	float rightThreshold;
	
	TomoData *tomoData;
	TomoOGL *tGL;
	uchar *src;				//информация, отображаемая на экране
	Hystogram *hysto;
	Stats *stats;
	RayCasting *rc;
	float phi, psi;

	QHBoxLayout *layout;
	QHBoxLayout *tresholds;
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
	QSlider *dbTresholdLRSl;
	QSlider *dbTresholdRRSl;
	QLineEdit *dbTresholdRR;
	QPushButton *tresholdBut;

	QCheckBox *tracingEdgBox;
	QLabel *posPressed;
	QPoint pointPressed;
	QLabel *posReleased;
	QLabel *averDensity;
	QLabel *expValue;
	QLabel *dispValue;
	QLabel *meanSquareDev;
	QCheckBox *rayCastingBox;

	QSlider *sliderLeft;
	QSlider *sliderRight;
	QSlider *sliderMid;

	void dumpEvent(QWheelEvent *we = 0);
	virtual void wheelEvent(QWheelEvent *we);
	virtual void keyPressEvent(QKeyEvent *pe);

private slots:
	void goClicked();
	void lineLowChange(QString str);
	void lineHiChange(QString str);

	void setRangeLeft(int);
	void setRangeMid(int);
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
	void TresholdLeftChangeSl(int);
	void TresholdRightChangeSl(int);
	void tresholdClick();

	void tracingEdgChanged(int);
	void rayCastingChanged(int);
};

