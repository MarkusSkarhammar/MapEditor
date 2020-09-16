#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <vector>
#include "Tile.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "World.h"

using std::vector;
using std::string;
using std::streampos;
using std::ofstream;
using std::ios;
using std::cout;
using std::ifstream;
using std::fstream;

class Serialize {
public:

	Serialize() {};

	/*
	*	Used for saving the currently loaded world.
	*
	*/
	void saveWorld(World* world);

	/*
	*	Used for loading a world.
	*
	*/
	void loadWorld(std::string worldName);

private:

	/*
	* Used for retrieving a char value from a file.
	*
	* @param file, the file of which the char is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the char.
	* @param character, a reference to a char value used for storing the retrieved char.
	*/
	void getChar(ifstream* file, char* memblock, char& character);

	/*
	* Used for retrieving an integer value from a file.
	*
	* @param file, the file of which the integer is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the integer.
	* @param integer, a reference to an integer value used for storing the retrieved integer.
	*/
	void getInt(ifstream* file, char* memblock, int& integer);

	/*
	* Used for retrieving two integer values from 4 bytes from a file.
	*
	* @param file, the file of which the integers are to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the two integers.
	* @param integer1, a reference to an integer value used for storing the first retrieved integer.
	* @param integer2, a reference to an integer value used for storing the second retrieved integer.
	*/
	void get_Two_Values_From_Int(ifstream* file, char* memblock, int& integer1, int& integer2);

	/*
	* Used for retrieving a char value and an integer value from 4 bytes from a file.
	*
	* @param file, the file of which the char and integer are to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the char and integer.
	* @param character, a reference to a char value used for storing the retrieved char.
	* @param integer, a reference to an integer value used for storing the retrieved integer.
	*/
	void get_Two_Values_From_Int(ifstream* file, char* memblock, char& character, int& integer);

	/*
	* Used for retrieving a string from a file.
	*
	* @param file, the file of which the string is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the string.
	* @param tempValueHolder, a reference to a char value used for storing a char from the string temporarily.
	* @param str, a reference to a string used for storing the retrieved string.
	* @param from, an integer used to determine the start position of the string in array of chars.
	* @param size, an integer used to determine the size of the string to get.
	*/
	void getString(ifstream* file, char* memblock, char& tempValueHolder, std::string& str, int from, int size);

	/*
	* Used for retrieving a label from the file. A label contains a 4 byte string followed by a 4 bytes integer(the integer tells the size of the data after the label).
	*
	* @param file, the file of which the label is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the label.
	* @param tempValueHolder, a reference to a char value used for storing a char from the label temporarily.
	* @param label, a reference to a string used for storing the retrieved label.
	* @param size, an integer used to determine the size of the data that follows after the label.
	*/
	void getNextLabel(ifstream* file, char* memblock, char& tempValueHolder, std::string& label, int& size);

	/*
	* Used for retrieving the header information of a world.
	*
	* @param file, the file of which the header information is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the header information.
	* @param tempValueHolder, a reference to a char value used for storing a char temporarily.
	* @param name, a reference to a string used for storing the retrieved name of the world.
	* @param floors, a reference to an integer used for storing the retrieved amout of floors of the world.
	* @param width, a reference to an integer used for storing the retrieved amount of sections in a row of the world.
	* @param height, a reference to an integer used for storing the retrieved amount of sections in a column of the world.
	* @param width_Tiles, a reference to an integer used for storing the retrieved amount of sections tiles in a section of the world.
	* @param header_Size, a reference to an integer used for storing the size of the header section of the file.
	*/
	void getHeaderInfo(ifstream* file, char* memblock, char& tempValueHolder, std::string& name, int& floors, int& width, int& height, int& width_Tiles, int& header_Size);

	/*
	* Used for retrieving the data of a world. 
	* The data is saved in the following format:
	* 
	* [ label 4 bytes][ size 4 bytes ][ [ label 4 bytes][ size 4 bytes ][ data ][ label 4 bytes][ size 4 bytes ][ data ] ] -> [ label 4 bytes][ size 4 bytes ][ data ] etc.
	* 
	* \nLabels:
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
	* X(X coordinate) = x coordinate.
	* Y(Z coordinate) = y coordinate.
	* T(Z cooordinate) = z coordinate.
	* ID
	* UID = Unique ID.
	* ZONE = The zone of the tile.
	* SPD(SPeeD) = The speed of the tile
	* NAME
	* ARTL(ARTicLe)
	* DESC(DESCription)
	* BPTH(Block PaTHfinding)
	* BOOL(BOOLean) = The various booleans of an item(Note the are all store in one of these sections and not one for each).
	* WGHT(WeiGHT) = The weight of the item.
	*
	* @param filePath, a string containing the path to the file.
	* @param file, the file of which the header information is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the header information.
	* @param world, a pointer to the world of which the retrieved data will be put into.
	* @param header_Size, a reference to an integer used for storing the size of the header section of the file.
	*/
	void getWorldData(std::string& filePath, ifstream* file, char* memblock, World* world, int& header_Size);

	/*
	* Used for multithreading the loading of a world.
	*
	* @param file, the file of which the header information is to be retrieved from.
	* @param path, a refrence string containing the path to the file.
	* @param world, a pointer to the world of which the retrieved data will be put into.
	* @param header_Size, a reference to an integer used for storing the size of the header section of the file.
	*/
	void floor_Thread_Handler(ifstream* file, std::string& path, World* world, const int& header_Size);
	
	/*
	* Used for retrieving a floor of a world.
	*
	* @param file, the file of which the header information is to be retrieved from.
	* @param world, a pointer to the world of which the retrieved data will be put into.
	* @param floor, a const integer used for telling the current floor.
	* @param sections, an unordered_map containing the sections to be loaded.
	*/
	void getLevel(ifstream* file, World* world, const int floor, std::unordered_map<int, int>& sections);

	/*
	* Used for retrieving the header information of a world.
	*
	* @param file, the file of which the header information is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the header information.
	* @param tempValueHolder, a reference to a char value used for storing a char temporarily.
	* @param world, a pointer to the world of which the retrieved data will be put into.
	* @param size, a reference to an integer used for storing retrieved values temporarily.
	* @param label, a reference to a string used for storing retrieved strings temporarily.
	* @param floor, a const integer used for telling the current floor.
	* @param section, a const integer used for telling the current section.
	* @param section_Data_Total, a reference to an integer used for counting the current section's total data.
	*/
	void getSection(ifstream* file, char* memblock, char& tempValueHolder, World* world, int& size, std::string& label, const int& floor, const int& section, int& section_Data_Total);

	/*
	* Used for retrieving the header information of a world.
	*
	* @param file, the file of which the header information is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the header information.
	* @param tempValueHolder, a reference to a char value used for storing a char temporarily.
	* @param world, a pointer to the world of which the retrieved data will be put into.
	* @param size, a reference to an integer used for storing retrieved values temporarily.
	* @param label, a reference to a string used for storing retrieved strings temporarily.
	* @param floor, a const integer used for telling the current floor.
	* @param section, a reference to a vector containing pointers to tiles used for storing retrieved tiles.
	*/
	void getTile(ifstream* file, char* memblock, char& tempValueHolder, World* world, int& size, std::string& label, std::vector<tile*>& section);

	/*
	* Used for retrieving the header information of a world.
	*
	* @param file, the file of which the header information is to be retrieved from.
	* @param memblock, a pointer to an array of chars used for retrieving the bytes that make up the header information.
	* @param tempValueHolder, a reference to a char value used for storing a char temporarily.
	* @param world, a pointer to the world of which the retrieved data will be put into.
	* @param size, a reference to an integer used for storing retrieved values temporarily.
	* @param label, a reference to a string used for storing retrieved strings temporarily.
	* @param floor, a const integer used for telling the current floor.
	* @param items, a reference to a vector containing pointers to items used for storing retrieved items.
	*/
	void getItems(ifstream* file, char* memblock, char& tempValueHolder, World* world, int& size, std::string& label, std::vector<Item*>& items);

	/*
	* Used for writing data from a world to a file.
	*
	* @param fileName, a string containing the name of the world.
	* @param world, a pointer to the world of which contains the data to be written down.
	*/
	void writeWorldToFile(string fileName, World* world);

	/*
	* Used for handling of multithreaded writing of a world.
	*
	* @param filePath, a const reference of a string containing the filepath of the world.
	* @param world, a pointer to the world of which contains the data to be written down.
	*/
	void write_Floor_Thread_Handler(const string& filePath, World* world);

	/*
	* Used for overwriting a section of a world to a file which already contains data of the world.
	*
	* @param filePath, a const reference of a string containing the filepath of the world.
	* @param fIn, the ifstream used for reading the data that is currently saved to the file.
	* @param fOut, the ofstream used for writing the data to the file.
	* @param world, a pointer to the world of which contains the data to be written down.
	* @param section, a pointer to the section of which contains the data to be written down.
	* @param length, a reference to an integer which is used to store the length of the previous data size of the section to be overwritten.
	* @param after_Start_Pos, a const integer containing the already traversed amount of the section to be overwritten.
	* @param floorStartPos, a const integer containing the already start position of the floor in the file.
	*/
	void overwrite_Section(const string& filePath, ifstream*& fIn, ofstream*& fOut, World* world, Section* section, int& length, const int after_Start_Pos, const int floorStartPos);

	/*
	* Used for inserting a section of a world to a file which already contains data of the world.
	*
	* @param filePath, a const reference of a string containing the filepath of the world.
	* @param fIn, the ifstream used for reading the data that is currently saved to the file.
	* @param fOut, the ofstream used for writing the data to the file.
	* @param world, a pointer to the world of which contains the data to be written down.
	* @param section, a pointer to the section of which contains the data to be written down.
	* @param floorStartPos, a const integer containing the already start position of the floor in the file.
	*/
	void insert_Section(const string& filePath, ifstream*& fIn, ofstream*& fOut, World* world, Section* section, const int floorStartPos);

	/*
	* Used for writing the world info of a world to a file.
	*
	* @param f, the ofstream used for writing the data to the file.
	* @param memblock, a pointer to an array of chars used for temporary storage of bytes.
	* @param world, a pointer to the world of which contains the data to be written down.
	* @param header_Size, a reference to an integer used for storing the size of the header section of the file.
	*/
	void writeWorldInfo(ofstream* f, char* memblock, World* world, int& headerSize);

	/*
	* Used for writing the world data of a world to a file.
	*
	* @param f, the ofstream used for writing the data to the file.
	* @param world, a pointer to the world of which contains the data to be written down.
	* @param header_Size, a reference to an integer used for storing the size of the header section of the file.
	*/
	void writeWorldData(ofstream* f, World* world, const int& header_Size);

	/*
	* Used for writing the floor of a world to a file.
	*
	* @param f, the ofstream used for writing the data to the file.
	* @param world, a pointer to the world of which contains the data to be written down.
	* @param from, the first floor to write.
	* @param to, the last floor to write.
	* @param size_World_Total, a reference to an integer used for storing the total size of the world.
	*/
	void writeFloor(ofstream* f, World* world, int from, int to, int& size_World_Total);

	/*
	* Used for writing the section of a world to a file.
	*
	* @param f, the ofstream used for writing the data to the file.
	* @param memblock, a pointer to an array of chars used for temporary storage of bytes.
	* @param world, a pointer to the world of which contains the data to be written down.
	* @param floor_Size, a reference to an integer which contains the size of the floor.
	* @param section, a const integer of the current section.
	* @param list, a reference to a vector of tile pointers containing the tiles of the section.
	*/
	void writeSection(ofstream* f, char* memblock, World* world, int& floor_Size, const int section, std::vector<tile*>& list);

	/*
	* Used for writing the section of a world to a file.
	*
	* @param f, the ofstream used for writing the data to the file.
	* @param memblock, a pointer to an array of chars used for temporary storage of bytes.
	* @param world, a pointer to the world of which contains the data to be written down.
	* @param section_Size, a reference to an integer which contains the size of the section.
	* @param t, a reference to a tile pointer.
	*/
	void writeTile(ofstream* f, char* memblock, World* world, int& section_Size, tile* t);

	/*
	* Used for writing the section of a world to a file.
	*
	* @param f, the ofstream used for writing the data to the file.
	* @param memblock, a pointer to an array of chars used for temporary storage of bytes.
	* @param tile_Size, a reference to an integer which contains the size of the tile.
	* @param t, a reference to an item.
	*/
	void writeItem(ofstream* f, char* memblock, int& tile_Size, Item* t);

	/*
	* Used for writing the section of a world to a file.
	*
	* @param T, a template of a class.
	* @param f, the ofstream used for writing the data to the file.
	* @param memblock, a pointer to an array of chars used for temporary storage of bytes.
	* @param item_Current_Size, a reference to an integer which contains the size of the item.
	* @param t, a reference to an item.
	*/
	template <class T>
	void write_Non_Static_Item(ofstream* f, char* memblock, int& item_Current_Size, T* item);

	/*
	* Used for writing labels and sizes.
	* 
	* @param memblock, a pointer to an array of chars used for writing to a file.
	* @param label, a string containing the label to be written.
	* @param size, an integer containing the size to be written down.
	* @param pos, a reference to an integer used for keeping track of the position to write to in the char array.
	*/
	void writeLabelAndSize(char* memblock, std::string label, int size, int& pos);

	/*
	* Used for writing strings.
	*
	* @param memblock, a pointer to an array of chars used for writing to a file.
	* @param str, a string containing the string to be written.
	* @param from, a reference to an integer used for which position in the char array to start to write to.
	*/
	void writeString(char* memblock, string str, int& from);

	/*
	* Used for writing integers.
	*
	* @param memblock, a pointer to an array of chars used for writing to a file.
	* @param value, an integer containing the value to be written.
	* @param from, a reference to an integer used for which position in the char array to start to write to.
	*/
	void writeInt(char* memblock, int value, int& from);

	/*
	* Used for writing two integers to one integer in the file.
	*
	* @param memblock, a pointer to an array of chars used for writing to a file.
	* @param value1, an integer containing the first value to be written.
	* @param value2, an integer containing the second value to be written.
	* @param from, a reference to an integer used for which position in the char array to start to write to.
	*/
	void write_Two_Values_As_Int(char* memblock, int value1, int value2, int& from);

	/*
	* Used for writing an integer and a char to one integer in the file.
	*
	* @param memblock, a pointer to an array of chars used for writing to a file.
	* @param value1, a char containing the first value to be written.
	* @param value2, an integer containing the second value to be written.
	* @param from, a reference to an integer used for which position in the char array to start to write to.
	*/
	void write_Two_Values_As_Int(char* memblock, char value1, int value2, int& from);

	/*
	* Used for writing an integer and a char to one integer in the file.
	*
	* @param memblock, a pointer to an array of chars used for writing to a file.
	* @param value, a char containing a value to be written.
	* @param from, a reference to an integer used for which position in the char array to start to write to.
	*/
	void writeChar(char* memblock, char value, int& from);
	
};

#endif