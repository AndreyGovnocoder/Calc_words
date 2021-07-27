#pragma once

#include <QDialog>
#include "ui_ChangePassDialog.h"

class ChangePassDialog : public QDialog, public Ui::ChangePassDialog
{
	Q_OBJECT

public:
	ChangePassDialog(QWidget *parent = Q_NULLPTR);
	~ChangePassDialog() = default;
};
