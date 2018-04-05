#ifndef ITEM_H
#define ITEM_H
#include <string>

using std::string;

class Item {
public:
	Item(size_t id, string article, string description) : id(id), article(article), description(description)  {};
	size_t getID();
	size_t getUID();
	void setUID(size_t id);
	bool getBlockProjectile();
	void setBlockProjectile(bool value);
	bool getBlockPathfind();
	void setBlockPathfind(bool value);
	bool getBlockObject();
	void setBlockObject(bool value);
	bool getMoveable();
	void setMoveable(bool value);
	bool getPickupable();
	void setPickupable(bool value);
	bool getUseable();
	void setUseable(bool value);
	bool getHangeable();
	void setHangeable(bool value);
	bool getAlwaysOnTop();
	void setAlwaysOnTop(bool value);
	string getDescription();
	string getArticle();
	string getType();
	void setType(string value);
private:
	size_t id, uID{ 0 };
	string article, description, type{""};
	bool blockProjectile{ false }, blockPathfind{ false }, blockObject{ false }, moveable{ true }, pickupable{ false }, useable{ false }, hangeable{ false }, alwaysOnTop{ false };
};

#endif