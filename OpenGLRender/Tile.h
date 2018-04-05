#ifndef TILE_H
#define TILE_H
#include <string>
#include <vector>
#include "Item.h"

using std::string;

class tile {
public:
	tile(size_t x, size_t y, size_t z, size_t id);
	void setID(size_t value);
	size_t getX();
	size_t getY();
	size_t getZ();
	size_t getID();
	size_t getZone();
	void setZone(size_t zone);
	size_t getSpeed();
	void setSpeed(size_t speed);
	string getArticle();
	void setArticle(string value);
	string getJustDescription();
	void setDescription(string value);
	const string& getDescription();
	bool getBlockPathfind();
	void setBlockPathfind(bool value);
	void insertItem(Item insert);
	Item& getTopItem();
	std::vector<Item>& getAllItems();
	void setItems(std::vector<Item>& otherItems);
	void clearItems();
private:
	size_t x, y, z, id, zone{ 0 }, speed{ 100 };
	string description, article;
	bool blockPathfind{ false };
	std::vector<Item> items;
};

#endif