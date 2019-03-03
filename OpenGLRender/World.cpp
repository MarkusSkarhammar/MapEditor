#include "World.h"
#include "Global.h"
#include <utility>

void insert_Things_Form_ThingsToDraw()
{
	worldTemp.copyWorld(world);
	bool current(false), east(false), south(false), southEast(false);
	if ( (cutToggle || copyToggle) && copyBuffer.size() > 0) {
		thingsToDraw.clear();
		int smallestX = -1, differenceX = -1, xCopy = 0;
		int smallestY = -1, differenceY = -1, yCopy = 0;
		if (copyBuffer.size() > 0) {
			smallestX = std::min_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getX() < rhs.first.getX()); })->first.getX();
			differenceX = std::max_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getX() < rhs.first.getX()); })->first.getX() - smallestX;
			smallestY = std::min_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getY() < rhs.first.getY()); })->first.getY();
			differenceY = std::max_element(copyBuffer.begin(), copyBuffer.end(), [](std::pair<ToDraw, tile*>& lhs, std::pair<ToDraw, tile*>& rhs) { return (lhs.first.getY() < rhs.first.getY()); })->first.getY() - smallestY;
		}
		for (auto& i : copyBuffer) {
			xCopy = (i.first.getX() - smallestX - int(differenceX / 2) + x);
			yCopy = (i.first.getY() - smallestY - int(differenceY / 2) + y);
			i.second->setX(xCopy);
			i.second->setY(yCopy);
			i.second->setZ(z);
			auto& section = worldTemp.getFloor(z).getSection((xCopy / 50 + (yCopy / 50) * 40));
			if (i.first.getSection() == currentSection) current = true;
			if (i.first.getSection() == currentSection + 1) east = true;
			if (i.first.getSection() == currentSection + 40) south = true;
			if (i.first.getSection() == currentSection + 41) southEast = true;
			auto it = std::find_if(section.begin(), section.end(), [xCopy, yCopy](tile*& t) { return (t->getX() == xCopy && t->getY() == yCopy); });
			if (it == section.end()) {
				section.push_back(new tile(i.second));
			}
			else {
				delete (*it);
				(*it) = new tile(i.second);
			}
		}
	}
	else {
		int x(0), y(0), z(0);
		auto temp = thingsToDraw;
		if (copyToggle || cutToggle) copyBufferLock = true;
		for (auto& element : temp) {
			if (element.getSection() == currentSection) current = true;
			if (element.getSection() == currentSection + 1) east = true;
			if (element.getSection() == currentSection + 40) south = true;
			if (element.getSection() == currentSection + 41) southEast = true;
			x = element.getX(); y = element.getY(); z = element.getZ();
			Item* item = nullptr;
			if (element.getId() >= 0) item = itemAtlas.getItem(element.getId());
			auto& section = worldTemp.getFloor(z).getSection(element.getSection());
			auto it = std::find_if(section.begin(), section.end(), [x, y](tile*& t) { return (t->getX() == x && t->getY() == y); });
			if (it == section.end()) {
				if (element.getId() / 1024 != 0) {
					tile* t = new tile(x, y, z, -1);
					t->setBlockPathfind(true);
					Item* temp = nullptr;
					getItemType(temp, element.getId());
					t->insertItem(temp);
					section.push_back(t);
				}
				else if (element.getId() >= 0) {
					tile* t = new tile(x, y, z, element.getId());
					t->setArticle(item->getArticle());
					t->setName(item->getName());
					t->setDescription(item->getDescription());
					if (item->getBlockPathfind()) t->setBlockPathfind(true);
					section.push_back(t);
				}
			}
			else {
				if (element.getId() < 0) {
					if (eraseToggle) {
						delete (*it);
						section.erase(it);
					}
					else if (destroyToggle) {
						(*it)->clearItems();
						if ((*it)->getID() >= 0 && !itemAtlas.getItem((*it)->getID())->getBlockPathfind()) (*it)->setBlockPathfind(false);
					}
					else if (destroyTileToggle) {
						(*it)->setID(-1);
						(*it)->setArticle("");
						(*it)->setName("");
						(*it)->setDescription("");
						(*it)->setBlockPathfind(true);
					}
					else if (cutToggle) {
						copyBuffer.push_back(std::pair<ToDraw, tile*>(ToDraw((*it)->getID(), element.getX(), element.getY(), element.getZ(), element.getSection()), new tile((*it))));
						delete (*it);
						section.erase(it);
					}
					else if (copyToggle) {
						copyBuffer.push_back(std::pair<ToDraw, tile*>(ToDraw((*it)->getID(), element.getX(), element.getY(), element.getZ(), element.getSection()), new tile((*it))));
					}
				}
				else if (element.getId() / 1024 != 0) {
					Item* temp = nullptr;
					getItemType(temp, element.getId());
					(*it)->insertItem(temp);
					if (temp->getBlockPathfind()) (*it)->setBlockPathfind(true);
				}
				else {
					(*it)->setID(item->getID());
					(*it)->setArticle(item->getArticle());
					(*it)->setName(item->getName());
					(*it)->setDescription(item->getDescription());
					(*it)->setBlockPathfind(item->getBlockPathfind());
				}
			}

		}
	}
	if (copyToggle || cutToggle) copyBufferLock = false;
	if (current)sortSection(currentSection);
	if (east)sortSection(currentSection + 1);
	if (south)sortSection(currentSection + 40);
	if (southEast)sortSection(currentSection + 41);
	
	printf("done\n");
	updateMapFloor = false;
	updateMap = true;
}


void updateWhatToDrawOnAllCurrentFloors() {
	printf("I started loading!\n");
	//while (updateWorld) { printf("I yield!\n"); std::this_thread::yield(); };
	if (z < 7) {
		updateWhatToDrawOnFloors(0, 6, true, true);
	}
	else {
		updateWhatToDrawOnFloors(7, 13, true, true);
	}
	//updateMap = true;
}

void updateWhatToDrawOnFloors(size_t fromFloor, size_t toFloor, bool tiles, bool items) {
	std::thread floors[7];
	size_t offset(0);
	if (fromFloor >= 7) offset = 7;
	for (size_t i = fromFloor; i < toFloor + 1; ++i) {
		floors[i - offset] = std::thread(updateWhatToDrawOnFloor, i, tiles, items);
	}
	for (size_t i = 0; i < 7; ++i) {
		floors[i].join();
	}
	/*for (size_t i = fromFloor; i < toFloor + 1; ++i) {
	updateWhatToDrawOnFloor( i, tiles, items);
	}*/
}

void updateWhatToDrawOnFloor(size_t floor, bool tilesB, bool itemsB) {
	/*float widthStart = 0.0f;
	float heightStart = 0.0f;
	double width = (imgScale / (screenWidth / 2));
	double height = (imgScale / (screenHeight / 2));
	float textWidth = (imgScale / 2048.0f);
	float textHeight = (imgScale / 2048.0f);
	float textOffset = (1 / 2048.0f);
	float textHeightStart = 1.0f;
	float textWidthStart = 0.0f;

	bool east(false), south(false), southEast(false);
	auto& _Floor = world.getFloor(floor);
	int amount = _Floor.getSection(currentSection).size(), atSection = currentSection;
	if (currentSection % SECTIONS_WIDTH != SECTIONS_WIDTH - 1) { east = true; amount += _Floor.getSection(currentSection + 1).size(); }
	if (currentSection / SECTIONS_WIDTH != SECTIONS_WIDTH - 1) { south = true; amount += _Floor.getSection(currentSection + 40).size(); }
	if (currentSection / SECTIONS_WIDTH != SECTIONS_WIDTH - 1 && currentSection % SECTIONS_WIDTH != SECTIONS_WIDTH - 1) { southEast = true; amount += _Floor.getSection(currentSection + 41).size(); }
	auto& _Tiles = tiles.at(floor);
	_Tiles.clearObjects();
	_Tiles.getObjects().reserve(amount);
	std::string vertecesName = "Tiles_1024";
	VertecesHandler vh = VertecesHandler::findByName(verteces, vertecesName);
	for (int i = 0; i < 4; i++) {
		for (auto& tile : _Floor.getSection(atSection)) {
			if (tile->getID() / 1024 == 0 && vh.getName().compare("Tiles_1024") != 0) vh = VertecesHandler::findByName(verteces, "Tiles_1024");
			_Tiles.addObject(new Object(tile->getX(), tile->getY(), tile->getID(), vh.getVAO(), vh.getTextureID()));
		}
		if (i == 0 && east)
			atSection = currentSection + 1;
		else if (i == 1 && south)
			atSection = currentSection + 40;
		else if (i == 2 && southEast)
			atSection = currentSection + 41;
		else
			i++;
	}


	atSection = currentSection;
	auto& _Items = items.at(floor);
	_Items.clearObjects();
	_Items.getObjects().reserve(_Floor.getItemsInSection(currentSection));
	vertecesName = "Doodads_1024";
	vh = VertecesHandler::findByName(verteces, vertecesName);
	for (int i = 0; i < 4; i++) {
		for (auto& tile : _Floor.getSection(atSection)) {
			for (auto& item : tile->getAllItems()) {
				if (item->getID() / 1024 == 1 && vh.getName().compare("Items_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Items_1024");
				}
				if (item->getID() / 1024 == 5 && vh.getName().compare("Nature_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Nature_1024");
				}
				if (item->getID() / 1024 == 3 && vh.getName().compare("Borders_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Borders_1024");
				}
				if (item->getID() / 1024 == 2 && vh.getName().compare("Doodads_1024") != 0) {
					vh = VertecesHandler::findByName(verteces, "Doodads_1024");
				}
				if (itemAtlas.checkIfAnimation(item->getID())) {
					AnimationObject* a = itemAtlas.getAnimationObject(tile->getX(), tile->getY(), item->getID(), vh.getVAO(), vh.getTextureID(), item->getAnimationState());
					_Items.addObject(a);
				}
				else
					_Items.addObject(new Object(tile->getX(), tile->getY(), item->getID(), vh.getVAO(), vh.getTextureID()));
			}
		}
		if (i == 0 && east)
			atSection = currentSection + 1;
		else if (i == 1 && south)
			atSection = currentSection + 40;
		else if (i == 2 && southEast)
			atSection = currentSection + 41;
		else
			i++;
	}*/

	
	/*
	holdFloor = true;
	updatingFloor = z;
	itTiles->setObjects(tiles.getObjects());
	itItems->setObjects(items.getObjects());
	holdFloor = false;
	*/

}

void sortSection(size_t currentSection) {
	auto& section = world.getFloor(z).getSection(currentSection);
	std::sort(section.begin(), section.end(), [](tile*& lhs, tile*& rhs) {
		if (lhs->getX() < rhs->getX()) return true;
		if (rhs->getX() < lhs->getX()) return false;

		// a=b for primary condition, go to secondary
		if (lhs->getY() < rhs->getY()) return true;
		if (rhs->getY() < lhs->getY()) return false;

		return false;
	});
}

bool checkAbove(int xCheck, int yCheck, int section, int zCheck) {
	int from = zCheck, to = z;
	if ((from != z) && ((from < 7 && z == 6) || (from >= 7 && z >= 7))) {
		for (int floor = from + 1; floor <= to; floor++) {
			auto& tiles = world.getFloor(floor).getSection(section);
			auto it = std::find_if(tiles.begin(), tiles.end(), [xCheck, yCheck](tile*& t) { return (t->getX() == xCheck && t->getY() == yCheck && t->getID() != -1); });
			if (it != tiles.end()) {
					return true;
			}
		}
	}
	return false;
}

bool checkAbove(int xCheck, int yCheck, int section, int zCheck, bool bigItem) {
	int tempX(0), tempY(0);
	int from = zCheck, to = z;
	if ((from != z) && ((from < 7 && z == 6) || (from >= 7 && z >= 7))) {
		for (int floor = from + 1; floor <= to; floor++) {
			auto& tiles = world.getFloor(floor).getSection((xCheck / 50 + (yCheck / 50) * 40));
			auto it = std::find_if(tiles.begin(), tiles.end(), [xCheck, yCheck](tile*& t) { return (t->getX() == xCheck && t->getY() == yCheck && t->getID() != -1); });
			if (it != tiles.end()) {
				tempX = xCheck - 1; tempY = yCheck;
				auto& tiles = world.getFloor(floor).getSection(currentSection);
				auto it = std::find_if(tiles.begin(), tiles.end(), [tempX, tempY](tile*& t) { return (t->getX() == tempX && t->getY() == tempY && t->getID() != -1); });
				if (it != tiles.end()) {
					tempX = xCheck - 1; tempY = yCheck - 1;
					auto& tiles = world.getFloor(floor).getSection(currentSection);
					auto it = std::find_if(tiles.begin(), tiles.end(), [tempX, tempY](tile*& t) { return (t->getX() == tempX && t->getY() == tempY && t->getID() != -1); });
					if (it != tiles.end()) {
						tempX = xCheck; tempY = yCheck - 1;
						auto& tiles = world.getFloor(floor).getSection(currentSection);
						auto it = std::find_if(tiles.begin(), tiles.end(), [tempX, tempY](tile*& t) { return (t->getX() == tempX && t->getY() == tempY && t->getID() != -1); });
						if (it != tiles.end()) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void newFloor() {
	size_t from = 0, obj = 0;
	size_t to = z;
	if (z > 6) {
		from = 7;
		to = z;
	}
	std::string currentName = "";
	int section = 0;
	bool skip = false, skipRight = false, skipDown = false;

	// Draw order tiles > Borders > Entities > Doodads
	double x(0), y(0);
	for (int floorAt = from; floorAt <= to; floorAt++) {
		for (int sections = 0; sections < 4; sections++) {
			skip = false;
			switch (sections)
			{
			case 1:
				if (currentSection % 40 != 39)
					section++;
				else {
					skip = true;
					skipRight = true;
				}
				break;
			case 2:
				if(currentSection < 1560)
					section = currentSection + SECTIONS_WIDTH;
				else {
					skip = true;
					skipDown = true;
				}
				break;
			case 3:
				if (!skipRight && !skipDown)
					section = currentSection + SECTIONS_WIDTH + 1;
				else
					skip = true;
				break;
			default:
				section = currentSection;
				break;
			}
			if (!skip)
				for (auto& t : world.getFloor(floorAt).getSection(section)) {
					auto& object = t->getObject();
					object->setDraw(!checkAbove(object->getXPosition(), object->getYPosition(), section, floorAt));
					for (auto& i : t->getAllItems()) {
						auto& object = i->getObject();
						if (itemAtlas.checkIfDouleSize(object->getID()))
							object->setDraw(!checkAbove(object->getXPosition(), object->getYPosition(), section, floorAt, true));
						else
							object->setDraw(!checkAbove(object->getXPosition(), object->getYPosition(), section, floorAt));
					}
				}
		}
	}
	/*
	for (auto& objects : objects) {
		currentName = objects.getName();
		if (currentName.at(currentName.size() - 1) != '_') { // Check to see if Objects container does not contain GUI elements
			obj = std::stoi(currentName.substr(currentName.size() - 1));
			//if(obj = 9 c)
			//if (updateMapFloor && updatingFloor != obj) printf("Hit! \n");
			if (obj >= from && obj <= to) { // Check to see if Object container is within a floor to be drawn
				for (auto& object : objects.getObjects()) {
					if (object->getID() / 1024 == 2)
						object->setDraw(!checkAbove(object->getXPosition(), object->getYPosition(), obj, true));
					else
						object->setDraw(!checkAbove(object->getXPosition(), object->getYPosition(), obj));
				}
			}
		}
	}
	*/
	updateMapFloor = false;
}