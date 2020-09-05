#include "ItemAtlas.h"
#include "Vertices.h"
#include "ObjectLibrary.h"
#include <chrono>

ItemAtals::ItemAtals()
{
	
}

ItemAtals::~ItemAtals() {
	for (auto& item : items) {
		delete item.first;
	}
}

void ItemAtals::generateAtlas()
{
	items.push_back(std::pair<Item*, Object*>(new Item(-1, "", ""), nullptr));
	pugi::xml_parse_result result = doc.load_file("./resources/Items/Items.xml");
	std::vector<string> types;
	std::string typeS;
	for (pugi::xml_node type = doc.child("Items").child("Types").child("Type"); type; type = type.next_sibling("Type"))
	{
		typeS = type.attribute("id").as_string();
		types.push_back(typeS);
	}
	std::string article, name, category;
	size_t id, objID;
	for (size_t i = 0; i < types.size(); i++) {
		const pugi::string_t s(types.at(i));
		for (pugi::xml_node item = doc.child("Items").child(&s[0]).child("Item"); item; item = item.next_sibling("Item"))
		{
			typeS = "";
			id = item.attribute("id").as_int();
			article = item.attribute("article").as_string();
			name = item.attribute("name").as_string();
			typeS = item.attribute("type").as_string();
			category = item.attribute("category").as_string();
			objID = item.attribute("objectID").as_int();

			Item* temp = nullptr;
			if (typeS.compare("Weapon") == 0) {
				temp = new Weapon(id, article, name);
			}
			else if (typeS.compare("Armor") == 0) {
				temp = new Armor(id, article, name);
			}
			else if (typeS.compare("non") == 0) {
				temp = new NonStaticItem(id, article, name);
			}
			else {
				temp = new Item(id, article, name);
			}


			if (typeS.size() != 0) temp->setType(typeS);
			for (pugi::xml_node attribute = item.child("attribute"); attribute; attribute = attribute.next_sibling("attribute"))
			{
				bool animation = false;
				std::pair<std::string, std::string> info(attribute.attribute("key").value(), attribute.attribute("value").value());
				if (info.first.compare("blockProjectile") == 0) {
					temp->setBlockProjectile(std::stoi(info.second));
				}
				else if (info.first.compare("blockPathfind") == 0) {
					temp->setBlockPathfind(std::stoi(info.second));
				}
				else if (info.first.compare("blockObject") == 0) {
					temp->setBlockObject(std::stoi(info.second));
				}
				else if (info.first.compare("moveable") == 0) {
					temp->setMoveable(std::stoi(info.second));
				}
				else if (info.first.compare("pickupable") == 0) {
					temp->setPickupable(std::stoi(info.second));
				}
				else if (info.first.compare("useable") == 0) {
					temp->setUseable(std::stoi(info.second));
				}
				else if (info.first.compare("hangeable") == 0) {
					temp->setHangeable(std::stoi(info.second));
				}
				else if (info.first.compare("alwaysOnTop") == 0) {
					temp->setAlwaysOnTop(std::stoi(info.second));
				}
				else if (info.first.compare("description") == 0) {
					temp->setDescription(info.second);
				}
				else if (info.first.compare("animationOn") == 0) {
					temp->setAnimationState(std::stoi(info.second));
				}
				else if (info.first.compare("animationDelay") == 0) {
					addNewAnimation(id, std::stoi(info.second));
					temp->setAnimationState(std::stoi(info.second));
				}
				else if (info.first.compare("animationID") == 0) {
					addAnimation(id, std::stoi(info.second));
				}
				else if (info.first.compare("animationOnly") == 0) {
					temp->setAnimationOnly(true);
				}
				else if (info.first.compare("doubleSize") == 0) {
					temp->setDoubleSize(true);
				}
				else if (info.first.compare("weight") == 0) {
					if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(temp)) {
						check->setWeight(std::stof(info.second));
					}
				}
				else if (info.first.compare("stack") == 0) {
					if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(temp)) {
						check->setStack(std::stoi(info.second));
					}
				}
				else if (info.first.compare("stackMax") == 0) {
					if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(temp)) {
						check->setMaxStack(std::stoi(info.second));
					}
				}
				else if (info.first.compare("attack") == 0) {
					if (Weapon* check = dynamic_cast<Weapon*>(temp)) {
						check->setAttack(std::stoi(info.second));
					}
				}
				else if (info.first.compare("defense") == 0) {
					if (Weapon* check = dynamic_cast<Weapon*>(temp)) {
						check->setDefense(std::stoi(info.second));
					}
				}
				else if (info.first.compare("armor") == 0) {
					if (Armor* check = dynamic_cast<Armor*>(temp)) {
						check->setArmor(std::stoi(info.second));
					}
				}
				else if (info.first.compare("armorType") == 0) {
					if (Armor* check = dynamic_cast<Armor*>(temp)) {
						check->setArmorType(info.second);
					}
				}
			}
			auto tempObj = getObjectFromLibrary(category, objID);
			items.push_back(std::pair<Item*, Object*>(temp, tempObj));
		}
	}
	std::sort(items.begin(), items.end(), [](std::pair<Item*, Object*>& lhs, std::pair<Item*, Object*>& rhs) {
		return lhs.first->getID() < rhs.first->getID();
		});
}

std::vector<size_t> ItemAtals::getItemTexturePositionForSelectionArea(const std::string category)
{
	std::vector<size_t> items;
	const pugi::string_t s(category);
	for (pugi::xml_node item = doc.child("Items").child(&s[0]).child("Item"); item; item = item.next_sibling("Item"))
	{
		items.push_back(item.attribute("id").as_int());

	}
	std::vector<size_t> position;
	for (const auto& i : items) {
		for (pugi::xml_node item = doc.child("Items").child("Item"); item; item = item.next_sibling("Item"))
		{
			if (item.attribute("id").as_int() == i) {
				position.push_back(item.attribute("textPos").as_int());
				break;
			}
		}
	}

	return position;
}

/*size_t ItemAtals::getSelectedItemID(size_t textPos, size_t pos)
{
	auto it = std::find_if(items.begin(), items.end(), [textPos, pos](ItemInfo& iF) { return iF.getPosInText() == pos && iF.getGUITextPos() == textPos; });
	if (it != items.end()) {
		return (*it).getID();
	}

	return 100000;
}*/

Item* ItemAtals::getItem(size_t id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](std::pair<Item*, Object*>& item) { return (item.first->getID() == id); });
	// To do: Throw error if item doesn't exist.
	if (it == items.end())
		return items.begin()->first;
	else
		return it->first;
}

Object* ItemAtals::getItemObject(int id)
{
	if (items.size() > 0) {
		auto it = std::find_if(items.begin(), items.end(), [id](std::pair<Item*, Object*>& item) { return (item.first->getID() == id); });
		// To do: Throw error if item doesn't exist.
		if (it == items.end())
			return items.begin()->second;
		else
			return it->second;
	}
	else
		return nullptr;
}

std::pair<Item*, Object*> ItemAtals::getItemAndObject(int id)
{
	auto item = std::pair<Item*, Object*>(nullptr, nullptr);
	auto it = std::find_if(items.begin(), items.end(), [id](std::pair<Item*, Object*>& item) { return (item.first->getID() == id); });
	if (it != items.end())
		item = std::pair<Item*, Object*>(it->first, it->second);
	
	return item;
}

std::string& ItemAtals::getWeaponType(int& id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](std::pair<Item*, Object*>& item) { return (item.first->getID() == id); });
	if (Weapon* check = dynamic_cast<Weapon*>(it->first)) {
		return check->getWeaponType();
	}
	std::string s = "";
	return s;
}

std::string& ItemAtals::getArmorType(int& id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](std::pair<Item*, Object*>& item) { return (item.first->getID() == id); });
	if (Armor* check = dynamic_cast<Armor*>(it->first)) {
		return check->getArmorType();
	}
	std::string s = "";
	return s;
}

std::vector<std::pair<std::string, std::string>> ItemAtals::getItem(size_t id, std::string category)
{
	std::vector<std::pair<std::string, std::string>> item;
	const pugi::string_t s(category);
	const pugi::string_t s2(std::to_string(id));
	pugi::xml_node node = doc.child("Items").child(&s[0]).find_child_by_attribute("id", &s2[0]);
	item.push_back(std::pair<std::string, std::string>("id", node.attribute("id").value()));
	item.push_back(std::pair<std::string, std::string>("name", node.attribute("name").value()));
	item.push_back(std::pair<std::string, std::string>("article", node.attribute("article").value()));
	for (pugi::xml_node attribute = node.child("attribute"); attribute; attribute = attribute.next_sibling("attribute"))
	{
		item.push_back(std::pair<std::string, std::string>(attribute.attribute("key").value(), attribute.attribute("value").value()));

	}

	return item;
}
void ItemAtals::addNewAnimation(int itemID, int animationDelay) {
	auto it = std::find_if(animations.begin(), animations.end(), [itemID](AnimationInfo& a) { return (a.getID() == itemID); });
	if (it == animations.end()) {
		animations.push_back(AnimationInfo(itemID, animationDelay));
	}
};

void ItemAtals::addAnimation(int itemID, int animationID) {
	auto it = std::find_if(animations.begin(), animations.end(), [itemID](AnimationInfo& a) { return (a.getID() == itemID); });
	if (it != animations.end()) {
		(*it).addAnimationID(animationID);
	}
};


AnimationObject* ItemAtals::getAnimationObject(float x, float y, int id, int VAO, int texturePos) {
	auto it = std::find_if(animations.begin(), animations.end(), [id](AnimationInfo& a) { return (a.getID() == id); });
	if (it != animations.end()) {
		AnimationObject* a = new AnimationObject(x, y, id, VAO, texturePos, (*it).getAnimationDelay(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
		if ((*it).getEndAnimation() != 0) a->setEndAnimation((*it).getEndAnimation());
		for (auto& aID : (*it).getAnimationIDs() ) {
			a->insertAnimationID(aID);
		}
		return a;
	}
	else
		return nullptr;
};

bool ItemAtals::checkIfAnimation(int id) {
	auto it = std::find_if(animations.begin(), animations.end(), [id](AnimationInfo& a) { return (a.getID() == id); });
	if (it != animations.end()) {
		return true;
	}
	else
		return false;
}
bool ItemAtals::checkIfDouleSize(int ID)
{
	Item* item = getItem(ID);
	if (item->getID() != -1)
		return item->isDoubleSize();
	else
		return 0;
}

bool ItemAtals::checkIfDouleSize(int texturePos, int id)
{
	auto it = std::find_if(items.begin(), items.end(), [texturePos, id](std::pair<Item*, Object*>& item) { 
		if (!item.second)
			return false;
		auto v = item.second->getVertices(); 
		return (v->getTextPos() == texturePos && v->getID() == id);
		});
	// To do: Throw error if item doesn't exist.
	if (it == items.end())
		return false;
	else
		return it->first->isDoubleSize();
}

/*std::vector<ItemInfo> ItemAtals::getTiles()
{
	std::vector<ItemInfo> tiles;
	for (auto& i : items) {
		if (i.getType().compare("Tiles") == 0) {
			tiles.push_back(i);
		}
	}
	return tiles;
}*/
