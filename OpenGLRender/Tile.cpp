#include "Tile.h"
#include "Vertices.h"
#include "Global.h"

tile::tile( int x,  int y,  int z, int id) : x(x), y(y), z(z), id(id)
{
	if (itemAtlas.checkIfAnimation(id)) {
		//ob = itemAtlas.getAnimationObject(x, y, id, vh->getVAO(), vh->getTextureID());
	}
	else {
		auto temp = itemAtlas.getItemObject(id);
		if(temp)
			ob = new DrawObject(x, y, temp->getID(), temp->getLib()->getVAO(), temp->getVertices()->getTextPos());
	}
		//ob = new DrawObject(x, y, id, vh->getVAO(), vh->getTextureID());
}

tile::tile(tile * t)
{
	*this = *t;
	if (t->getObject()) {
		if (itemAtlas.checkIfAnimation(id)) {
			//ob = itemAtlas.getAnimationObject(x, y, id, vh->getVAO(), vh->getTextureID());
		}
		else
			ob = new DrawObject(t->ob->getXPosition(), t->ob->getYPosition(), id, t->ob->getVAO(), t->ob->getTexturePos());
		ob->setAnimationState(t->getObject()->getAnimationState());
	}
	if (t->getAllItems().size() > 0) {
		items.clear();
		for (auto& i : t->getAllItems()) {
			if (Weapon* check = dynamic_cast<Weapon*>(i)) {
				items.push_back(new Weapon(check));
			}
			else if (Armor* check = dynamic_cast<Armor*>(i)) {
				items.push_back(new Armor(check));
			}
			else if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(i)) {
				items.push_back(new NonStaticItem(check));
			}
			else
				items.push_back(new Item(i));
		}
	}
}

void tile::setID(int value)
{
	id = value;
}

void tile::setUID(int value)
{
	uid = value;
}

int& tile::getUID()
{
	return uid;
}

int& tile::getX()
{
	return x;
}

int& tile::getY()
{
	return y;
}

int& tile::getZ()
{
	return z;
}

int& tile::getID()
{
	return id;
}

int& tile::getZone()
{
	return zone;
}

void tile::setZone(int value)
{
	zone = value;
}

int& tile::getSpeed()
{
	return speed;
}

void tile::setSpeed(int value)
{
	speed = value;
}

string& tile::getArticle()
{
	return article;
}

void tile::setArticle(string value)
{
	article = value;
}

void tile::setDescription(string value)
{
	description = value;
}

string& tile::getDescription()
{
	return description;
}

void tile::setName(string value)
{
	name = value;
}

const string & tile::getName()
{
	return name;
}

bool tile::getBlockPathfind()
{
	return blockPathfind;
}

void tile::setBlockPathfind(bool value)
{
	blockPathfind = value;
}

void tile::insertItem(Item* insert)
{
	insert->updateObjectPosition(x, y);
	items.push_back(insert);
}

Item* tile::getTopItem()
{
	if (items.size() > 0)
		return items.back();
	else 
		return nullptr;
}


std::vector<Item*>& tile::getAllItems()
{
	return items;
}

void tile::setItems(std::vector<Item*>& otherItems)
{
	items = otherItems;
	for (auto& i : items) {
		auto obj = i->getObject();
		if (obj) {
			obj->setXPosition(x);
			obj->setYPosition(y);
		}
	}
}

void tile::clearItems()
{
	for (auto item : items) {
		if(item) delete item;
	}
	items.clear();
	items.shrink_to_fit();
}

void tile::destroyItemAt(int pos) {
	delete items.at(pos);
	items.erase(items.begin()+pos);
}

void tile::setObject(DrawObject* o)
{
	delete ob;
	ob = new DrawObject(o);
}
