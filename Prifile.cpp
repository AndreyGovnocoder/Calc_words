#include "Profile.h"

Profile::Profile(int id, const QString& name, const float& width, const QString& color, const float& price, bool favorite)
	: _id(id)
	, _name(name)
	, _width(width)
	, _color(color)
	, _price(price)
	, _favorite(favorite)
{
}