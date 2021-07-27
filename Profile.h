#pragma once
#include <QString.h>

class Profile
{
public:
	Profile(int id, const QString& name, const float& width, const QString& color, const float& price, bool favorite);
	Profile() = default;
	~Profile() = default;

	void set_id(const int id) { _id = id; }
	void set_name(const QString& name) { _name = name; }
	void set_width(const float& width) { _width = width; }
	void set_color(const QString& color) { _color = color; }
	void set_price(const float& price) { _price = price; }
	void set_favorite(bool favorite) { _favorite = favorite; }

	int get_id() const { return _id; }
	const QString& get_name() const { return _name; }
	const float& get_width() const { return _width; }
	const QString& get_color() const { return _color; }
	const float& get_price() const { return _price; }
	bool isFavorite() const { return _favorite; }

private:
	int _id;
	QString _name;
	float _width;
	QString _color;
	float _price;
	bool _favorite;
};