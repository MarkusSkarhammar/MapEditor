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
	void getSectionsAtFloor(World& world, int floor, int from, int to);
	void getTilesFromFile(vector<tile>& tiles, string fileName);
	void writeTilesToFile(string fileName, World& world);
	void writeLevel(char* memblock, int amount);
	void writeAmountTiles(char* memblock, int amount);
	void writeAmountItems(char* memblock, int amount);
	void writeCords(char * memblock, tile*& t);
	void writeID(char * memblock, int& id);
	void write3Int(char * memblock, int & id);
	void writeZone(char * memblock, int& zone);
	void writeSpeed(char * memblock, int& speed);
	void writeArticle(char * memblock, string& article);
	void writeDescription(char * memblock, string& description);
	void writeTileBooleans(char * memblock, tile*& t);
	void writeItemBooleans(char * memblock, Item*& i);
	void writeItems(ofstream & file, char * memblock, tile*& t);
	void writeTile(ofstream& file, char* memblock, tile*& t, streampos& size, size_t& pos);
	int getFloor(char * memblock);
	size_t getAmountOfTiles(char * memblock);
	void getTileCords(char* memblock, int& x, int& y, int& z);
	void getID(char* memblock, int& id);
	void get3Int(char * memblock, int & id);
	template <class T>
	void getNonStaticItemValues(ifstream & file, char * memblock, int& size, std::string weight, int& stack, int & stackMax, T& c);
	void getZone(char* memblock, int& zone);
	void getSpeed(char* memblock, int& speed);
	void getArticle(ifstream & file, char* memblock, string& article);
	void getDescription(ifstream & file, char* memblock, string& description);
	void getBooleans(ifstream & file, char* memblock, bool& moveable);
	void getTileItems(ifstream & file, char* memblock, vector<Item*>& items, bool section);
	void getItemBooleans(ifstream & file, char* memblock, bool& blockProjectile, bool& blockPathfind, bool& blockObject, bool& moveable, bool& pickupable, bool& useable, bool& hangeable, bool& alwaysOnTop, bool& animationOn);
	tile getTile(char * memblock, const size_t& pos);
	void sortTileVector(std::vector<tile>& tileVector);
};

#endif