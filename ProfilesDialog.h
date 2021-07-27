#pragma once

#include <QDialog>
#include "ui_ProfilesDialog.h"
#include <QtGui/qvalidator.h>

class ProfilesDialog : public QDialog, public Ui::ProfilesDialog
{
	Q_OBJECT

public:
	ProfilesDialog(QWidget *parent = Q_NULLPTR);
	~ProfilesDialog() = default;
};
