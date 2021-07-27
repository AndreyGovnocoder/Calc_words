#include "PricesForm.h"

PricesForm::PricesForm(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	setLineEdits();
}

void PricesForm::setLineEdits()
{
	acrilycLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	acrilycLineEdit->setText(QString::number(MainForm::_prices.get_acrylic()));
	millingLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	millingLineEdit->setText(QString::number(MainForm::_prices.get_frezer()));
	pvcLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	pvcLineEdit->setText(QString::number(MainForm::_prices.get_pvc()));

	if (MainForm::_admin)
	{
		acrilycLineEdit->setReadOnly(false);
		millingLineEdit->setReadOnly(false);
		pvcLineEdit->setReadOnly(false);
	}
}

bool PricesForm::checkEditLines()
{
	if (acrilycLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите стоимость за акрил");
		return false;
	}
	else if (millingLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите стоимость фрезеровки");
		return false;
	}
	else if (pvcLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите стоимость ПВХ");
		return false;
	}

	return true;
}

void PricesForm::saveBtnSlot()
{
	if (!checkEditLines())
		return;

	MainForm::_prices.set_acrylic(acrilycLineEdit->text().replace(",", ".").toFloat());
	MainForm::_prices.set_frezer(millingLineEdit->text().replace(",", ".").toFloat());
	MainForm::_prices.set_pvc(pvcLineEdit->text().replace(",", ".").toFloat());

	if (Database::setPrices(MainForm::_prices))
		this->close();
}
