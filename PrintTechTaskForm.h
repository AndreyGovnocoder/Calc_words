#pragma once

#include <QWidget>
#include "ui_PrintTechTaskForm.h"
#include "Profile.h"
#include <QtPrintSupport/qprinter.h>
#include <QPrintDialog>
#include <QtGui>

//a4 = 2484x3511 ï

class PrintTechTaskForm : public QWidget, public Ui::PrintTechTaskForm
{
	Q_OBJECT

public:
	PrintTechTaskForm(const QString& pathToImage, QWidget* parent = Q_NULLPTR);
	~PrintTechTaskForm() = default;
	void setData(const Profile* profile, bool backDrop, bool otherClient);

private:
	QString _pathToImage;
	void setReadOnly(bool value);
	void prepareLineEdits(bool toPrepare);
	const QString _lineEditBckgrnd = "background-color: rgba(255, 255, 255, 0);\nborder-color: rgba(255, 255, 255, 0);";

private slots:
	void toPrintSlot();
};
