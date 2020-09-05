#ifndef PALETTE_WORK_H
#define PALETTE_WORK_H


#include <vector>
#include <string>
#include "../pugixml/pugixml.hpp"

using namespace pugi;


class PaletteItem {
public:
	PaletteItem(int id, bool doubleSize, int x, int y) : id(id), doubleSize(doubleSize), x(x), y(y)
	{};
	PaletteItem() : id(-1), doubleSize(false), x(-1), y(-1)
	{};
	void setID(int v) { id = v; };
	int getID() { return id; };
	int& getIDRef() { return id; };
	int getX() { return x; };
	void setX(int v) { x = v; };
	void setY(int v) { y = v; };
	int getY() { return y; };
	void setDoubleSize(bool v) { doubleSize = v; };
	bool const isDoubleSize() { return doubleSize; };
	void changeAll(int id, bool doubleSize, int x, int y) { this->id = id; this->doubleSize = doubleSize; this->x = x; this->y = y; };
	void changeAll(PaletteItem& p) { this->id = p.getID(); this->doubleSize = p.isDoubleSize(); this->x = p.getX(); this->y = p.getX(); };
	void change_ID_And_Size(int ID, bool size) { id = ID; doubleSize = size; };
	void change_ID_And_Size(PaletteItem& other);
	void changePositions(int x, int y) { this->x = x; this->y = y; };
	PaletteItem& operator=(PaletteItem other);
	bool operator==(const PaletteItem& other);
	bool operator!=(const PaletteItem& other);
	int operator>(const PaletteItem& other);
private:
	int x{ 0 }, y{ 0 }, id{ -1 };
	bool doubleSize{ false };
};

class Palette {
public:
	Palette(std::string name) : name(name) { };
	std::string getName() { return name; }
	void setName(std::string v) { name = v; };
	void addItem(int id, bool doubleSize, size_t xPos, size_t yPos) { if (yPos > maxY) maxY = yPos; items.push_back(PaletteItem(id, doubleSize, xPos, yPos)); };
	void addItem(int id, bool doubleSize);
	void addItem(PaletteItem& item) { if (item.getY() > maxY) maxY = item.getY(); items.push_back(PaletteItem(item)); };
	void addItems(std::vector<PaletteItem>& list);
	std::vector<PaletteItem>& getPalette() { return items; };
	std::vector<PaletteItem> copyPalette() { return items; };
	void replacePalette(std::vector<PaletteItem>& p);
	std::pair<bool, PaletteItem&> getItemAtLocation(int x, int y);
	void clear();
	void removeItemAtLocation(int x, int y);
	void removeItemAtLocation(int x, int y, std::vector<PaletteItem>& changes);
	void removeItemAtLocationUnlessSame(int x, int y, PaletteItem& other);
	void removeItemAtLocationIFDoubleSize(int x, int y);
	void removeItemAtLocationIFDoubleSizeUnlessSame(int x, int y, PaletteItem& other);
	void changePositionAt(int xTo, int yTo, int xSorce, int ySorce);
	void changePositionAt(int xTo, int yTo, PaletteItem& source);
	void changePositionAtUnlessSame(int xTo, int yTo, int xSorce, int ySorce, PaletteItem& other);
	void change_ID_And_Size_At(int id, bool size, int x, int y);
	int getMaxPage(int paletteMaxY) { return items.at(items.size() - 1).getY() / paletteMaxY; };
	static void fillPalettes(std::vector<Palette>& palettes);
	PaletteItem getItemFromID(int ID);
	float getMaxYAsFloat(int maxPage, float maxOffset);
	void findMaxY();
	int getMaxY() { return maxY; };
	int check_If_Double_Size_Is_Blocked_At_Location(int xDst, int yDst, int xSrc, int ySrc);
	int check_If_Double_Size_Is_Possible_At_Location(int xDst, int yDst, int xSrc, int ySrc);
	int check_If_Normal_Size_Is_Possible_At_Location(int x, int y);
	bool is_Double_Size(int x, int y);
	bool item_Exists_at(int x, int y);
	void sortPalette();
private:
	std::vector<PaletteItem> items;
	std::string name{""};
	int maxY{ 0 };
};

class PaletteChange {
public:
	PaletteChange(std::string action, std::vector<PaletteItem> itemsToCreate = {}, std::vector<PaletteItem> itemsToDelete = {}) : action(action), itemsToCreate(itemsToCreate), itemsToDelete(itemsToDelete) {};
	std::vector<PaletteItem>& getItemsToDelete() { return itemsToDelete; };
	std::vector<PaletteItem>& getItemsToCreate() { return itemsToCreate; };
	std::string& getAction() { return action;  };
private:
	std::string action = "";
	std::vector<PaletteItem> itemsToDelete = {};
	std::vector<PaletteItem> itemsToCreate = {};
};

int getFirstPossiblePositionForDoubleSize(std::vector<PaletteItem> items);
int getFirtsPossiblePosition(std::vector<PaletteItem> items);

#endif
