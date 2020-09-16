#include "Serializer.h"
#include "Global.h"
#include <filesystem>

void Serialize::saveWorld(World* world)
{
	writeWorldToFile("./resources/map/" + world->getName() + ".bin", world);
}

void Serialize::loadWorld(std::string worldName)
{
	int header_Size = 0;

	std::string name = "", path = "./resources/map/" + worldName + ".bin";
	int floors = 0, width = 0, height = 0, width_Tiles = 0;
	char tempValueHolder = 0;

	char* memblock{};
	ifstream* file = new ifstream(path, ios::in | ios::binary | ios::ate);
	if (file->is_open())
	{
		file->seekg(0, ios::beg);
		__int64 then = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		getHeaderInfo(file, memblock, tempValueHolder, name, floors, width, height, width_Tiles, header_Size);

		if (worldLoadTemp) {
			delete worldLoadTemp;
		}
		worldLoadTemp = new World(name, floors, width, height, width_Tiles);

		getWorldData(path, file, memblock, worldLoadTemp, header_Size);
		__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("The time to load the world took: %i ms\n",  int(now-then));
		file->close();
	}

	delete file;

	updateWorld = true;
}

void Serialize::getChar(ifstream* file, char* memblock, char& character)
{
	memblock = new char[1];
	file->read(memblock, 1);
	character = 0;
	for (int j = 0; j < 1; j++) {
		for (int i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				character |= (1u << (i + (j * 8)));
			}
		}
	}
	delete[] memblock;
}

void Serialize::getInt(ifstream* file, char* memblock, int& integer)
{
	memblock = new char[4];
	file->read(memblock, 4);
	integer = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				integer |= (1u << (i + (j*8)));
			}
		}
	}
	delete[] memblock;
}

void Serialize::get_Two_Values_From_Int(ifstream* file, char* memblock, int& integer1, int& integer2)
{
	memblock = new char[4];
	file->read(memblock, 4);
	integer1 = 0; integer2 = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 8; ++i) {
			if (j < 2 && (memblock[j] & (1 << i)) != 0) {
				integer1 |= (1u << (i + (j * 8)));
			}
			if (j > 1 && (memblock[j] & (1 << i)) != 0) {
				integer2 |= (1u << (i + ((j - 2) * 8)));
			}

		}
	}
	delete[] memblock;
}

void Serialize::get_Two_Values_From_Int(ifstream* file, char* memblock, char& character, int& integer)
{
	memblock = new char[4];
	file->read(memblock, 4);
	character = 0; integer = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 8; ++i) {
			if (j < 1 && (memblock[j] & (1 << i)) != 0) {
				character |= (1u << (i + (j * 8)));
			}
			if (j > 0 && (memblock[j] & (1 << i)) != 0) {
				integer |= (1u << (i + ((j - 1) * 8)));
			}

		}
	}
	if (integer == 16777215)
		integer = -1;
	delete[] memblock;
}

void Serialize::getString(ifstream* file, char* memblock, char& tempValueHolder, std::string& str, int from, int size)
{
	memblock = new char[size];
	file->read(memblock, size);
	str = "";
	for (size_t j = from; j < from+size; ++j) {
		tempValueHolder = 0;
		for (size_t i = 0; i < 8; ++i) {
			if ((memblock[j] & (1 << i)) != 0) {
				tempValueHolder |= (1u << i);
			}
		}
		if (tempValueHolder != 0)
			str += tempValueHolder;
	}
	delete[] memblock;
}

void Serialize::getNextLabel(ifstream* file, char* memblock, char& tempValueHolder, std::string& label, int& size)
{
	size = 0;
	label = "";
	getString(file, memblock, tempValueHolder, label, 0, 4);
	getInt(file, memblock, size);
}

void Serialize::getHeaderInfo(ifstream* file, char* memblock, char& tempValueHolder, std::string& name, int& floors, int& width, int& height, int& width_Tiles, int& header_Size)
{
	int size = 0;
	std::string label = "";
	getNextLabel(file, memblock, tempValueHolder, label, size);
	header_Size += size + 8;
	if (label == "HDR") {
		while(label != "END") {
			getNextLabel(file, memblock, tempValueHolder, label, size);
			if (label == "NAME") {
				getString(file, memblock, tempValueHolder, name, 0, size);
			}
			else if (label == "AFLR") {
				getInt(file, memblock, floors);
			}
			else if (label == "WIHI") {
				getInt(file, memblock, width);
				getInt(file, memblock, height);
			}
			else if (label == "WIST") {
				getInt(file, memblock, width_Tiles);
			}
		}
	}
}

void Serialize::getWorldData(std::string& filePath, ifstream* file, char* memblock, World* world, int& header_Size)
{
	int size = 0, world_Size = 0, floor_From = 0, floor_To = world->get_Floor_Amount() - 1;
	char tempValueHolder = 0;
	std::string label = "";
	while (label != "END") {
		getNextLabel(file, memblock, tempValueHolder, label, size);
		if (label == "WRLD") {
			world_Size = size;
			
			floor_Thread_Handler(file, filePath, world, header_Size);
		
			break;
		}
	}
}

void Serialize::floor_Thread_Handler(ifstream* file, std::string& path, World* world, const int& header_Size)
{
	int floor_From = world->get_Floor_Amount() - 1, floor_To = 0;
	std::vector<std::thread> threads;
	std::vector<ifstream*> files;

	auto sections = get_Currently_Loaded_Sections();
	
	for (int floor = floor_From; floor > 1; floor--) {
		files.push_back(new ifstream(path, ios::in | ios::binary | ios::ate));
		files[floor_From-floor]->seekg(header_Size + 8, ios::beg);
		threads.push_back(std::thread(&Serialize::getLevel, this, files[floor_From - floor], std::ref(world), floor, std::ref(sections)));
	}
	
	getLevel(file, world, 0, sections);
	for (auto& thread : threads) {
		thread.join();
	}
	for (auto& file : files) {
		file->close();
		delete file;
	}
	files.clear();
}


void Serialize::getLevel(ifstream* file, World* world, const int floor_Target, std::unordered_map<int, int>& sections)
{
	char* memblock = nullptr;
	std::string label = "";
	int size = 0, floor_Level = 0, floor_Next_Pos = 0, floor_Current_Size = 0, floor_Current_Size_Total = 0, section_Data_Total = 0, section = 0;
	char tempValueHolder = 0;
	while (floor_Level != floor_Target || label != "END" || floor_Current_Size < floor_Current_Size_Total) {
		getNextLabel(file, memblock, tempValueHolder, label, size);
		if(label != "FLR")
			floor_Current_Size += size + 8;
		if (label == "FLR") {
			floor_Next_Pos = size;
			floor_Current_Size_Total = size;

			getNextLabel(file, memblock, tempValueHolder, label, size);
			getInt(file, memblock, floor_Level);
			floor_Next_Pos -= (size + 8);
			floor_Current_Size += size + 16;

			if (floor_Level != floor_Target) {
				file->seekg(floor_Next_Pos, ios::cur);
				floor_Current_Size = 0;
				floor_Next_Pos = 0;
			}
		}
		else if (label == "SCT") {
			section_Data_Total = size;
			getNextLabel(file, memblock, tempValueHolder, label, size);
			getInt(file, memblock, section);
			section_Data_Total -= (size + 8);
			if (sections.contains(section))
				getSection(file, memblock, tempValueHolder, world, size, label, floor_Level, section, section_Data_Total);
			else
				file->seekg(section_Data_Total, ios::cur);
		}
	}
}

void Serialize::getSection(ifstream* file, char* memblock, char& tempValueHolder, World* world, int& size, std::string& label, const int& floor, const int& section, int& section_Data_Total)
{
	int section_Data = 0;
	std::vector<tile*> tiles;
	while (label != "END" || section_Data < section_Data_Total) {
		getNextLabel(file, memblock, tempValueHolder, label, size);
		section_Data += size + 8;
		if (label == "TILE") {
			getTile(file, memblock, tempValueHolder, world, size, label, tiles);
		}
		else if (label != "END") {
			file->seekg(size, ios::cur);
		}
		else {
			if (tiles.size() > 0) {
				world->getFloor(floor)->add_Section(new Section(section, floor, tiles), section);
			}
		}
	}
}

void Serialize::getTile(ifstream* file, char* memblock, char& tempValueHolder, World* world, int& size, std::string& label, std::vector<tile*>& section)
{
	int x = 0, y = 0, ID = 0, UID = 0, tile_Data_Total = size, tile_Data = 0;
	char z = 0, zone = 0, speed = 0, blockPathfinding = 0;
	std::string name = "", article = "", description = "";
	std::vector<Item*> items;
	while (label != "END" || tile_Data < tile_Data_Total) {
		getNextLabel(file, memblock, tempValueHolder, label, size);
		tile_Data += size + 8;
		//current_Pos += 8;
		if (label == "COOR") {
			get_Two_Values_From_Int(file, memblock, x, y);
			get_Two_Values_From_Int(file, memblock, z, ID);
		}
		else if (label == "UID") {
			getInt(file, memblock, UID);
		}
		else if (label == "ZONE") {
			getChar(file, memblock, zone);
		}
		else if (label == "Speed") {
			getChar(file, memblock, speed);
		}
		else if (label == "NAME") {
			getString(file, memblock, tempValueHolder, name, 0, size);
		}
		else if (label == "ARTL") {
			getString(file, memblock, tempValueHolder, article, 0, size);
		}
		else if (label == "DESC") {
			getString(file, memblock, tempValueHolder, description, 0, size);
		}
		else if (label == "BPTH") {
			getChar(file, memblock, blockPathfinding);
		}
		else if (label == "ITEM") {
			getItems(file, memblock, tempValueHolder, world, size, label , items);
		}
		else if(label != "END") {
			file->seekg(size, ios::cur);
		}
		else {
			auto t = new tile(x, y, z, ID);
			if (UID != 0)
				t->setUID(UID);
			if (zone != 0)
				t->setZone(zone);
			if (speed != 100)
				t->setZone(speed);
			if (name != "")
				t->setName(name);
			if (article != "")
				t->setArticle(article);
			if (description != "")
				t->setDescription(description);
			if(blockPathfinding)
				t->setBlockPathfind(blockPathfinding);
			if (items.size() > 0)
				t->setItems(items);
			section.push_back(t);
		}
	}
}

void Serialize::getItems(ifstream* file, char* memblock, char& tempValueHolder, World* world, int& size, std::string& label, std::vector<Item*>& items)
{
	int ID = 0, UID = 0;
	char blockProjectile = false, blockPathfind = false, blockObject = false, moveable = false, pickupable = false, useable = false, hangeable = false, alwaysOnTop = false, animationOnly = false, doubleSize = false;
	std::string name = "", article = "", description = "", type = "";
	while (label != "END") {
		getNextLabel(file, memblock, tempValueHolder, label, size);
		//current_Pos += 8;
		if (label == "ID") {
			getInt(file, memblock, ID);
		}
		else if (label == "UID") {
			getInt(file, memblock, UID);
		}
		else if (label == "NAME") {
			getString(file, memblock, tempValueHolder, name, 0, size);
		}
		else if (label == "ARTL") {
			getString(file, memblock, tempValueHolder, article, 0, size);
		}
		else if (label == "DESC") {
			getString(file, memblock, tempValueHolder, description, 0, size);
		}
		else if (label == "TYPE") {
			getString(file, memblock, tempValueHolder, type, 0, size);
		}
		else if (label == "BOOL") {
			getChar(file, memblock, doubleSize);
			getChar(file, memblock, blockProjectile);
			getChar(file, memblock, blockPathfind);
			getChar(file, memblock, blockObject);
			getChar(file, memblock, moveable);
			getChar(file, memblock, pickupable);
			getChar(file, memblock, useable);
			getChar(file, memblock, hangeable);
			getChar(file, memblock, alwaysOnTop);
			getChar(file, memblock, animationOnly);
		}
		else if(label != "END") {
			file->seekg(size, ios::cur);
		}
		else {
			auto i = new Item(itemAtlas.getItem(ID));

			if (UID != 0)
				i->setUID(UID);
			if (name != "")
				i->setName(name);
			if (article != "")
				i->setArticle(article);
			if (description != "")
				i->setDescription(description);

				// Booleans
				i->setBlockProjectile(blockProjectile);
				i->setBlockPathfind(blockPathfind);
				i->setBlockObject(blockObject);
				i->setMoveable(moveable);
				i->setPickupable(pickupable);
				i->setUseable(useable);
				i->setHangeable(hangeable);
				i->setAlwaysOnTop(alwaysOnTop);
				i->setAnimationOnly(animationOnly);
				i->setDoubleSize(doubleSize);

				items.push_back(i);

		}
	}
}

/*
* The data is saved in the following format:
*	[ label 4 bytes][ size 4 bytes ][ [ label 4 bytes][ size 4 bytes ][ data ][ label 4 bytes][ size 4 bytes ][ data ] ] -> [ label 4 bytes][ size 4 bytes ][ data ] etc
*
*
* Labels:
* HDR(Header) = Header of the map containing information on the map. 
* END = End of a section (example at the end of the header section).
* NAME = Name of the map.
* AFLR(Amount FLooR) = Amount of floors on the map.
* WIHI(WIdth HIgth) = Width and height of the map;
* WIST(WIdth Section Tiles) = The amount of tiles of the section square (exampel 20*20)
* WRLD(WoRLD) = the amount of bytes in the world section.
* FLRN(FLooR Number) = The number of the current floor;
* SCT(SeCTion) = A section of a certain floor.
* SCTN(SeCTion Number) = A section's number.
* TILE = A tile.
* COOR(COORdinate) = The tile's coordinates.
* ID
* UID = Unique ID.
* ZONE = The zone of the tile.
* SPD(SPeeD) = The speed of the tile
* NAME
* ARTL(ARTicLe)
* DESC(DESCription)
* BPTH(Block PaTHfinding)
* BOOL(BOOLean) = The various booleans of an item(Note the are all store in one of these sections and not one for each).
*
*
*/
void Serialize::writeWorldToFile(string filePath, World* world)
{
	long totalSize = 0;
	int headerSize = 0, floorSize = 0;
	streampos size;
	char * memblock{};

	bool exists = std::filesystem::exists(filePath);

	
	
	if (!exists) {
		std::ofstream* file = new ofstream(filePath, std::ios::binary);
		file->seekp(0, ios::beg);

		writeWorldInfo(file, memblock, world, headerSize);

		writeWorldData(file, world, headerSize);

		file->close();
		delete file;
	}
	write_Floor_Thread_Handler(filePath, world);
	/*
	std::ifstream src = std::ifstream("./resources/map/tempMap.bin", std::ios::binary);
	std::ofstream  dst(filePath, std::ios::binary);

	dst << src.rdbuf();
	src.close();

	if (remove("./resources/map/tempMap.bin") != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");
		*/

}

void Serialize::write_Floor_Thread_Handler(const string& filePath, World* world)
{
	ofstream* fileOut = new ofstream(filePath, std::ios::binary | fstream::out | fstream::in);
	fileOut->seekp(0, ios::beg);
	ifstream* fileIn = new ifstream(filePath, ios::in | ios::binary | ios::ate);
	fileIn->seekg(0, ios::beg);
	int pos = 0, headerPos = 0, size = 0, size_Temp = 0, floor_Level = 0, floor_Target = 0, floor_Current_Size_Total = 0, floor_Current_Size = 0, floor_Next_Pos = 0, section_Data_Total = 0, section = 0, sectionTarget;
	long floorStartPos = 0;
	char tempValueHolder = 0;
	std::string label = "";
	char* memblock = nullptr;
	getNextLabel(fileIn, memblock, tempValueHolder, label, size);
	headerPos = size + 8;
	pos += headerPos;
	fileIn->seekg(size, ios::cur);
	while (label != "END") {
		getNextLabel(fileIn, memblock, tempValueHolder, label, size);
		pos += size + 8;
		if (label == "WRLD") {
			while (floor_Level != world->get_Floor_Amount()-1 || (label != "END" && floor_Current_Size <= floor_Current_Size_Total)) {
				getNextLabel(fileIn, memblock, tempValueHolder, label, size);
				if (label != "FLR" && label != "END")
					floor_Current_Size += size + 8;
				if (label == "END") {
					auto check = world->check_If_Updated_Sections(floor_Level);
					if (check.size() > 0) {
						fileIn->seekg(-8, ios::cur);
						for (auto section : check) {
							auto sectionPtr = world->getFloor(floor_Level)->get_Section(section.first);
							insert_Section(filePath, fileIn, fileOut, world, sectionPtr, floorStartPos);
							sectionPtr->set_Changed(false);
						}
						fileIn->seekg(floorStartPos, ios::beg);
					}
				}
				else if (label == "FLR") {
					floorStartPos = fileIn->tellg(); floorStartPos -= 8;
					floor_Next_Pos = size;
					floor_Current_Size_Total = size;
					floor_Current_Size = 0;

					getNextLabel(fileIn, memblock, tempValueHolder, label, size);
					getInt(fileIn, memblock, floor_Level);
					floor_Next_Pos -= (size + 8);
					floor_Current_Size += size + 16;

					
					if (floor_Current_Size == floor_Current_Size_Total) {
						auto check = world->check_If_Updated_Sections(floor_Level);
						if (check.size() > 0) {
							for (auto section : check) {
								auto sectionPtr = world->getFloor(floor_Level)->get_Section(section.first);
								insert_Section(filePath, fileIn, fileOut, world, sectionPtr, floorStartPos);
								sectionPtr->set_Changed(false);
							}
							fileIn->seekg(floorStartPos, ios::beg);
						}else if (floor_Level != world->get_Floor_Amount() - 1) {
							fileIn->seekg(floor_Next_Pos, ios::cur);
							floor_Current_Size = 0;
							floor_Next_Pos = 0;
						}
					}
					
				}
				else if (label == "SCT") {
					section_Data_Total = size;
					getNextLabel(fileIn, memblock, tempValueHolder, label, size); size_Temp = size + 16;
					getInt(fileIn, memblock, section);
					section_Data_Total -= (size + 8);
					auto sectionPtr = world->getFloor(floor_Level)->get_Section(section);
					if (sectionPtr && sectionPtr->get_Changed()) {
						
						section_Data_Total += (size + 8);
						floor_Current_Size -= section_Data_Total;
						overwrite_Section(filePath, fileIn, fileOut, world, sectionPtr, section_Data_Total, size_Temp, floorStartPos);
						floor_Current_Size += section_Data_Total;
						sectionPtr->set_Changed(false);
						
					}
					else
						fileIn->seekg(section_Data_Total, ios::cur);
				}
			}
		}
	}
	delete fileIn;
	delete fileOut;
}

void Serialize::overwrite_Section(const string& filePath, ifstream*& fIn, ofstream*& fOut, World* world, Section* section, int& length, const int after_Start_Pos_Already_Moved, const int floorStartPos)
{
	int sectionSize = 0, pos = 0, floorSize = 0; char tempValueHolder = 0; std::string label = "";
	char* memblock = nullptr, *memblock2 = nullptr;

	// Get position at start of section
	long start_In_Pos = fIn->tellg(); start_In_Pos += -after_Start_Pos_Already_Moved;
	// Get position at end of section
	long start_After_Pos = start_In_Pos; start_After_Pos += length + 8;
	// Get length of rest of the file
	fIn->seekg(0, ios::end);
	long restOfFile = fIn->tellg(); 
	restOfFile -= start_After_Pos;
	// Read the rest of the file
	memblock = new char[restOfFile];
	fIn->seekg(start_After_Pos, ios::beg);
	fIn->read(memblock, restOfFile);

	fOut->seekp(start_In_Pos, ios::beg);
	writeSection(fOut, memblock2, world, sectionSize, section->get_ID(), section->get_Tiles());
	delete[] memblock2;
	long writeRestPos = fOut->tellp();

	fOut->seekp(floorStartPos + 4, ios::beg);
	fIn->seekg(floorStartPos, ios::beg);
	getNextLabel(fIn, memblock, tempValueHolder, label, floorSize);
	memblock2 = new char[4];
	writeInt(memblock2, floorSize + (sectionSize - length - 8), pos);
	fOut->write(memblock2, 4);
	delete[] memblock2;

	fOut->seekp(writeRestPos, ios::beg);
	fOut->write(memblock, restOfFile);
	delete[] memblock;
	
	long removeAfter = fOut->tellp();
	fOut->seekp(0, ios::end);
	long totalSize = fOut->tellp();
	if (removeAfter != totalSize) {
		fIn->seekg(0, ios::beg);
		memblock = new char[removeAfter];
		fIn->read(memblock, removeAfter);
		delete fOut;
		fOut = new ofstream(filePath, std::ios::binary | fstream::trunc);
		delete fOut;
		fOut = new ofstream(filePath, std::ios::binary | fstream::app);
		fOut->seekp(0, ios::beg);
		fOut->write(memblock, removeAfter);
		delete[] memblock;
	}
	
	delete fIn;
	delete fOut;
	fOut = new ofstream(filePath, std::ios::binary | fstream::out | fstream::in);
	fOut->seekp(floorStartPos, ios::beg);
	fIn = new ifstream(filePath, ios::in | ios::binary | ios::ate);
	fIn->seekg(floorStartPos, ios::beg);
	length = sectionSize;
}

void Serialize::insert_Section(const string& filePath, ifstream*& fIn, ofstream*& fOut, World* world, Section* section, const int floorStartPos)
{
	int sectionSize = 0, pos = 0, floorSize = 0; char tempValueHolder = 0; std::string label = "";
	char* memblock = nullptr, *memblock2 = nullptr;

	// Get the position of where to insert the section
	long startWritePos = fIn->tellg();
	// Get length of rest of the file
	fIn->seekg(0, ios::end);
	long restOfFile = fIn->tellg();
	restOfFile -= startWritePos;
	// Read the rest of the file
	memblock = new char[restOfFile];
	fIn->seekg(startWritePos, ios::beg);
	fIn->read(memblock, restOfFile);

	fOut->seekp(startWritePos, ios::beg);
	writeSection(fOut, memblock2, world, sectionSize, section->get_ID(), section->get_Tiles());
	delete[] memblock2;
	startWritePos = fOut->tellp();

	fOut->seekp(floorStartPos + 4, ios::beg);
	fIn->seekg(floorStartPos, ios::beg);
	getNextLabel(fIn, memblock, tempValueHolder, label, floorSize);
	memblock2 = new char[4];
	writeInt(memblock2, floorSize + sectionSize, pos);
	fOut->write(memblock2, 4);
	delete[] memblock2;

	fOut->seekp(startWritePos, ios::beg);
	fOut->write(memblock, restOfFile);
	delete[] memblock;

	delete fIn;
	delete fOut;
	fOut = new ofstream(filePath, std::ios::binary | fstream::out | fstream::in);
	fOut->seekp(startWritePos, ios::beg);
	fIn = new ifstream(filePath, ios::in | ios::binary | ios::ate);
	fIn->seekg(startWritePos, ios::beg);
}

void Serialize::writeWorldInfo(ofstream* f, char* memblock, World* world, int& headerSize)
{
	int pos = 0, current_Size = 0;
	memblock = new char[8];
	writeLabelAndSize(memblock, "HDR", 0, pos);
	f->write(memblock, 8);
	delete[] memblock;

	int size = 8 + world->getName().size(); pos = 0;
	memblock = new char[size]; current_Size += size;
	writeLabelAndSize(memblock, "NAME", size - 8, pos);
	writeString(memblock, world->getName(), pos);
	f->write(memblock, size);
	delete[] memblock;
	
	size = 8 + 4; pos = 0;
	memblock = new char[size]; current_Size += size;
	writeLabelAndSize(memblock, "AFLR", size - 8, pos);
	writeInt(memblock, world->get_Floor_Amount(), pos);
	f->write(memblock, size);
	delete[] memblock;
	
	size = 8 + 4 + 4; pos = 0;
	memblock = new char[size]; current_Size += size;
	writeLabelAndSize(memblock, "WIHI", size - 8, pos);
	writeInt(memblock, world->get_Width_Section(), pos);
	writeInt(memblock, world->get_Height_Section(), pos);
	f->write(memblock, size);
	delete[] memblock;

	size = 8 + 4; pos = 0;
	memblock = new char[size]; current_Size += size;
	writeLabelAndSize(memblock, "WIST", size - 8, pos);
	writeInt(memblock, world->get_Width_Section_Tiles(), pos);
	f->write(memblock, size);
	delete[] memblock;

	size = 8; pos = 0;
	memblock = new char[size]; current_Size += size;
	writeLabelAndSize(memblock, "END", 0, pos);
	f->write(memblock, size);
	delete[] memblock;
	
	// Fill in the size of data for the HDR section
	size = 4; pos = 0;
	f->seekp(4, ios::beg);
	memblock = new char[size];
	for (int i = 0; i < size; i++)
		memblock[i] = 0;
	writeInt(memblock, current_Size, pos);
	f->write(memblock, size);
	delete[] memblock;
	f->seekp(0, ios::end);
	
	headerSize += current_Size + 8;
}

void Serialize::writeWorldData(ofstream* f, World* world, const int& startPos)
{
	char* memblock = nullptr;
	int size_World_Total = 0, size_Floor_So_Far = 0, size_Floor_Current = 0, size_Section_So_Far = 0, size_Section_Current = 0, temp = 0;
	int pos = 0, size = 0, section = 0, section_Max = world->get_Section_Max();

	size = 8; pos = 0;
	memblock = new char[8];
	writeLabelAndSize(memblock, "WRLD", -1, pos);
	f->write(memblock, 8);
	delete[] memblock;

	writeFloor(f, world, 0, 14, size_World_Total);

	size = 8;
	memblock = new char[size]; pos = 0; size_World_Total += size;
	writeLabelAndSize(memblock, "END", 0, pos);
	f->write(memblock, size);
	delete[] memblock;
	

	// Fill in the size of data for the WRLD section
	size = 4; pos = 0;
	temp = startPos + 4;
	f->seekp(temp, ios::beg);
	memblock = new char[size]; 
	for (int i = 0; i < 4; i++)
		memblock[i] = 0;
	writeInt(memblock, size_World_Total, pos);
	f->write(memblock, size);
	delete[] memblock;
	f->seekp(temp, ios::cur);
	
}

void Serialize::writeFloor(ofstream* f, World* world, int from, int to, int& size_World_Total)
{
	char* memblock = nullptr;
	int size = 8, pos = 0, size_Floor_Current = 0, section = 0, section_Max = world->get_Section_Max(), temp = 0;
	for (int floor = from; floor < to; floor++) {
		size = 8; pos = 0; size_Floor_Current = 0;
		memblock = new char[8];
		writeLabelAndSize(memblock, "FLR", -1, pos);
		f->write(memblock, 8);
		delete[] memblock;

		size = 8 + 4; pos = 0;
		memblock = new char[size]; size_Floor_Current += size;
		writeLabelAndSize(memblock, "FLRN", 4, pos);
		writeInt(memblock, floor, pos);
		f->write(memblock, size);
		delete[] memblock;


		for (auto section : world->getFloor(floor)->get_Sections()) {
			if (section->get_Changed()) {
				writeSection(f, memblock, world, size_Floor_Current, section->get_ID(), section->get_Tiles());
				section->set_Changed(false);
			}
		}

		size = 8;
		memblock = new char[size]; pos = 0; size_Floor_Current += size;
		writeLabelAndSize(memblock, "END", 0, pos);
		f->write(memblock, size);
		delete[] memblock;

		// Fill in the size of data for the FLR section
		temp = size_Floor_Current + 4;
		f->seekp(-temp, ios::cur);
		memblock = new char[4]; pos = 0;
		for (int i = 0; i < 4; i++)
			memblock[i] = 0;
		writeInt(memblock, size_Floor_Current, pos);
		f->write(memblock, 4);
		delete[] memblock;
		f->seekp(size_Floor_Current, ios::cur);

		size_World_Total += size_Floor_Current + 8;
	}
}

void Serialize::writeSection(ofstream* f, char* memblock, World* world, int& size_Floor_Current, const int section, std::vector<tile*>& list)
{
	int size = 8, pos = 0, temp = 0, size_Section_Current = 0;
	memblock = new char[size]; 
	writeLabelAndSize(memblock, "SCT", -1, pos);
	f->write(memblock, size);
	delete[] memblock;

	size = 8 + 4;
	memblock = new char[size]; pos = 0; size_Section_Current += size;
	writeLabelAndSize(memblock, "SCTN", 4, pos);
	writeInt(memblock, section, pos);
	f->write(memblock, size);
	delete[] memblock;

	for (auto t : list) {
		writeTile(f, memblock, world, size_Section_Current, t);
	}


	size = 8;
	memblock = new char[size]; pos = 0; size_Section_Current += size;
	writeLabelAndSize(memblock, "END", 0, pos);
	f->write(memblock, size);
	delete[] memblock;

	// Fill in the size of data for the SCT section
	temp = size_Section_Current + 4;
	f->seekp(-temp, ios::cur);
	memblock = new char[4]; pos = 0;
	for (int i = 0; i < 4; i++)
		memblock[i] = 0;
	writeInt(memblock, size_Section_Current, pos);
	f->write(memblock, 4);
	delete[] memblock;
	f->seekp(size_Section_Current, ios::cur);

	size_Floor_Current += size_Section_Current + 8;
}

void Serialize::writeTile(ofstream* f, char* memblock, World* world, int& size_Section_Current, tile* t)
{
	int size = 8, pos = 0, temp = 0, size_Tile_Current = 0;
	memblock = new char[size]; 
	writeLabelAndSize(memblock, "TILE", 0, pos);
	f->write(memblock, size);
	delete[] memblock;

	size = 8 + sizeof(int) * 2; pos = 0; size_Tile_Current += size;
	memblock = new char[size];
	writeLabelAndSize(memblock, "COOR", size - 8, pos);
	write_Two_Values_As_Int(memblock, t->getX(), t->getY(), pos);
	write_Two_Values_As_Int(memblock, (char)t->getZ(), t->getID(), pos);
	f->write(memblock, size);
	delete[] memblock;

	if (t->getUID() != 0) {
		size = 8 + 4; pos = 0; size_Tile_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "UID", size - 8, pos);
		writeInt(memblock, t->getUID(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	if (t->getZone() != 0) {
		size = 8 + 1; pos = 0; size_Tile_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "ZONE", size - 8, pos);
		writeChar(memblock, (char)t->getZone(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	if (t->getSpeed() != 100) {
		size = 8 + 1; pos = 0; size_Tile_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "SPD", size - 8, pos);
		writeChar(memblock, (char)t->getSpeed(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	auto item = itemAtlas.getItem(t->getID());

	if (t->getName() != item->getName()) {
		size = 8 + t->getName().size(); pos = 0; size_Tile_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "NAME", size - 8, pos);
		writeString(memblock, t->getName(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	if (t->getArticle() != item->getArticle()) {
		size = 8 + t->getArticle().size(); pos = 0; size_Tile_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "ARTL", size - 8, pos);
		writeString(memblock, t->getArticle(), pos);
		f->write(memblock, size);
		
		delete[] memblock;
	}

	if (t->getDescription() != item->getDescription()) {
		size = 8 + t->getDescription().size(); pos = 0; size_Tile_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "DESC", size - 8, pos);
		writeString(memblock, t->getDescription(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	if (!t->getBlockPathfind()) {
		size = 8 + 1; pos = 0; size_Tile_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "BPTH", size - 8, pos);
		writeChar(memblock, t->getBlockPathfind(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	auto items = t->getAllItems();

	for (auto item : items) {
		writeItem(f, memblock, size_Tile_Current, item);
	}


	size = 8;
	memblock = new char[size]; pos = 0; size_Tile_Current += size;
	writeLabelAndSize(memblock, "END", 0, pos);
	f->write(memblock, size);
	delete[] memblock;

	// Fill in the size of data for the TILE section
	temp = size_Tile_Current + 4;
	f->seekp(-temp, ios::cur);
	memblock = new char[4]; pos = 0;
	for (int i = 0; i < 4; i++)
		memblock[i] = 0;
	writeInt(memblock, size_Tile_Current, pos);
	f->write(memblock, 4);
	delete[] memblock;
	f->seekp(size_Tile_Current, ios::cur);

	size_Section_Current += size_Tile_Current + 8;
}

void Serialize::writeItem(ofstream* f, char* memblock, int& size_Tile_Current, Item* item)
{
	int size = 8, pos = 0, temp = 0, size_Item_Current = 0;
	memblock = new char[size];
	writeLabelAndSize(memblock, "ITEM", 0, pos);
	f->write(memblock, size);
	delete[] memblock;

	size = 8 + 4; pos = 0; size_Item_Current += size;
	memblock = new char[size];
	writeLabelAndSize(memblock, "ID", size - 8, pos);
	writeInt(memblock, item->getID(), pos);
	f->write(memblock, size);
	delete[] memblock;

	if (item->getUID() != 0) {
		size = 8 + 4; pos = 0; size_Item_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "UID", size - 8, pos);
		writeInt(memblock, item->getUID(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	auto itemRef = itemAtlas.getItem(item->getID());

	if (item->getName() != itemRef->getName()) {
		size = 8 + item->getName().size(); pos = 0; size_Item_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "NAME", size - 8, pos);
		writeString(memblock, item->getName(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	if (item->getArticle() != itemRef->getArticle()) {
		size = 8 + item->getArticle().size(); pos = 0; size_Item_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "ARTL", size - 8, pos);
		writeString(memblock, item->getArticle(), pos);
		f->write(memblock, size);

		delete[] memblock;
	}

	if (item->getDescription() != itemRef->getDescription()) {
		size = 8 + item->getDescription().size(); pos = 0; size_Item_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "DESC", size - 8, pos);
		writeString(memblock, item->getDescription(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	if (item->getType() != itemRef->getType()) {
		size = 8 + item->getType().size(); pos = 0; size_Item_Current += size;
		memblock = new char[size];
		writeLabelAndSize(memblock, "TYPE", size - 8, pos);
		writeString(memblock, item->getType(), pos);
		f->write(memblock, size);
		delete[] memblock;
	}

	size = 8 + sizeof(item->getBlockProjectile()) + sizeof(item->getBlockPathfind()) + sizeof(item->getBlockObject()) + sizeof(item->getMoveable()) + sizeof(item->getPickupable()) +
		       sizeof(item->getUseable()) + sizeof(item->getHangeable()) + sizeof(item->getAlwaysOnTop()) + sizeof(item->getAnimationOnly()) + sizeof(item->isDoubleSize());
	pos = 0; size_Item_Current += size;
	memblock = new char[size];
	writeLabelAndSize(memblock, "BOOL", size - 8, pos);
	writeChar(memblock, item->isDoubleSize(), pos);
	writeChar(memblock, item->getBlockProjectile(), pos);
	writeChar(memblock, item->getBlockPathfind(), pos);
	writeChar(memblock, item->getBlockObject(), pos);
	writeChar(memblock, item->getMoveable(), pos);
	writeChar(memblock, item->getPickupable(), pos);
	writeChar(memblock, item->getUseable(), pos);
	writeChar(memblock, item->getHangeable(), pos);
	writeChar(memblock, item->getAlwaysOnTop(), pos);
	writeChar(memblock, item->getAnimationOnly(), pos);
	f->write(memblock, size);
	delete[] memblock;

	if (Weapon* weapon = dynamic_cast<Weapon*>(item)) {
		write_Non_Static_Item(f, memblock, size_Item_Current, weapon);
	}
	else if (Armor* armor = dynamic_cast<Armor*>(item)) {
		write_Non_Static_Item(f, memblock, size_Item_Current, armor);
	}
	else if (NonStaticItem* nonStaticItem = dynamic_cast<NonStaticItem*>(item)) {
		write_Non_Static_Item(f, memblock, size_Item_Current, nonStaticItem);
	}

	size = 8;
	memblock = new char[size]; pos = 0; size_Item_Current += size;
	writeLabelAndSize(memblock, "END", 0, pos);
	f->write(memblock, size);
	delete[] memblock;


	// Fill in the size of data for the ITEM section
	temp = size_Item_Current + 4;
	f->seekp(-temp, ios::cur);
	memblock = new char[4]; pos = 0;
	for (int i = 0; i < 4; i++)
		memblock[i] = 0;
	writeInt(memblock, size_Item_Current, pos);
	f->write(memblock, 4);
	delete[] memblock;
	f->seekp(size_Item_Current, ios::cur);

	size_Tile_Current += size_Item_Current + 8;
}

void Serialize::writeLabelAndSize(char* memblock, std::string label, int size, int& pos)
{
	for (int i = 0; i < 8; i++)
		memblock[i] = 0;

	for (size_t j = 0; j < 4; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if (j < label.size() && (label.at(j) & (1 << i)) != 0) {
				memblock[j] |= (1u << i);
			}
		}
		for (size_t i = 0; i < 8; ++i) {
			if ((size & (1 << (i+(j*8)))) != 0) {
				memblock[j+4] |= (1u << i);
			}
		}
	}
	pos += 8;
}

void Serialize::writeString(char* memblock, string str, int& from)
{
	for (int i = from; i < from + str.size(); i++)
		memblock[i] = 0;
	for (int j = from; j < from+str.size(); ++j) {
		for (int i = 0; i < 8; ++i) {
			if ((str.at(j-from) & (1 << i)) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
	from += str.size();
}

void Serialize::writeInt(char* memblock, int value, int& from)
{
	for (int i = from; i < from+4; i++)
		memblock[i] = 0;
	for (size_t j = from; j < from+4; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((value & (1 << (i + ((j-from) * 8)))) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
	from += 4;
}

void Serialize::write_Two_Values_As_Int(char* memblock, int value1, int value2, int& from)
{
	for (int i = from; i < from + 4; i++)
		memblock[i] = 0;
	for (size_t j = from; j < from + 4; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((j - from) < 2 && (value1 & (1 << (i + ((j - from) * 8)))) != 0) {
				memblock[j] |= (1u << i);
			}
			if ((j - from) > 1 && (value2 & (1 << (i + ((j - from - 2) * 8)))) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
	from += 4;
}

void Serialize::write_Two_Values_As_Int(char* memblock, char value1, int value2, int& from)
{
	for (int i = from; i < from + 4; i++)
		memblock[i] = 0;
	for (size_t j = from; j < from + 4; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((j - from) < 1 && (value1 & (1 << (i + ((j - from) * 8)))) != 0) {
				memblock[j] |= (1u << i);
			}
			if ((j - from) > 0 && (value2 & (1 << (i + ((j - from - 1) * 8)))) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
	from += 4;
}

void Serialize::writeChar(char* memblock, char value, int& from)
{
	for (int i = from; i < from + 1; i++)
		memblock[i] = 0;
	for (size_t j = from; j < from + 1; ++j) {
		for (size_t i = 0; i < 8; ++i) {
			if ((value & (1 << i)) != 0) {
				memblock[j] |= (1u << i);
			}
		}
	}
	from += 1;
}


template<class T>
void Serialize::write_Non_Static_Item(ofstream* f, char* memblock, int& size_Item_Current, T* item)
{
	float weight = item->getWeight();
	float decimals = item->getWeight() - (int)weight;
	while (fmod(decimals, 10) != 0) {
		decimals *= 10;
	}
	int size = 8, pos = 0;
	size = 8 + 4; pos = 0; //size_Item_Current += size;
	memblock = new char[size];
	//writeLabelAndSize(memblock, "WGHT", size - 8, pos);
	//writeInt(memblock, (int)item->getWeight(), pos);
	//writeInt(memblock, (int)decimals, pos);
	//f.write(memblock, size);
	delete[] memblock;
}