#ifndef ITEM_ATLAS_H
#define ITEM_ATLAS_H
#include "../pugixml/pugixml.hpp"

#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include "ItemInfo.h"
#include "Item.h"
using namespace pugi;

class ItemAtals {
public:
	ItemAtals();
	std::string getTextureLocation(size_t id);
	std::string getGUITextureLocation(size_t id);
	size_t getGUITextureLocationAsNbr(size_t id);
	size_t getItemTexturePosition(size_t id);
	std::vector<size_t> getItemTexturePositionForSelectionArea(std::string category);
	size_t getSelectedItemID(size_t textPos, size_t pos);
	Item getItem(size_t id);
	std::vector<std::pair<std::string, std::string>> getItem(size_t id, std::string category);
	std::vector<ItemInfo> getTiles();
private:
	pugi::xml_document doc;
	std::vector<ItemInfo> items;
	std::vector<ItemInfo> GUI;
};
#endif