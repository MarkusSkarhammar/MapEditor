#ifndef ITEM_H
#define ITEM_H
#include <string>

using std::string;

class Item {
public:
	Item(int id, string article, string name) : id(id), article(article), name(name)  {};
	Item(Item* i) { *this = *i; };
	virtual ~Item() {};
	int& getID();
	int& getUID();
	void setUID(int id);
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
	void setDescription(string value);
	string& getName();
	string& getDescription();
	string& getArticle();
	string& getType();
	void setType(string value);
	bool& getAnimationState() { return animationOn; };
	void setAnimationState(bool value) { animationOn = value; };
private:
	int id{ 0 }, uID{ 0 };
	string article{ "" }, name{ "" }, description{ "" }, type{ "" };
	bool blockProjectile{ false }, blockPathfind{ false }, blockObject{ false }, moveable{ false }, pickupable{ false }, useable{ false }, hangeable{ false }, alwaysOnTop{ false }, animationOn{ false };
};

class NonStaticItem : public Item {
public:
	NonStaticItem(int id, string article, string name) : Item(id, article, name) { setMoveable(true); setPickupable(true); };
	NonStaticItem(NonStaticItem* n) : Item(n->getID(), n->getArticle(), n->getName()) { *this = *n; };
	virtual ~NonStaticItem() { };
	void setWeight(float value) { weight = value; };
	void setStack(int value) { stack = value; };
	void setMaxStack(int value) { stackMax = value; };
	int& getStack() { return stack; };
	int& getStackMax() { return stackMax; };
	float& getWeight() { return weight; };
private:
	float weight{0.0};
	int stack{ 1 }, stackMax{ 1 };
};

class Weapon : public NonStaticItem {
public:
	Weapon(int id, string article, string name) : NonStaticItem(id, article, name) {};
	Weapon(Weapon* w) : NonStaticItem(w->getID(), w->getArticle(), w->getName()) { *this = *w; };
	~Weapon() {};
	void setAttack(int value) { attack = value; };
	int& getAttack() { return attack; };
	void setDefense(int value) { defense = value; };
	int& getDefense() { return defense; };
	void setWeaponType(std::string& value) { weaponType = value; };
	std::string& getWeaponType() { return weaponType; };
private:
	int attack{ 0 }, defense{ 0 };
	std::string weaponType{ "" };
};

class Armor : public NonStaticItem {
public:
	Armor(int id, string article, string name) : NonStaticItem(id, article, name) {};
	Armor(Armor* a) : NonStaticItem(a->getID(), a->getArticle(), a->getName()) { *this = *a; };
	~Armor() {};
	void setArmor(int value) { armor = value; };
	int& getArmor() { return armor; };
	void setArmorType(std::string& value) { armorType = value; };
	std::string& getArmorType() { return armorType; };
private:
	int armor{ 0 };
	std::string armorType{ "" };
};

void getItemType(Item*& output, int id);

#endif