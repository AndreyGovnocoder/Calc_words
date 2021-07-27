#pragma once

#include <QWidget>
#include "ui_ProfilesForm.h"
#include "MainForm.h"
#include "ProfilesDialog.h"

class ProfilesForm : public QWidget, public Ui::ProfilesForm
{
	Q_OBJECT

public:
	ProfilesForm(QWidget *parent = Q_NULLPTR);
	~ProfilesForm() = default;

private:
	void setProfilesTable();
	void setItemToTable(const Profile& profile, const int row);

private slots:
	void addProfileSlot();
	void editProfileSlot();
	void removeProfileSlot();
	void customMenuRequestedSlot(QPoint pos);
	void setFavoriteSlot(const int state);
};
