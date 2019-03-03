#include "Item.h"
#include "VertecesHandler.h"
#include "Global.h"

Item::Item(int id, string article, string name) : id(id), article(article), name(name)
{
	VertecesHandler vh;
	getVertecesHandlerFromID(vh, id);
	if (itemAtlas.checkIfAnimation(id)) {
		ob = itemAtlas.getAnimationObject(-1, -1, id, vh.getVAO(), vh.getTextureID());
	}
	else
		ob = new Object(-1, -1, id, vh.getVAO(), vh.getTextureID());
}

Item::Item(Item * i)
{
	*this = *i;
	VertecesHandler vh;
	getVertecesHandlerFromID(vh, id);
	if (itemAtlas.checkIfAnimation(id)) {
		ob = itemAtlas.getAnimationObject(i->ob->getXPosition(), i->ob->getYPosition(), id, vh.getVAO(), vh.getTextureID());
		if (AnimationObject* check1 = dynamic_cast<AnimationObject*>(i->getObject())) {
			if (AnimationObject* check2 = dynamic_cast<AnimationObject*>(ob))
				*check2 = *check1;
		}
	}
	else
		ob = new Object(i->ob->getXPosition(), i->ob->getYPosition(), id, vh.getVAO(), vh.getTextureID());
	ob->setAnimationState(i->getAnimationState());
}

int& Item::getID()
{
	return id;
}

int& Item::getUID()
{
	return uID;
}

void Item::setUID(int id)
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

string& Item::getDescription()
{
	return description;
}

string& Item::getArticle()
{
	return article;
}

string& Item::getType()
{
	return type;
}

void Item::setType(string value)
{
	type = value;
}

void Item::setDescription(string value) {
	description = value;
}

string& Item::getName() {
	return name;
}

void getItemType(Item*& output, int id) {
	auto item = itemAtlas.getItem(id);
	if (Weapon* check = dynamic_cast<Weapon*>(item)) {
		output = new Weapon(check);
	}
	else if (Armor* check = dynamic_cast<Armor*>(item)) {
		output = new Armor(check);
	}
	else if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(item)) {
		output = new NonStaticItem(check);
	}
	else
		output = new Item(item);
};
