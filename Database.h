#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <qstring.h>
#include <QtWidgets/qmessagebox.h>
#include <QSqlDatabase>
#include <QtSql>
#include <vector>
#include <qfile.h>
#include "Profile.h"
#include "Coefficients.h"
#include "Prices.h"
#include "MainForm.h"

class Database
{
public:
	Database() = default;
	~Database() = default;

	//static const QString DB_NAME;
	//static QString DB_NAME;
	static const QString TABLE_COEFFS;
	static const QString TABLE_OPTIONS;
	static const QString TABLE_PRICES;
	static const QString TABLE_PROFILES;

	static QSqlDatabase getDatabase();
	static bool initDatabase(const QString& DB_NAME);
	static int getLastId(const QString& table);
	static std::vector<Profile> getProfilesList();
	static bool addProfile(const Profile& profile);
	static bool editProfile(const Profile& profile);
	static bool setProfileFavorite(const int profileId, const bool favorite);
	static Coefficients getCoefficients();
	static bool setCoefficients(const Coefficients& coefficients);
	static Prices getPrices();
	static bool setPrices(const Prices& prices);
	static QString getPassword();
	static QString getPassToReset();
	static bool changePassword(const QString& newPassword);

	static bool removeObject(const QString& table, const int id);
};

#endif  DATABASE_H