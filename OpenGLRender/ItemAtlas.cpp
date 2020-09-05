#include "ItemAtlas.h"
#include <chrono>

ItemAtals::ItemAtals()
{
	
}

ItemAtals::~ItemAtals() {
	for (auto& item : items) {
		delete item;
	}
}

void ItemAtals::generateAtlas()
{
	pugi::xml_parse_result result = doc.load_file("./resources/Items/Items.xml");
	std::vector<string> types;
	std::string typeS;
	for (pugi::xml_node type = doc.child("Items").child("Types").child("Type"); type; type = type.next_sibling("Type"))
	{
		typeS = type.attribute("id").as_string();
		types.push_back(typeS);
	}
	std::string article, name;
	size_t id;
	for (size_t i = 0; i < types.size(); i++) {
		const pugi::string_t s(types.at(i));
		for (pugi::xml_node item = doc.child("Items").child(&s[0]).child("Item"); item; item = item.next_sibling("Item"))
		{
			typeS = "";
			id = item.attribute("id").as_int();
			article = item.attribute("article").as_string();
			name = item.attribute("name").as_string();
			typeS = item.attribute("type").as_string();
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
			items.push_back(temp);
		}
	}
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
	auto it = std::find_if(items.begin(), items.end(), [id](Item*& item) { return (item->getID() == id); });
	// To do: Throw error if item doesn't exist.
	return (*it);
}

std::string& ItemAtals::getWeaponType(int& id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](Item*& item) { return (item->getID() == id); });
	if (Weapon* check = dynamic_cast<Weapon*>((*it))) {
		return check->getWeaponType();
	}
	std::string s = "";
	return s;
}

std::string& ItemAtals::getArmorType(int& id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](Item*& item) { return (item->getID() == id); });
	if (Armor* check = dynamic_cast<Armor*>((*it))) {
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
	if (ID / 1024 == 2) {
		return true;
	}
	else if (ID / 1024 == 5) {
		return true;
	}
	return false;
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
