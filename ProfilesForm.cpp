#include "ProfilesForm.h"

ProfilesForm::ProfilesForm(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	setProfilesTable();
	connect(profilesTableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequestedSlot(QPoint)));
}

void ProfilesForm::setProfilesTable()
{
	profilesTableWidget->clearContents();
	profilesTableWidget->setRowCount(0);

	for (auto& profile : MainForm::_profilesList)
		setItemToTable(profile, profilesTableWidget->rowCount());

	profilesTableWidget->hideColumn(0);
	profilesTableWidget->resizeColumnsToContents();
	profilesTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	profilesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	profilesTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void ProfilesForm::setItemToTable(const Profile& profile, const int row)
{
	QTableWidgetItem* profileIdItem = new QTableWidgetItem(QString::number(profile.get_id()));
	profileIdItem->setData(Qt::UserRole, profile.get_id());

	QCheckBox* favoriteCB = new QCheckBox();
	favoriteCB->setProperty("row", row);
	if (profile.isFavorite())
		favoriteCB->setCheckState(Qt::Checked);

	connect(favoriteCB, &QCheckBox::stateChanged, this, &ProfilesForm::setFavoriteSlot);

	profilesTableWidget->insertRow(row);
	profilesTableWidget->setItem(row, 0, profileIdItem);
	profilesTableWidget->setItem(row, 1, new QTableWidgetItem(profile.get_name()));
	profilesTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(profile.get_width())));
	profilesTableWidget->setItem(row, 3, new QTableWidgetItem(profile.get_color()));
	profilesTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(profile.get_price())));
	profilesTableWidget->setCellWidget(row, 5, favoriteCB);
	
}

void ProfilesForm::customMenuRequestedSlot(QPoint pos)
{
	QMenu* menu = new QMenu(this);
	QAction* addProfileAction = new QAction("Добавить", this);
	QAction* editProfileAction = new QAction("Редактировать", this);
	QAction* removeProfileAction = new QAction("Удалить", this);

	connect(addProfileAction, SIGNAL(triggered()), this, SLOT(addProfileSlot()));
	connect(editProfileAction, SIGNAL(triggered()), this, SLOT(editProfileSlot()));
	connect(removeProfileAction, SIGNAL(triggered()), this, SLOT(removeProfileSlot()));

	menu->addAction(addProfileAction);
	menu->addAction(editProfileAction);
	menu->addAction(removeProfileAction);
	if (!MainForm::_admin)
	{
		editProfileAction->setEnabled(false);
		removeProfileAction->setEnabled(false);
	}
	menu->popup(profilesTableWidget->viewport()->mapToGlobal(pos));
}

void ProfilesForm::addProfileSlot()
{
	ProfilesDialog dialog(this);
	dialog.setWindowTitle("Добавить профиль");
	dialog.saveButton->setVisible(false);

	if (!dialog.exec())
		return;

	if (dialog.nameLineEdit->text().isEmpty())
		QMessageBox::warning(this, "Внимание", "Введите название профиля");
	else if (dialog.widthLineEdit->text().isEmpty())
		QMessageBox::warning(this, "Внимание", "Введите ширину профиля");
	else if (dialog.colorLineEdit->text().isEmpty())
		QMessageBox::warning(this, "Внимание", "Введите цвет профиля");
	else
	{
		Profile profile;
		profile.set_name(dialog.nameLineEdit->text());
		profile.set_width(dialog.widthLineEdit->text().replace(",", ".").toFloat());
		profile.set_color(dialog.colorLineEdit->text());
		profile.set_price(dialog.priceLineEdit->text().replace(",", ".").toFloat());
		profile.set_favorite(false);

		if (Database::addProfile(profile))
		{
			profile.set_id(Database::getLastId(Database::TABLE_PROFILES));
			MainForm::_profilesList.push_back(std::move(profile));
			setItemToTable(profile, profilesTableWidget->rowCount());

			int index = MainForm::_profilesCombobox->count();
			QVariant var;
			var.setValue(profile.get_id());
			MainForm::_profilesCombobox->addItem(profile.get_name());
			MainForm::_profilesCombobox->setItemData(index, var);
		}
	}
}

void ProfilesForm::editProfileSlot()
{
	const int currRow = profilesTableWidget->currentRow();

	if (currRow < 0)
		return;

	const int profileId = profilesTableWidget->item(currRow, 0)->data(Qt::UserRole).toInt();

	Profile* editableProfile = MainForm::findProfile(profileId);
	ProfilesDialog dialog(this);
	dialog.setWindowTitle("Редактирование профиля");
	dialog.addButton->setVisible(false);
	dialog.nameLineEdit->setText(editableProfile->get_name());
	dialog.widthLineEdit->setText(QString::number(editableProfile->get_width()));
	dialog.colorLineEdit->setText(editableProfile->get_color());
	dialog.priceLineEdit->setText(QString::number(editableProfile->get_price()));

	if (!dialog.exec())
		return;

	const Profile editedProfile(
		editableProfile->get_id(),
		dialog.nameLineEdit->text(),
		dialog.widthLineEdit->text().toFloat(),
		dialog.colorLineEdit->text(),
		dialog.priceLineEdit->text().toFloat(),
		editableProfile->isFavorite());

	if (Database::editProfile(editedProfile))
	{
		std::replace_if(
			MainForm::_profilesList.begin(),
			MainForm::_profilesList.end(),
			[editedProfile](const Profile& profile) { return editedProfile.get_id() == profile.get_id(); },
			editedProfile);

		setItemToTable(editedProfile, currRow);

		for (int i = 0; i < MainForm::_profilesCombobox->count(); ++i)
		{
			if (MainForm::_profilesCombobox->itemData(i).toInt() == editedProfile.get_id())
			{
				QVariant var;
				var.setValue(editedProfile.get_id());
				MainForm::_profilesCombobox->setItemText(i, editedProfile.get_name());
				MainForm::_profilesCombobox->setItemData(i, var);
			}
		}
	}
}

void ProfilesForm::removeProfileSlot()
{
	const int currRow = profilesTableWidget->currentRow();

	if (currRow < 0)
		return;

	const int profileId = profilesTableWidget->item(currRow, 0)->data(Qt::UserRole).toInt();
	QMessageBox messageBox(QMessageBox::Question,
		"Удаление профиля",
		"Удалить выделенный профиль?",
		QMessageBox::Yes | QMessageBox::No,
		this);
	messageBox.setButtonText(QMessageBox::Yes, "Да");
	messageBox.setButtonText(QMessageBox::No, "Нет");
	if (messageBox.exec() == QMessageBox::Yes)
	{
		if (Database::removeObject(Database::TABLE_PROFILES, profileId))
		{
			auto itProfile = std::remove_if(MainForm::_profilesList.begin(), MainForm::_profilesList.end(), [&profileId](const Profile& profile) {return profile.get_id() == profileId; });
			MainForm::_profilesList.erase(itProfile, MainForm::_profilesList.end());

			profilesTableWidget->removeRow(currRow);

			for (int i = 0; i < MainForm::_profilesCombobox->count(); ++i)
			{
				if (MainForm::_profilesCombobox->itemData(i).toInt() == profileId)
					MainForm::_profilesCombobox->removeItem(i);
			}
		}
	}
}

void ProfilesForm::setFavoriteSlot(const int state)
{
	QObject* comboBox = sender();
	if (!comboBox)
		return;

	const int row = comboBox->property("row").toInt();
	bool favorite;
	QTableWidgetItem* item = profilesTableWidget->item(row, 0);
	if (Profile* profile = MainForm::findProfile(item->data(Qt::UserRole).toInt()))
	{
		favorite = (state == Qt::Checked) ? true : false;

		if (Database::setProfileFavorite(profile->get_id(), favorite))
			profile->set_favorite(favorite);
	}
}
