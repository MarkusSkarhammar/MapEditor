#include "Item.h"

size_t Item::getID()
{
	return id;
}

size_t Item::getUID()
{
	return uID;
}

void Item::setUID(size_t id)
{
	uID = id;
}

bool Item::getBlockProjectile()
{
	return blockProjectile;
}

void Item::setBlockProjectile(bool value)
{
	blockProjectile = value;
}

bool Item::getBlockPathfind()
{
	return blockPathfind;
}

void Item::setBlockPathfind(bool value)
{
	blockPathfind = value;
}

bool Item::getBlockObject()
{
	return blockObject;
}

void Item::setBlockObject(bool value)
{
	blockObject = value;
}

bool Item::getMoveable()
{
	return moveable;
}

void Item::setMoveable(bool value)
{
	moveable = value;
}

bool Item::getPickupable()
{
	return pickupable;
}

void Item::setPickupable(bool value)
{
	pickupable = value;
}

bool Item::getUseable()
{
	return useable;
}

void Item::setUseable(bool value)
{
	useable = value;
}

bool Item::getHangeable()
{
	return hangeable;
}

void Item::setHangeable(bool value)
{
	hangeable = value;
}

bool Item::getAlwaysOnTop()
{
	return alwaysOnTop;
}

void Item::setAlwaysOnTop(bool value)
{
	alwaysOnTop = value;
}

string Item::getDescription()
{
	return description;
}

string Item::getArticle()
{
	return article;
}

string Item::getType()
{
	return type;
}

void Item::setType(string value)
{
	type = value;
}
