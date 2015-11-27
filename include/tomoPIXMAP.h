#pragma once

#include <QtGui/QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "tomo_data.h"

class TomoPIXMAP : public QWidget
{
	Q_OBJECT

public: 
	TomoPIXMAP(char* file, QWidget *parent = 0);
	~TomoPIXMAP();
	void upd(int _lay, int lowIdx = 0, int hiIdx = 6000);

private:
	int lay; 
	
	Tomo_Data *t;
	QImage *img;
	QLabel *lbl;
	QHBoxLayout *layout;
};