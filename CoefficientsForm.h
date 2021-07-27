#pragma once

#include <QWidget>
#include "ui_CoefficientsForm.h"
#include <QtGui/qvalidator.h>
#include <QtWidgets/qmessagebox.h>
#include "Coefficients.h"
#include "Database.h"
#include "MainForm.h"

class CoefficientsForm : public QWidget, public Ui::CoefficientsForm
{
	Q_OBJECT

public:
	CoefficientsForm(QWidget *parent = Q_NULLPTR);
	~CoefficientsForm() = default;

private:
	void setLineEdits();
	bool checkLineEdits();

private slots:
	void saveBtnSlot();
};
