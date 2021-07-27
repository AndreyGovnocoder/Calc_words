#include "CoefficientsForm.h"

CoefficientsForm::CoefficientsForm(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	setLineEdits();
}

void CoefficientsForm::setLineEdits()
{
	netProfitLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	netProfitLineEdit->setText(QString::number(MainForm::_coefficients.get_netProfit()));
	laborCostsLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	laborCostsLineEdit->setText(QString::number(MainForm::_coefficients.get_laborCosts()));
	firstRangeLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	firstRangeLineEdit->setText(QString::number(MainForm::_coefficients.get_height_5_11()));
	secondRangeLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	secondRangeLineEdit->setText(QString::number(MainForm::_coefficients.get_height_12_19()));
	thirdRangeLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	thirdRangeLineEdit->setText(QString::number(MainForm::_coefficients.get_height_20_29()));
	fourthRangeLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	fourthRangeLineEdit->setText(QString::number(MainForm::_coefficients.get_height_30_49()));
	fifthRangeLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	fifthRangeLineEdit->setText(QString::number(MainForm::_coefficients.get_height_more50()));
	otherClientLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	otherClientLineEdit->setText(QString::number(MainForm::_coefficients.get_otherClient()));
	advertisingLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	advertisingLineEdit->setText(QString::number(MainForm::_coefficients.get_advertAgency()));

	if (MainForm::_admin)
	{
		netProfitLineEdit->setReadOnly(false);
		laborCostsLineEdit->setReadOnly(false);
		firstRangeLineEdit->setReadOnly(false);
		secondRangeLineEdit->setReadOnly(false);
		thirdRangeLineEdit->setReadOnly(false);
		fourthRangeLineEdit->setReadOnly(false);
		fifthRangeLineEdit->setReadOnly(false);
		otherClientLineEdit->setReadOnly(false);
		advertisingLineEdit->setReadOnly(false);
	}
}

bool CoefficientsForm::checkLineEdits()
{
	if (netProfitLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент чистой прибыли");
		return false;
	} 
	else if (laborCostsLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент трудозатрат");
		return false;
	}
	else if (firstRangeLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент 5-11 см");
		return false;
	}
	else if (secondRangeLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент 12-19 см");
		return false;
	}
	else if (thirdRangeLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент 20-29 см");
		return false;
	}
	else if (fourthRangeLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент 30-49 см");
		return false;
	}
	else if (fifthRangeLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент >50 см");
		return false;
	}
	else if (otherClientLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент для конечного заказчика");
		return false;
	}
	else if (advertisingLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Внимание!", "Укажите коэффициент для рекламного агентства");
		return false;
	}

	return true;
}

void CoefficientsForm::saveBtnSlot()
{
	if (!checkLineEdits())
		return;

	MainForm::_coefficients.set_netProfit(netProfitLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_laborCosts(laborCostsLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_height_5_11(firstRangeLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_height_12_19(secondRangeLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_height_20_29(thirdRangeLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_height_30_49(fourthRangeLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_height_more50(fifthRangeLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_advertAgency(advertisingLineEdit->text().replace(",", ".").toFloat());
	MainForm::_coefficients.set_otherClient(otherClientLineEdit->text().replace(",", ".").toFloat());

	if (Database::setCoefficients(MainForm::_coefficients))
		this->close();
}
