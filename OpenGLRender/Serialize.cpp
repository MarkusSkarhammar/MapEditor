#include "Serializer.h"
#include "Global.h"
void Serialize::saveWorld(World& world)
{
	writeTilesToFile("./resources/map/" + world.getName() + ".bin", world);
}

void Serialize::loadWorld(World& world)
{
	//for (size_t f = 0; f < 14; ++f) {
		//getSectionsAtFloor(world, f, 0, 1);
	//}

	getSectionsAtFloor(world, MAX_FLOORS, 0, 1);
	printf("Done with world load. \n");
	//updateWhatToDrawOnAllCurrentFloors();

	updateWorld = true;
}

void Serialize::loadSection(vector<tile>& tiles, size_t floor, size_t section)
{
	tiles.clear();
	tiles.shrink_to_fit();
	getTilesFromFile(tiles, "./resources/map/" + std::to_string(floor) + "/" + std::to_string(section) + ".bin");
}


void Serialize::getSectionsAtFloor(World& world, int floor, int from, int amount) {
	int x, y, z, id, speed, zone, uid;
	string article, description, name;
	bool blockPathfind;
	vector<Item*> items;
	size_t size;
	int fromS(0), toS(0);
	int tilesSize;
	int itemsSize;
	int currentFloor;
	tile *t(nullptr);
	char * memblock;
	ifstream file("./resources/map/" + world.getName() + ".bin", ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		file.seekg(0, ios::beg);
		for (int floors = 0; floors < floor; floors++) {
			size = 1;
			memblock = new char[size];
			//memblock[0] = 0;
			file.read(memblock, size);
			currentFloor =  getFloor(memblock);
			delete[] memblock;

			for (int sections = 0; sections < SECTIONS_WIDTH*SECTIONS_WIDTH; sections++) {
				fromS = (from % 40) + ((sections / 40) * 40);
				toS = fromS + amount;
				size = 2;
				memblock = new char[size];
				file.read(memblock, size);
				tilesSize = getAmountOfTiles(memblock);
				//tiles.reserve(tilesSize);
				delete[] memblock;
				auto& _Tiles = world.getFloor(currentFloor).getSection(sections);
				if (sections >= fromS && sections <= toS) {
					world.getFloor(currentFloor).clearSection(sections);
					_Tiles.clear();
					_Tiles.reserve(tilesSize);
				}
				if (tilesSize != 0) {
					for (size_t i = 0; i < tilesSize; ++i) {

						// Get Coords
						size = 6;
						memblock = new char[size];
						memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; memblock[3] = 0; memblock[4] = 0; memblock[5] = 0; // ID
						file.read(memblock, size);
						getTileCords(memblock, x, y, z);
						delete[] memblock;

						// Get ID
						size = 4;
						memblock = new char[size];
						//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
						file.read(memblock, size);
						getID(memblock, id);
						delete[] memblock;
						if (id == 65000)
							id = -1;
						Item* tempItem = nullptr;
						if(id != -1)	tempItem = itemAtlas.getItem(id);

						// Get UID
						size = 4;
						memblock = new char[size];
						//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
						file.read(memblock, size);
						getID(memblock, uid);
						delete[] memblock;
						
						// Get zone
						size = 1;
						memblock = new char[size];
						//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
						file.read(memblock, size);
						getZone(memblock, zone);
						delete[] memblock;

						// Get speed
						size = 1;
						memblock = new char[size];
						//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
						file.read(memblock, size);
						getSpeed(memblock, speed);
						delete[] memblock;

						// Get article
						getArticle(file, memblock, article);
						if (id >= 0 && article.size() == 0)
							article = tempItem->getArticle();

						// Get name
						getDescription(file, memblock, name);
						if (id >= 0 && name.size() == 0)
							name = tempItem->getName();

						// Get description
						getDescription(file, memblock, description);
						if (id >= 0 && description.size() == 0)
							description = tempItem->getDescription();

						// Get booleans
						getBooleans(file, memblock, blockPathfind);

						// Get Items
						getTileItems(file, memblock, items, (sections >= fromS && sections <= toS));


						if (sections >= fromS && sections <= toS) {
							t = new tile(x, y, z, id);

							if (zone != 0) t->setZone(zone);
							if (speed != 100) t->setSpeed(speed);
							if (article.size() != 0) t->setArticle(article);
							if (name.size() != 0) t->setName(name);
							if (description.size() != 0) t->setDescription(description);
							if (blockPathfind != 0) t->setBlockPathfind(blockPathfind);
							if (items.size() != 0) t->setItems(items);

							_Tiles.push_back(t);
							//delete t;
						}
					}
				}
			}
		}
		file.close();
		//sortTileVector(tiles);
	}
};

int Serialize::getFloor(char * memblock) {
	int value = 0;
	for (int i = 0; i < 8; ++i) {
		if ((memblock[0] & (1 << i)) != 0) {
			value |= (1u << i);
		}
	}
	cout << "Level: " << value << std::endl;
	return value;
}


void Serialize::getTilesFromFile(vector<tile>& tiles, string fileName)
{
	int x, y, z, id, speed, zone;
	string article, description;
	bool blockPathfind;
	vector<Item*> items;
	size_t size;
	tile *t(nullptr);
	char * memblock;
	ifstream file(fileName, ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		size = 2;
		memblock = new char[size];
		//memblock[0] = 0; memblock[1] = 0;
		file.seekg(0, ios::beg);
		file.read(memblock, size);
		const size_t tilesSize = getAmountOfTiles(memblock);
		tiles.reserve(tilesSize);
		delete[] memblock;
		for (size_t i = 0; i < tilesSize; ++i) {

			// Get Coords
			size = 6;
			memblock = new char[size];
			memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; memblock[3] = 0; memblock[4] = 0; memblock[5] = 0; // ID
			file.read(memblock, size);
			getTileCords(memblock, x, y, z);
			delete[] memblock;
			
			// Get ID
			size = 3;
			memblock = new char[size];
			//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
			file.read(memblock, size);
			getID(memblock, id);
			delete[] memblock;

			// Get zone
			size = 1;
			memblock = new char[size];
			//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
			file.read(memblock, size);
			getZone(memblock, zone);
			delete[] memblock;

			// Get speed
			size = 1;
			memblock = new char[size];
			//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
			file.read(memblock, size);
			getSpeed(memblock, speed);
			delete[] memblock;

			// Get article
			getArticle(file, memblock, article);

			// Get description
			getDescription(file, memblock, description);

			// Get booleans
			getBooleans(file, memblock, blockPathfind);

			bool yes = true;
			// Get Items
			getTileItems(file, memblock, items, yes);


			t = new tile(x, y, z, id);

			if (zone != 0) t->setZone(zone);
			if (speed != 100) t->setSpeed(speed);
			if (article.size() != 0) t->setArticle(article);
			if (description.size() != 0) t->setName(description);
			if (blockPathfind != 1) t->setBlockPathfind(blockPathfind);
			//if (items.size() != 0) t->setItems(items);

			tiles.push_back(*t);
			delete t;
		}
		if(tilesSize == 0 ) delete[] memblock;
		file.close();
		//sortTileVector(tiles);
	}
}

void Serialize::writeTilesToFile(string fileName, World& world)
{
	streampos size;
	char * memblock;

	ofstream file("./resources/map/tempMap.bin", ios::out | ios::binary | std::ofstream::trunc);
	
	if (file.is_open())
	{
		file.seekp(0, ios::beg);
		for (int floor = 0; floor < MAX_FLOORS; floor++) {
			memblock = new char[1];
			writeLevel(memblock, floor);
			file.write(memblock, 1);
			delete[] memblock;
			for (int section = 0; section < SECTIONS_WIDTH*SECTIONS_WIDTH; section++) {
				auto& sectionRef = world.getFloor(floor).getSection(section);
				size_t tileSize = 2;
				size_t pos = tileSize;
				memblock = new char[tileSize];
				writeAmountTiles(memblock, sectionRef.size());
				file.write(memblock, tileSize);
				delete[] memblock;
				/*tileSize = 2;
				memblock = new char[tileSize];
				writeAmountTiles(memblock, world.getFloor(floor).getItemsInSection(section));
				file.write(memblock, tileSize);
				delete[] memblock;*/
				if (sectionRef.size() != 0) {
					for (auto& t : sectionRef) {
						if (t != nullptr) {
							writeTile(file, memblock, t, size, pos);
						}
					}
				}
			}
		}
		file.close();
	}
	std::ifstream  src("./resources/map/tempMap.bin", std::ios::binary);
	std::ofstream  dst(fileName, std::ios::binary);

	dst << src.rdbuf();
	src.close();

	if (remove("./resources/map/tempMap.bin") != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");

}

void Serialize::writeLevel(char* memblock, int amount) {
	memblock[0] = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((amount & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
};


void Serialize::writeAmountTiles(char* memblock, int amount) {
	memblock[0] = 0;
	memblock[1] = 0;
	//memblock[2] = 0;
	//memblock[3] = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((amount & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
	for (size_t i = 8; i < 16; ++i) {
		if ((amount & (1 << i)) != 0) {
			memblock[1] |= (1u << (i - 8));
		}
	}
	/*for (size_t i = 16; i < 24; ++i) {
		if ((amount & (1 << i)) != 0) {
			memblock[2] |= (1u << i - 16);
		}
	}
	for (size_t i = 24; i < 32; ++i) {
		if ((amount & (1 << i)) != 0) {
			memblock[3] |= (1u << i - 24);
		}
	}*/
}

void Serialize::writeAmountItems(char* memblock, int amount) {
	memblock[0] = 0;
	memblock[1] = 0;
	//memblock[2] = 0;
	//memblock[3] = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((amount & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
	for (size_t i = 8; i < 16; ++i) {
		if ((amount & (1 << i)) != 0) {
			memblock[1] |= (1u << (i - 8));
		}
	}
	/*for (size_t i = 16; i < 24; ++i) {
	if ((amount & (1 << i)) != 0) {
	memblock[2] |= (1u << i - 16);
	}
	}
	for (size_t i = 24; i < 32; ++i) {
	if ((amount & (1 << i)) != 0) {
	memblock[3] |= (1u << i - 24);
	}
	}*/
}

/**
*	Function for getting the coordinates of a tile into bits.
*	@params: char* for storing the bits, tile& for getting the coordinates.
*/
void Serialize::writeCords(char* memblock, tile*& t) {
	memblock[0] = 0; memblock[1] = 0; // X
	memblock[2] = 0; memblock[3] = 0; // Y
	memblock[4] = 0; memblock[5] = 0; // Z
	int amount = 0, first = 0, second = 0;
	for (size_t j = 0; j < 6; j += 2) {
		switch (j)
		{
		case 0:
			amount = t->getX();
			first = 0;
			second = 1;
			break;
		case 2:
			amount = t->getY();
			first = 2;
			second = 3;
			break;
		case 4:
			amount = t->getZ();
			first = 4;
			second = 5;
			break;
		}
		for (size_t i = 0; i < 8; ++i) {
			if ((amount & (1 << i)) != 0) {
				memblock[first] |= (1u << i);
			}
		}
		for (size_t i = 8; i < 16; ++i) {
			if ((amount & (1 << i)) != 0) {
				memblock[second] |= (1u << (i - 8));
			}
		}
	}
}

void Serialize::writeID(char * memblock, int & id)
{
	memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; memblock[3] = 0; // id
	for (size_t j = 0; j < 4; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((id & (1 << (i + j * 8))) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
}

void Serialize::write3Int(char * memblock, int & id)
{
	memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // id
	for (size_t j = 0; j < 3; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((id & (1 << (i + j * 8))) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
}

void Serialize::writeZone(char * memblock, int & zone)
{
	memblock[0] = 0;  // zone
	for (size_t i = 0; i < 8; ++i) {
		if ((zone & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
}

void Serialize::writeSpeed(char * memblock, int & speed)
{
	memblock[0] = 0;  // zone
	for (size_t i = 0; i < 8; ++i) {
		if ((speed & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
}

void Serialize::writeArticle(char * memblock, string & article)
{
	for (size_t i = 0; i < article.size() + 1; ++i) {
		memblock[i] = 0;
	}
	for (size_t i = 0; i < 8; ++i) {
		if ((article.size() & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
	for (size_t j = 1; j < article.size() + 1; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((article.at(j-1) & (1 << i)) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
}

void Serialize::writeDescription(char * memblock, string & description)
{
	for (size_t i = 0; i < description.size() + 1; ++i) {
		memblock[i] = 0;
	}
	for (size_t i = 0; i < 8; ++i) {
		if ((description.size() & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
	for (size_t j = 1; j < description.size() + 1; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((description.at(j - 1) & (1 << i)) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
}

void Serialize::writeTileBooleans(char * memblock, tile* & t)
{
	memblock[0] = 0;
	if (t->getBlockPathfind()) {
		memblock[0] |= (1u << 0);
	}
}

void Serialize::writeItemBooleans(char * memblock, Item* & i)
{
	memblock[0] = 0, memblock[1] = 0;
	if (i->getBlockProjectile()) {
		memblock[0] |= (1u << 0);
	}
	if (i->getBlockPathfind()) {
		memblock[0] |= (1u << 1);
	}
	if (i->getBlockObject()) {
		memblock[0] |= (1u << 2);
	}
	if (i->getMoveable()) {
		memblock[0] |= (1u << 3);
	}
	if (i->getPickupable()) {
		memblock[0] |= (1u << 4);
	}
	if (i->getUseable()) {
		memblock[0] |= (1u << 5);
	}
	if (i->getHangeable()) {
		memblock[0] |= (1u << 6);
	}
	if (i->getAlwaysOnTop()) {
		memblock[0] |= (1u << 7);
	}
	if (i->getAnimationState()) {
		memblock[1] |= (1u << 0);
	}

}

void Serialize::writeItems(ofstream & file, char * memblock, tile*& t)
{

	size_t size = t->getAllItems().size();
	// Write amount of items
	size_t amountToWrite = 1;
	memblock = new char[amountToWrite];
	memblock[0] = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((size & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
	file.write(memblock, amountToWrite);
	delete[] memblock;

	// Items
	for (auto& i : t->getAllItems()) {
		auto tempItem = itemAtlas.getItem(i->getID());
		// Item ID
		amountToWrite = 4;
		memblock = new char[amountToWrite];
		int id = i->getID();
		if (id < 0) id = 65000;
		writeID(memblock, id);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item UID
		amountToWrite = 4;
		memblock = new char[amountToWrite];
		int uID = i->getUID();
		writeID(memblock, uID);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item article
		string article = i->getArticle();
		if (article.compare(tempItem->getArticle()) == 0)
			article = "";
		amountToWrite = article.size() + 1;
		memblock = new char[amountToWrite];
		writeArticle(memblock, article);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item name
		string name = i->getName();
		if (name.compare(tempItem->getName()) == 0)
			name = "";
		amountToWrite = name.size() + 1;
		memblock = new char[amountToWrite];
		writeDescription(memblock, name);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item description
		string description = i->getDescription();
		if (description.compare(tempItem->getDescription()) == 0)
			description = "";
		amountToWrite = description.size() + 1;
		memblock = new char[amountToWrite];
		writeDescription(memblock, description);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item type
		string type = i->getType();
		amountToWrite = type.size() + 1;
		memblock = new char[amountToWrite];
		writeDescription(memblock, type);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item booleans
		amountToWrite = 2;
		memblock = new char[amountToWrite];
		writeItemBooleans(memblock, i);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		if (NonStaticItem* check = dynamic_cast<NonStaticItem*>(i)) {
			//  Weight
			string weight = std::to_string(check->getWeight());
			amountToWrite = weight.size() + 1;
			memblock = new char[amountToWrite];
			writeDescription(memblock, weight);
			file.write(memblock, amountToWrite);
			delete[] memblock;

			// Stack
			amountToWrite = 3;
			memblock = new char[amountToWrite];
			int stack = check->getStack();
			write3Int(memblock, stack);
			file.write(memblock, amountToWrite);
			delete[] memblock;
			
			// Max stack
			amountToWrite = 3;
			memblock = new char[amountToWrite];
			int maxStack = check->getStackMax();
			write3Int(memblock, maxStack);
			file.write(memblock, amountToWrite);
			delete[] memblock;
		}

		if (Weapon* check = dynamic_cast<Weapon*>(i)) {
			
			// Weapon attack
			amountToWrite = 3;
			memblock = new char[amountToWrite];
			int attack = check->getAttack();
			write3Int(memblock, attack);
			file.write(memblock, amountToWrite);
			delete[] memblock;
			
			// Weapon defence
			amountToWrite = 3;
			memblock = new char[amountToWrite];
			int defense = check->getDefense();
			write3Int(memblock, defense);
			file.write(memblock, amountToWrite);
			delete[] memblock;
			/*
			//  Weapon type
			string weaponType = check->getWeaponType();
			amountToWrite = weaponType.size() + 1;
			memblock = new char[amountToWrite];
			writeDescription(memblock, weaponType);
			file.write(memblock, amountToWrite);
			delete[] memblock;*/
		}

		if (Armor* check = dynamic_cast<Armor*>(i)) {

			// Weapon attack
			amountToWrite = 3;
			memblock = new char[amountToWrite];
			int armor = check->getArmor();
			write3Int(memblock, armor);
			file.write(memblock, amountToWrite);
			delete[] memblock;

			/*
			//  Weapon type
			string weaponType = check->getWeaponType();
			amountToWrite = weaponType.size() + 1;
			memblock = new char[amountToWrite];
			writeDescription(memblock, weaponType);
			file.write(memblock, amountToWrite);
			delete[] memblock;*/
		}
	}
}

void Serialize::writeTile(ofstream & file, char * memblock, tile*& t, streampos & size, size_t& pos)
{
	auto tempItem = itemAtlas.getItem(t->getID());
	// Tile coordinates
	size_t amountToWrite = 6;
	memblock = new char[amountToWrite];
	writeCords(memblock, t);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile ID
	amountToWrite = 4;
	memblock = new char[amountToWrite];
	int id = t->getID();
	if (id < 0) id = 65000;
	writeID(memblock, id);
	file.write(memblock, amountToWrite);
	delete[] memblock;

	// Tile UID
	amountToWrite = 4;
	memblock = new char[amountToWrite];
	int uid = t->getID();
	writeID(memblock, uid);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile zone
	amountToWrite = 1;
	memblock = new char[amountToWrite];
	int zone = t->getZone();
	writeZone(memblock, zone);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile speed
	amountToWrite = 1;
	memblock = new char[amountToWrite];
	int speed = t->getSpeed();
	writeSpeed(memblock, speed);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile article
	string article = t->getArticle();
	if (article.compare(tempItem->getArticle()) == 0)
		article = "";
	amountToWrite = article.size() + 1;
	memblock = new char[amountToWrite];
	writeArticle(memblock, article);
	file.write(memblock, amountToWrite);
	delete[] memblock;

	// Tile Name
	string name = t->getName();
	if (name.compare(tempItem->getName()) == 0)
		name = "";
	amountToWrite = name.size() + 1;
	memblock = new char[amountToWrite];
	writeArticle(memblock, name);
	file.write(memblock, amountToWrite);
	delete[] memblock;

	// Tile Description
	string description = t->getDescription();
	if (description.compare(tempItem->getDescription()) == 0)
		description = "";
	amountToWrite = description.size() + 1;
	memblock = new char[amountToWrite];
	writeArticle(memblock, description);
	file.write(memblock, amountToWrite);
	delete[] memblock;

	// Tile booleans
	amountToWrite = 1;
	memblock = new char[amountToWrite];
	writeTileBooleans(memblock, t);
	file.write(memblock, amountToWrite);
	delete[] memblock;

	// Tile items
	writeItems(file, memblock, t);
	
}

size_t Serialize::getAmountOfTiles(char * memblock)
{
	size_t value = 0;
	for (size_t j = 0; j < 2; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i + (j * 8));
			}
		}
	}
	//cout << "Amount of tiles: " << value << std::endl;
	return value;
}

tile Serialize::getTile(char * memblock, const size_t & pos)
{
	int x = 0, y = 0, z = 0, id = 0;
	getTileCords(memblock, x, y, z);
	getID(memblock, id);
	return tile(x, y, z, id);
}

void Serialize::getTileCords(char* memblock, int& x, int& y, int& z)
{
	size_t value = 0;
	for (size_t j = 0; j < 2; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i + (j * 8));
			}
		}
	}
	x = value;
	value = 0;
	for (size_t j = 2; j < 4; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i + ((j % 2) * 8));
			}
		}
	}
	y = value;
	value = 0;
	for (size_t j = 4; j < 6; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i + ((j % 2) * 8));
			}
		}
	}
	z = value;
	value = 0;
}

void Serialize::getID(char * memblock, int & id)
{
	//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
	int value = 0;
	for (size_t j = 0; j < 4; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i + (j * 8));
			}
		}
	}
	id = value;
}

void Serialize::get3Int(char * memblock, int & id)
{
	//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
	int value = 0;
	for (size_t j = 0; j < 3; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i + (j * 8));
			}
		}
	}
	id = value;
}
template <class T>
void Serialize::getNonStaticItemValues(ifstream & file, char * memblock,int& size, std::string weight, int& stack, int & stackMax, T& c) {

	// Get Weight
	getArticle(file, memblock, weight);
	c->setWeight(std::stof(weight));

	// Stack
	size = 3;
	memblock = new char[size];
	file.read(memblock, size);
	get3Int(memblock, stack);
	delete[] memblock;
	c->setStack(stack);

	// stack max
	size = 3;
	memblock = new char[size];
	file.read(memblock, size);
	get3Int(memblock, stackMax);
	delete[] memblock;
	c->setMaxStack(stackMax);
}

void Serialize::getZone(char * memblock, int & zone)
{
	int value = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((memblock[0] & (1 << i)) != 0) {
			value |= (1u << i);
		}
	}
	zone = value;
}

void Serialize::getSpeed(char * memblock, int & speed)
{
	int value = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((memblock[0] & (1 << i)) != 0) {
			value |= (1u << i);
		}
	}
	speed = value;
}

void Serialize::getArticle(ifstream & file, char * memblock, string & article)
{
	size_t size = 1;
	memblock = new char[size];
	file.read(memblock, size);
	size = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((memblock[0] & (1 << i)) != 0) {
			size |= (1u << i);
		}
	}
	delete[] memblock;
	memblock = new char[size];
	file.read(memblock, size);
	article = "";
	char value;
	for (size_t j = 0; j < size; ++j) {
		value = 0;
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i);
			}
		}
		article += value;
	}
	delete[] memblock;
}

void Serialize::getDescription(ifstream & file, char * memblock, string & description)
{
	size_t size = 1;
	memblock = new char[size];
	file.read(memblock, size);
	size = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((memblock[0] & (1 << i)) != 0) {
			size |= (1u << i);
		}
	}
	delete[] memblock;
	memblock = new char[size];
	file.read(memblock, size);
	description = "";
	char value;
	for (size_t j = 0; j < size; ++j) {
		value = 0;
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i);
			}
		}
		description += value;
	}
	delete[] memblock;
}

void Serialize::getBooleans(ifstream & file, char * memblock, bool & moveable)
{
	size_t size = 1;
	memblock = new char[size];
	file.read(memblock, size);
	moveable = false;
	if ((memblock[0] & (1 << 0)) != 0) {
		moveable |= (1u << 0);
	}
	delete[] memblock;
}

void Serialize::getTileItems(ifstream & file, char * memblock, vector<Item*>& items, bool section)
{
	items.clear();

	size_t size = 1;
	memblock = new char[size];
	file.read(memblock, size);
	size = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((memblock[0] & (1 << i)) != 0) {
			size |= (1u << i);
		}
	}
	delete[] memblock;

	Item* item(nullptr);
	int id(0), uID(0), attack, defense, armor;
	int stack, stackMax;
	string article, description, name, type, weaponType, weight;
	bool blockProjectile{ false }, blockPathfind{ false }, blockObject{ false }, moveable{ true }, pickupable{ false }, useable{ false }, hangeable{ false }, alwaysOnTop{ false }, animationOn{ false };


	for (size_t i = 0; i < size; ++i) {

		// Get ID
		int size = 4;
		memblock = new char[size];
		file.read(memblock, size);
		getID(memblock, id);
		delete[] memblock;
		if (id == 65000) 
			id = -1;
		Item* tempItem = nullptr;
		if (id != -1)	tempItem = itemAtlas.getItem(id);

		// Get uID
		size = 4;
		memblock = new char[size];
		file.read(memblock, size);
		getID(memblock, uID);
		delete[] memblock;
		if (uID > 6000)
			cout << "wut";

		// Get article
		getArticle(file, memblock, article);
		if (id >= 0 && article.size() == 0)
			article = tempItem->getArticle();

		// Get description
		getArticle(file, memblock, name);
		if (id >= 0 && name.size() == 0)
			name = tempItem->getName();

		// Get description
		getArticle(file, memblock, description);
		if (id >= 0 && description.size() == 0)
			description = tempItem->getDescription();

		// Get type
		getArticle(file, memblock, type);

		// Get Booleans
		getItemBooleans(file, memblock, blockProjectile, blockPathfind, blockObject, moveable, pickupable, useable, hangeable, alwaysOnTop, animationOn);

		if (section) {
			//Item* temp = itemAtlas.getItem(id);
			if (type.compare("Weapon") == 0) {
				Weapon* w = new Weapon(id, article, name);
				

				getNonStaticItemValues(file, memblock, size, weight, stack, stackMax, w);
				
				// Get attack
				size = 3;
				memblock = new char[size];
				file.read(memblock, size);
				get3Int(memblock, attack);
				delete[] memblock;
				w->setAttack(attack);
				
				// Get defense
				size = 3;
				memblock = new char[size];
				file.read(memblock, size);
				get3Int(memblock, defense);
				delete[] memblock;
				w->setDefense(defense);
				/*
				// Get description
				getArticle(file, memblock, weaponType);*/
				w->setWeaponType(itemAtlas.getWeaponType(id));

				item = w;
			}
			else if (type.compare("Armor") == 0) {
				Armor* a = new Armor(id, article, name);

				getNonStaticItemValues(file, memblock, size, weight, stack, stackMax, a);

				// Get attack
				size = 3;
				memblock = new char[size];
				file.read(memblock, size);
				get3Int(memblock, armor);
				delete[] memblock;
				a->setArmor(armor);

				item = a;
			}
			else if (type.compare("non") == 0) {
				NonStaticItem* n = new NonStaticItem(id, article, name);
				getNonStaticItemValues(file, memblock, size, weight, stack, stackMax, n);
				item = n;
			}else if(type.size() == 0) {
				item = new Item(id, article, name);
			}

			if (type.size() != 0) item->setType(type);
			if (description.size() != 0) item->setDescription(description);
			if (uID != 0) item->setUID(uID);
			if (blockProjectile != 0) item->setBlockProjectile(true);
			if (blockPathfind != 0) item->setBlockPathfind(true);
			if (blockObject != 0) item->setBlockObject(true);
			if (moveable != 0) item->setMoveable(true);
			if (pickupable != 0) item->setPickupable(true);
			if (useable != 0) item->setUseable(true);
			if (hangeable != 0) item->setHangeable(true);
			if (alwaysOnTop != 0) item->setAlwaysOnTop(true);
			if (animationOn) item->setAnimationState(true);

			items.push_back(item);
		}
	}
}

void Serialize::getItemBooleans(ifstream & file, char * memblock, bool & blockProjectile, bool & blockPathfind, bool & blockObject, bool & moveable, bool & pickupable, bool & useable, bool & hangeable, bool & alwaysOnTop, bool& animationOn)
{
	blockProjectile = false, blockPathfind = false, blockObject = false, moveable = true, pickupable = false, useable = false, hangeable = false, alwaysOnTop = false, animationOn = false;
	size_t size = 1;
	memblock = new char[size];
	file.read(memblock, size);

	if ((memblock[0] & (1 << 0)) != 0) {
		blockProjectile = true;
	}
	if ((memblock[0] & (1 << 1)) != 0) {
		blockPathfind = true;
	}
	if ((memblock[0] & (1 << 2)) != 0) {
		blockObject = true;
	}
	if ((memblock[0] & (1 << 3)) != 0) {
		moveable = true;
	}
	if ((memblock[0] & (1 << 4)) != 0) {
		pickupable = true;
	}
	if ((memblock[0] & (1 << 5)) != 0) {
		useable = true;
	}
	if ((memblock[0] & (1 << 6)) != 0) {
		hangeable = true;
	}
	if ((memblock[0] & (1 << 7)) != 0) {
		alwaysOnTop = true;
	}
	if ((memblock[0] & (1 << 8)) != 0) {
		animationOn = true;
	}

	delete[] memblock;

	size = 1;
	memblock = new char[size];
	file.read(memblock, size);

	if ((memblock[0] & (1 << 0)) != 0) {
		animationOn = true;
	}

	delete[] memblock;
}

void Serialize::sortTileVector(std::vector<tile>& tileVector) {
	std::sort(tileVector.begin(), tileVector.end(), [](tile& lhs, tile& rhs) {
		if (lhs.getX() < rhs.getX()) return true;
		if (rhs.getX() < lhs.getX()) return false;

		// a=b for primary condition, go to secondary
		if (lhs.getY() < rhs.getY()) return true;
		if (rhs.getY() < lhs.getY()) return false;

		return false;
	});
}
