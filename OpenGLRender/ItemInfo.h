#ifndef ITEM_INFO_H
#define ITEM_INFO_H

#include <string>

class ItemInfo {
public:
	ItemInfo(size_t id, size_t textID) : id(id), textID(textID) {};
	size_t getID() { return id; };
private:
	size_t id, textID;
};

#endif