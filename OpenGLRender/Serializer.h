#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <vector>
#include "Tile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "World.h"

using std::vector;
using std::string;
using std::streampos;
using std::ofstream;
using std::ios;
using std::cout;
using std::ifstream;
using std::fstream;

class Serialize {
public:
	Serialize() {};
	void saveWorld(World& world);
	void loadWorld(World& world);
	void loadSection(vector<tile>& tiles, size_t floor, size_t section);
private:
	void getTilesFromFile(vector<tile>& tiles, string fileName);
	void writeTilesToFile(string fileName, vector<tile>& tiles);
	void writeAmount(char* memblock, int amount);
	void writeCords(char * memblock, tile & t);
	void writeID(char * memblock, size_t& id);
	void writeZone(char * memblock, size_t& zone);
	void writeSpeed(char * memblock, size_t& speed);
	void writeArticle(char * memblock, string& article);
	void writeDescription(char * memblock, string& description);
	void writeTileBooleans(char * memblock, tile& t);
	void writeItemBooleans(char * memblock, Item& i);
	void writeItems(ofstream & file, char * memblock, tile& t);
	void writeTile(ofstream& file, char* memblock, tile& t, streampos& size, size_t& pos);
	size_t getAmountOfTiles(char * memblock);
	void getTileCords(char* memblock, size_t& x, size_t& y, size_t& z);
	void getID(char* memblock, size_t& id);
	void getZone(char* memblock, size_t& zone);
	void getSpeed(char* memblock, size_t& speed);
	void getArticle(ifstream & file, char* memblock, string& article);
	void getDescription(ifstream & file, char* memblock, string& description);
	void getBooleans(ifstream & file, char* memblock, bool& moveable);
	void getTileItems(ifstream & file, char* memblock, vector<Item>& items);
	void getItemBooleans(ifstream & file, char* memblock, bool& blockProjectile, bool& blockPathfind, bool& blockObject, bool& moveable, bool& pickupable, bool& useable, bool& hangeable, bool& alwaysOnTop);
	tile getTile(char * memblock, const size_t& pos);
	void sortTileVector(std::vector<tile>& tileVector);
};

#endif