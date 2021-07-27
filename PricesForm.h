#pragma once

#include <QWidget>
#include "ui_PricesForm.h"
#include <QtGui/qvalidator.h>
#include <QtWidgets/qmessagebox.h>
#include "Prices.h"
#include "Database.h"
#include "MainForm.h"

class PricesForm : public QWidget, public Ui::PricesForm
{
	Q_OBJECT

public:
	PricesForm(QWidget *parent = Q_NULLPTR);
	~PricesForm() = default;

private:
	void setLineEdits();
	bool checkEditLines();

private slots:
	void saveBtnSlot();
	
};
