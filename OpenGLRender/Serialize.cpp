#include "Serializer.h"

void Serialize::saveWorld(World& world)
{
	for (size_t f = 0; f < 14; ++f) {
		for (size_t s = 0; s < 100; ++s) {
			if (world.getFloor(f).getSection(s).size() != 0) {
				writeTilesToFile("./resources/map/" + std::to_string(f) + "/" + std::to_string(s) + ".bin", world.getFloor(f).getSection(s));
			}
		}
	}
}

void Serialize::loadWorld(World& world)
{
	for (size_t f = 0; f < 14; ++f) {
		for (size_t s = 0; s < 100; ++s) {
			loadSection(world.getFloor(f).getSection(s), f, s);
		}
	}
}

void Serialize::loadSection(vector<tile>& tiles, size_t floor, size_t section)
{
	tiles.clear();
	tiles.shrink_to_fit();
	getTilesFromFile(tiles, "./resources/map/" + std::to_string(floor) + "/" + std::to_string(section) + ".bin");
}


void Serialize::getTilesFromFile(vector<tile>& tiles, string fileName)
{
	size_t x, y, z, id, speed, zone;
	string article, description;
	bool blockPathfind;
	vector<Item> items;
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
			//delete[] memblock;
			
			// Get ID
			size = 3;
			memblock = new char[size];
			//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
			file.read(memblock, size);
			getID(memblock, id);
			//delete[] memblock;

			// Get zone
			size = 1;
			memblock = new char[size];
			//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
			file.read(memblock, size);
			getZone(memblock, zone);
			//delete[] memblock;

			// Get speed
			size = 1;
			memblock = new char[size];
			//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
			file.read(memblock, size);
			getSpeed(memblock, speed);
			//delete[] memblock;

			// Get article
			getArticle(file, memblock, article);

			// Get description
			getDescription(file, memblock, description);

			// Get booleans
			getBooleans(file, memblock, blockPathfind);

			// Get Items
			getTileItems(file, memblock, items);


			t = &tile(x, y, z, id);

			if (zone != 0) t->setZone(zone);
			if (speed != 100) t->setSpeed(speed);
			if (article.size() != 0) t->setArticle(article);
			if (description.size() != 0) t->setDescription(description);
			if (blockPathfind != 1) t->setBlockPathfind(blockPathfind);
			if (items.size() != 0) t->setItems(items);

			tiles.push_back(*t);
		}
		delete[] memblock;
		file.close();
		//sortTileVector(tiles);
	}
}

void Serialize::writeTilesToFile(string fileName, vector<tile>& tiles)
{
	streampos size;
	char * memblock;

	ofstream file(fileName, ios::out | ios::binary | std::ofstream::trunc);
	
	if (file.is_open())
	{
		size_t tileSize = 2;
		size_t pos = tileSize;
		memblock = new char[tileSize];
		writeAmount(memblock, tiles.size());
		file.seekp(0, ios::beg);
		file.write(memblock, tileSize);
		delete[] memblock;
		for (auto& t : tiles) {
			writeTile(file, memblock, t, size, pos);
		}
		file.close();
	}
}


void Serialize::writeAmount(char* memblock, int amount) {
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
			memblock[1] |= (1u << i - 8);
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
void Serialize::writeCords(char* memblock, tile& t) {
	memblock[0] = 0; memblock[1] = 0; // X
	memblock[2] = 0; memblock[3] = 0; // Y
	memblock[4] = 0; memblock[5] = 0; // Z
	int amount = 0, first = 0, second = 0;
	for (size_t j = 0; j < 6; j += 2) {
		switch (j)
		{
		case 0:
			amount = t.getX();
			first = 0;
			second = 1;
			break;
		case 2:
			amount = t.getY();
			first = 2;
			second = 3;
			break;
		case 4:
			amount = t.getZ();
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
				memblock[second] |= (1u << i - 8);
			}
		}
	}
}

void Serialize::writeID(char * memblock, size_t & id)
{
	memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // id
	for (size_t j = 0; j < 3; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((id & (1 << i + j * 8)) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
}

void Serialize::writeZone(char * memblock, size_t & zone)
{
	memblock[0] = 0;  // zone
	for (size_t i = 0; i < 8; ++i) {
		if ((zone & (1 << i)) != 0) {
			memblock[0] |= (1u << i);
		}
	}
}

void Serialize::writeSpeed(char * memblock, size_t & speed)
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

void Serialize::writeTileBooleans(char * memblock, tile & t)
{
	memblock[0] = 0;
	if (t.getBlockPathfind()) {
		memblock[0] |= (1u << 0);
	}
}

void Serialize::writeItemBooleans(char * memblock, Item & i)
{
	memblock[0] = 0;
	if (i.getBlockProjectile()) {
		memblock[0] |= (1u << 0);
	}
	if (i.getBlockPathfind()) {
		memblock[0] |= (1u << 1);
	}
	if (i.getBlockObject()) {
		memblock[0] |= (1u << 2);
	}
	if (i.getMoveable()) {
		memblock[0] |= (1u << 3);
	}
	if (i.getPickupable()) {
		memblock[0] |= (1u << 4);
	}
	if (i.getUseable()) {
		memblock[0] |= (1u << 5);
	}
	if (i.getHangeable()) {
		memblock[0] |= (1u << 6);
	}
	if (i.getAlwaysOnTop()) {
		memblock[0] |= (1u << 7);
	}
}

void Serialize::writeItems(ofstream & file, char * memblock, tile & t)
{
	size_t size = t.getAllItems().size();
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
	for (auto& i : t.getAllItems()) {
		// Item ID
		amountToWrite = 3;
		memblock = new char[amountToWrite];
		size_t id = i.getID();
		writeID(memblock, id);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item UID
		amountToWrite = 3;
		memblock = new char[amountToWrite];
		size_t uID = i.getUID();
		writeID(memblock, uID);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item article
		string article = i.getArticle();
		amountToWrite = article.size() + 1;
		memblock = new char[amountToWrite];
		writeArticle(memblock, article);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item description
		string description = i.getDescription();
		amountToWrite = description.size() + 1;
		memblock = new char[amountToWrite];
		writeDescription(memblock, description);
		file.write(memblock, amountToWrite);
		delete[] memblock;

		// Item booleans
		amountToWrite = 1;
		memblock = new char[amountToWrite];
		writeItemBooleans(memblock, i);
		file.write(memblock, amountToWrite);
		delete[] memblock;
	}
}

void Serialize::writeTile(ofstream & file, char * memblock, tile & t, streampos & size, size_t& pos)
{

	// Tile coordinates
	size_t amountToWrite = 6;
	memblock = new char[amountToWrite];
	writeCords(memblock, t);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile ID
	amountToWrite = 3;
	memblock = new char[amountToWrite];
	size_t id = t.getID();
	writeID(memblock, id);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile zone
	amountToWrite = 1;
	memblock = new char[amountToWrite];
	size_t zone = t.getZone();
	writeZone(memblock, zone);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile speed
	amountToWrite = 1;
	memblock = new char[amountToWrite];
	size_t speed = t.getSpeed();
	writeSpeed(memblock, speed);
	file.write(memblock, amountToWrite);
	delete[] memblock;
	
	// Tile article
	string article = t.getArticle();
	amountToWrite = article.size() + 1;
	memblock = new char[amountToWrite];
	writeArticle(memblock, article);
	file.write(memblock, amountToWrite);
	delete[] memblock;

	// Tile description
	string description = t.getDescription();
	amountToWrite = description.size() + 1;
	memblock = new char[amountToWrite];
	writeDescription(memblock, description);
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
	cout << "Amount of tiles: " << value << std::endl;
	return value;
}

tile Serialize::getTile(char * memblock, const size_t & pos)
{
	size_t x = 0, y = 0, z = 0, id = 0;
	getTileCords(memblock, x, y, z);
	getID(memblock, id);
	return tile(x, y, z, id);
}

void Serialize::getTileCords(char* memblock, size_t& x, size_t& y, size_t& z)
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

void Serialize::getID(char * memblock, size_t & id)
{
	//memblock[0] = 0; memblock[1] = 0; memblock[2] = 0; // ID
	size_t value = 0;
	for (size_t j = 0; j < 3; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				value |= (1u << i + (j * 8));
			}
		}
	}
	id = value;
}

void Serialize::getZone(char * memblock, size_t & zone)
{
	size_t value = 0;
	for (size_t i = 0; i < 8; ++i) {
		if ((memblock[0] & (1 << i)) != 0) {
			value |= (1u << i);
		}
	}
	zone = value;
}

void Serialize::getSpeed(char * memblock, size_t & speed)
{
	size_t value = 0;
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

void Serialize::getTileItems(ifstream & file, char * memblock, vector<Item>& items)
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
	size_t id, uID;
	string article, description;
	bool blockProjectile{ false }, blockPathfind{ false }, blockObject{ false }, moveable{ true }, pickupable{ false }, useable{ false }, hangeable{ false }, alwaysOnTop{ false };


	for (size_t i = 0; i < size; ++i) {

		// Get ID
		size_t size = 3;
		memblock = new char[size];
		file.read(memblock, size);
		getID(memblock, id);
		delete[] memblock;

		// Get uID
		size = 3;
		memblock = new char[size];
		file.read(memblock, size);
		getID(memblock, uID);
		delete[] memblock;

		// Get article
		getArticle(file, memblock, article);

		// Get description
		getArticle(file, memblock, description);

		// Get Booleans
		getItemBooleans(file, memblock, blockProjectile, blockPathfind, blockObject, moveable, pickupable, useable, hangeable, alwaysOnTop);

		item = &Item(id, article, description);
		if( uID != 0) item->setUID(uID);
		if (blockProjectile != 0) item->setBlockProjectile(true);
		if (blockPathfind != 0) item->setBlockPathfind(true);
		if (blockObject != 0) item->setBlockObject(true);
		if (moveable != 0) item->setMoveable(true);
		if (pickupable != 0) item->setPickupable(true);
		if (useable != 0) item->setUseable(true);
		if (hangeable != 0) item->setHangeable(true);
		if (alwaysOnTop != 0) item->setAlwaysOnTop(true);

		items.push_back(*item);
	}
}

void Serialize::getItemBooleans(ifstream & file, char * memblock, bool & blockProjectile, bool & blockPathfind, bool & blockObject, bool & moveable, bool & pickupable, bool & useable, bool & hangeable, bool & alwaysOnTop)
{
	blockProjectile = false, blockPathfind = false, blockObject = false, moveable = true, pickupable = false, useable = false, hangeable = false, alwaysOnTop = false;
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
