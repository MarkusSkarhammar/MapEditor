#ifndef TILE_H
#define TILE_H
#include <string>
#include <vector>
#include "Item.h"

using std::string;

class tile {
public:
	tile(int x, int y, int z, int id);
	tile(tile*& t);
	~tile() { clearItems(); };
	void setID(int value);
	void setUID(int value);
	void setX(int value) { x = value; };
	void setY(int value) { y = value; };
	void setZ(int value) { z = value; };
	int& getUID();
	int& getX();
	int& getY();
	int& getZ();
	int& getID();
	int& getZone();
	void setZone(int zone);
	int& getSpeed();
	void setSpeed(int speed);
	string& getArticle();
	void setArticle(string value);
	string& getDescription();
	void setDescription(string value);
	void setName(string value);
	const string& getName();
	bool getBlockPathfind();
	void setBlockPathfind(bool value);
	void insertItem(Item* insert);
	Item& getTopItem();
	std::vector<Item*>& getAllItems();
	void setItems(std::vector<Item*>& otherItems);
	void clearItems();
	void destroyItemAt(int pos);
private:
	int x, y, z, zone{ 0 }, speed{ 100 };
	int id, uid{ 0 };
	string name, article, description;
	bool blockPathfind{ false };
	std::vector<Item*> items;
};

#endif