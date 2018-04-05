#ifndef ITEM_INFO_H
#define ITEM_INFO_H

#include <string>

class ItemInfo {
public:
	ItemInfo(size_t id, size_t posInText, size_t GUITextPos, std::string textPath, std::string type) : id(id), posInText(posInText), GUITextPos(GUITextPos), textPath(textPath), type(type) {};
	size_t getID() { return id; };
	size_t getPosInText() { return posInText; };
	size_t getGUITextPos() { return GUITextPos; };
	std::string getTextPath() { return textPath; };
	std::string getType() { return type; };
private:
	size_t id, posInText, GUITextPos;
	std::string textPath, type;
};

#endif