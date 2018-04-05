#include "ItemAtlas.h"

ItemAtals::ItemAtals()
{
	pugi::xml_parse_result result = doc.load_file("./resources/Items/Items.xml");
	size_t id, posInText, GUITextPos;
	std::string textPath, type;
	for (pugi::xml_node item = doc.child("Items").child("Item"); item; item = item.next_sibling("Item"))
	{
		id = item.attribute("id").as_int();
		textPath = item.attribute("texture").as_string();
		posInText = item.attribute("pos").as_int();
		GUITextPos = item.attribute("textPos").as_int();
		type = item.attribute("type").as_string();
		items.push_back(ItemInfo(id, posInText, GUITextPos, textPath, type));
	}
	for (pugi::xml_node item = doc.child("Items").child("GUIs").child("GUI"); item; item = item.next_sibling("GUI"))
	{
		id = item.attribute("id").as_int();
		textPath = item.attribute("texture").as_string();
		posInText = 0;
		GUITextPos = 0;
		type = "";
		GUI.push_back(ItemInfo(id, posInText, GUITextPos, textPath, type));
	}
}

std::string ItemAtals::getTextureLocation(const size_t id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](ItemInfo& iF) { return iF.getID() == id; });
	if (it != items.end()) {
		return (*it).getTextPath();
	}
	return "";

}

std::string ItemAtals::getGUITextureLocation(size_t id)
{

	auto it = std::find_if(GUI.begin(), GUI.end(), [id](ItemInfo& iF) { return iF.getID() == id; });
	if (it != GUI.end()) {
		return (*it).getTextPath();
	}
	return "";
}

size_t ItemAtals::getGUITextureLocationAsNbr(size_t id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](ItemInfo& iF) { return iF.getID() == id; });
	if (it != items.end()) {
		return (*it).getGUITextPos();
	}
	return 10000;
}

size_t ItemAtals::getItemTexturePosition(size_t id)
{
	auto it = std::find_if(items.begin(), items.end(), [id](ItemInfo& iF) { return iF.getID() == id; });
	if (it != items.end()) {
		return (*it).getPosInText();
	}
	return 0;
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

size_t ItemAtals::getSelectedItemID(size_t textPos, size_t pos)
{
	auto it = std::find_if(items.begin(), items.end(), [textPos, pos](ItemInfo& iF) { return iF.getPosInText() == pos && iF.getGUITextPos() == textPos; });
	if (it != items.end()) {
		return (*it).getID();
	}

	return 100000;
}

Item ItemAtals::getItem(size_t id)
{
	const pugi::string_t s2(std::to_string(id));
	pugi::xml_node node = doc.child("Items").find_child_by_attribute("id", &s2[0]);
	string category = node.attribute("type").as_string();
	std::vector<std::pair<std::string, std::string>> itemInfo;
	const pugi::string_t s(category);
	node = doc.child("Items").child(&s[0]).find_child_by_attribute("id", &s2[0]);
	Item item(id, node.attribute("article").as_string(), node.attribute("name").as_string());
	item.setType(category);
	for (pugi::xml_node attribute = node.child("attribute"); attribute; attribute = attribute.next_sibling("attribute"))
	{
		itemInfo.push_back(std::pair<std::string, std::string>(attribute.attribute("key").value(), attribute.attribute("value").value()));

	}
	for (auto& iF : itemInfo) {
		if (iF.first.compare("blockProjectile") == 0) {
			if (iF.second.compare("true")) item.setBlockProjectile(true);
			else
				item.setBlockProjectile(false);
		}
		if (iF.first.compare("blockPathfind") == 0) {
			if (iF.second.compare("true") == 0) {
				item.setBlockPathfind(true);
				item.setMoveable(false);
			}
			else
				item.setBlockPathfind(false);
		}
		if (iF.first.compare("blockObject") == 0) {
			if (iF.second.compare("true") == 0) item.setBlockObject(true);
			else
				item.setBlockObject(false);
		}
		if (iF.first.compare("moveable") == 0) {
			if (iF.second.compare("true") == 0) item.setMoveable(true);
			else
				item.setMoveable(false);
		}
		if (iF.first.compare("pickupable") == 0) {
			if (iF.second.compare("true") == 0) item.setPickupable(true);
			else
				item.setPickupable(false);
		}
		if (iF.first.compare("useable") == 0) {
			if (iF.second.compare("true") == 0) item.setUseable(true);
			else
				item.setUseable(false);
		}
		if (iF.first.compare("hangeable") == 0) {
			if (iF.second.compare("true") == 0) item.setHangeable(true);
			else
				item.setHangeable(false);
		}
		if (iF.first.compare("alwaysOnTop") == 0) {
			if (iF.second.compare("true") == 0) item.setAlwaysOnTop(true);
			else
				item.setAlwaysOnTop(false);
		}
	}
	return item;
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

std::vector<ItemInfo> ItemAtals::getTiles()
{
	std::vector<ItemInfo> tiles;
	for (auto& i : items) {
		if (i.getType().compare("Tiles") == 0) {
			tiles.push_back(i);
		}
	}
	return tiles;
}
