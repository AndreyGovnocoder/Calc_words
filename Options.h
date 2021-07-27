#pragma once
#include <qstring.h>

class Options
{
public:
	Options() = default;
	~Options() = default;

	void set_id(int id) { _id = id; }
	void set_password(const QString& password) { _password = password; }
	void set_passwordToReset(const QString& passToReset) { _passwordToReset = passToReset; }

	int get_id() { return _id; }
	const QString& get_password() const { return _password; }
	const QString& get_passwordToReset() const { return _passwordToReset; }

private:
	int _id;
	QString _password;
	QString _passwordToReset;
};