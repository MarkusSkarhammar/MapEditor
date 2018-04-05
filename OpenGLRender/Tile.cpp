#include "Tile.h"

tile::tile(size_t x, size_t y, size_t z, size_t id) : x(x), y(y), z(z), id(id)
{
}

void tile::setID(size_t value)
{
	id = value;
}

size_t tile::getX()
{
	return x;
}

size_t tile::getY()
{
	return y;
}

size_t tile::getZ()
{
	return z;
}

size_t tile::getID()
{
	return id;
}

size_t tile::getZone()
{
	return zone;
}

void tile::setZone(size_t value)
{
	zone = value;
}

size_t tile::getSpeed()
{
	return speed;
}

void tile::setSpeed(size_t value)
{
	speed = value;
}

string tile::getArticle()
{
	return article;
}

void tile::setArticle(string value)
{
	article = value;
}

string tile::getJustDescription()
{
	return description;
}

void tile::setDescription(string value)
{
	description = value;
}

const string & tile::getDescription()
{
	return description;
}

bool tile::getBlockPathfind()
{
	return blockPathfind;
}

void tile::setBlockPathfind(bool value)
{
	blockPathfind = value;
}

void tile::insertItem(Item insert)
{
	items.push_back(insert);
}

Item & tile::getTopItem()
{
	return items.back();
}

std::vector<Item>& tile::getAllItems()
{
	return items;
}

void tile::setItems(std::vector<Item>& otherItems)
{
	items = otherItems;
}

void tile::clearItems()
{
	items.clear();
	items.shrink_to_fit();
}
