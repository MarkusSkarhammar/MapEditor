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
#include "Object.h"
using namespace pugi;

class AnimationInfo {
public:
	AnimationInfo(int id, int animationDelay) : id(id), animationDelay(animationDelay) { animationIDs.push_back(id); };
	void setEndAnimation(bool value) { endAnimation = value; };
	int& getEndAnimation() { return endAnimation; };
	int& getID() { return id; };
	void addAnimationID(int a) { animationIDs.push_back(a); };
	int& getAnimationDelay() { return animationDelay; };
	std::vector<int>& getAnimationIDs() { return animationIDs; };
private:
	int id, animationDelay, endAnimation{ 0 };
	std::vector<int> animationIDs;
};

class ItemAtals {
public:
	ItemAtals();
	~ItemAtals();
	void generateAtlas();
	std::vector<size_t> getItemTexturePositionForSelectionArea(std::string category);
	Item* getItem(size_t id);
	std::string& getWeaponType(int& id);
	std::string& getArmorType(int& id);
	std::vector<std::pair<std::string, std::string>> getItem(size_t id, std::string category);
	void addNewAnimation(int itemID, int animationDelay);
	void addAnimation(int itemID, int animationID);
	AnimationObject* getAnimationObject(float x, float y, int id, int VAO, int texturePos);
	bool checkIfAnimation(int id);
	bool checkIfDouleSize(int id);
private:
	pugi::xml_document doc;
	std::vector<Item*> items;
	std::vector<AnimationInfo> animations;
};
#endif