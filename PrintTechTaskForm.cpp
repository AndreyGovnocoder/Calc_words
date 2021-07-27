#include "PrintTechTaskForm.h"

PrintTechTaskForm::PrintTechTaskForm(const QString& pathToImage, QWidget* parent)
	: QWidget(parent)
	, _pathToImage(pathToImage)
{
	setupUi(this);
	manager_lineEdit->setVisible(false);
}

void PrintTechTaskForm::setData(const Profile* profile, bool backDrop, bool otherClient)
{
	dateValue_lbl->setText(QDate::currentDate().toString("dd.MM.yyyy"));
	backDrop ? backDrop_lbl->setText("Да") : backDrop_lbl->setText("Нет");
	otherClient ? clientKind_lbl->setText("Конечный заказчик") : clientKind_lbl->setText("Рекламная компания");
	if (!profile)
		return;
	color_lineEdit->setText(profile->get_color());
	width_lineEdit->setText(QString::number(profile->get_width()) + " мм");
}

void PrintTechTaskForm::setReadOnly(bool value)
{
	client_lineEdit->setReadOnly(value);
	count_lineEdit->setReadOnly(value);
	acrilyc_lineEdit->setReadOnly(value);
	width_lineEdit->setReadOnly(value);
	color_lineEdit->setReadOnly(value);
	pvc_lineEdit->setReadOnly(value);
	manager_lineEdit->setReadOnly(value);
}

void PrintTechTaskForm::prepareLineEdits(bool toPrepare)
{
	setReadOnly(toPrepare);
	manager_cBox->setVisible(!toPrepare);
	manager_lineEdit->setVisible(toPrepare);
	manager_lineEdit->setText(manager_cBox->lineEdit()->text());
	client_lineEdit->setFrame(!toPrepare);
	count_lineEdit->setFrame(!toPrepare);
	acrilyc_lineEdit->setFrame(!toPrepare);
	width_lineEdit->setFrame(!toPrepare);
	color_lineEdit->setFrame(!toPrepare);
	pvc_lineEdit->setFrame(!toPrepare);
	manager_lineEdit->setFrame(!toPrepare);
	if (toPrepare)
	{
		client_lineEdit->setStyleSheet(_lineEditBckgrnd);
		count_lineEdit->setStyleSheet(_lineEditBckgrnd);
		acrilyc_lineEdit->setStyleSheet(_lineEditBckgrnd);
		width_lineEdit->setStyleSheet(_lineEditBckgrnd);
		color_lineEdit->setStyleSheet(_lineEditBckgrnd);
		pvc_lineEdit->setStyleSheet(_lineEditBckgrnd);
		manager_lineEdit->setStyleSheet(_lineEditBckgrnd);
	}
	else
	{
		client_lineEdit->setStyleSheet("");
		count_lineEdit->setStyleSheet("");
		acrilyc_lineEdit->setStyleSheet("");
		width_lineEdit->setStyleSheet("");
		color_lineEdit->setStyleSheet("");
		pvc_lineEdit->setStyleSheet("");
		manager_lineEdit->setStyleSheet("");
	}
}

void PrintTechTaskForm::toPrintSlot()
{
	QSize mWidgetSize;
	mWidgetSize.setWidth(mainWidget->width());
	mWidgetSize.setHeight(mainWidget->height());

	QSize sizeA4;
	sizeA4.setWidth(775);
	sizeA4.setHeight(1123);

	QPixmap pixmap(sizeA4);

	hide();
	prepareLineEdits(true);
	toPrint_btn->setVisible(false);
	mainWidget->setFixedSize(sizeA4);

	mainWidget->render(&pixmap);
	pixmap.toImage().save("techTask.png", "PNG");

	QPrinter printer(QPrinter::ScreenResolution);
	printer.setPaperSize(QPrinter::A4);
	printer.setFromTo(0, 1);
	printer.setOrientation(QPrinter::Portrait);

	QPrintDialog dlg(&printer, this);

	if (dlg.exec() == QDialog::Accepted)
	{
		QImage img("techTask.png");
		QPainter painter(&printer);
		painter.drawImage(QPoint(0, 0), img);
		painter.end();
		deleteLater();
	}
	else
	{
		mainWidget->setFixedSize(mWidgetSize);
		setFixedSize(mWidgetSize);
		show();
		toPrint_btn->setVisible(true);
		prepareLineEdits(false);
		manager_lineEdit->setFocus();
	}
}
