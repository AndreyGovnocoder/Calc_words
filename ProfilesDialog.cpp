#include "ProfilesDialog.h"

ProfilesDialog::ProfilesDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	nameLineEdit->setFocus();
	widthLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
	priceLineEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
}
