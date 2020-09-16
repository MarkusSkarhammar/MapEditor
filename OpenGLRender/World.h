#ifndef WORLD_H
#define	WORLD_H

#include "floor.h"
#include <thread>

class World {
public:

	World(std::string name, int floors, int width, int height, int width_Tiles) : name(name), width_Section(width), height_Section(height), width_Section_Tiles(width_Tiles) {
		for (int i = 0; i < floors; i++) {
			this->floors.push_back(new Floor(i, width, height));
		}
	};

	World(World* w) {
		copyWorldPrivate(w);
	};

	~World() {
		deleteWorld();
	 }

	void insertWorld(World* w) {
		insertWorldPrivate(w);
	}

	void copyWorld(World* w) {
		copyWorldPrivate(w);
	}

	void deleteWorld() {
		for (auto f : floors) {
			delete f;
		}
		floors.clear();
		floors.shrink_to_fit();
	}

	void clear_World() {
		for (auto f : floors) {
			f->clear();
		}
	}

	void insert_Sections_From_Other(World* other, int floor, std::unordered_map<int, int>& list);

	int get_Width_Section() { return width_Section; };
	int get_Height_Section() { return height_Section; };
	int get_Width_Section_Tiles() { return width_Section_Tiles; };
	int get_Section_Max() { return width_Section * height_Section; };
	int get_Floor_Amount() { return floors.size(); };
	int get_Tiles_Amount();
	Floor* getFloor(size_t floor) { return floors[floor]; };
	std::vector<Floor*> getFloors() { return floors; };
	std::string& getName() { return name; };
	void setName(std::string v) { name = v; };
	
	std::unordered_map<int, int> check_If_Updated_Sections(int floor);

private:
	int width_Section{ 0 }, height_Section{ 0 }, width_Section_Tiles{ 0 };
	std::vector<Floor*> floors;
	std::string name, description{"Test."};

	void copyWorldPrivate(World* w) {
		name = w->getName();
		width_Section = w->get_Width_Section();
		height_Section = w->get_Height_Section();
		width_Section_Tiles = w->get_Width_Section_Tiles();

		if (floors.size() == 0) {
			for (auto* floor : w->getFloors()) {
				floors.push_back(new Floor(floor));
			}
		}

	}

	void insertWorldPrivate(World* w) {
		for (auto floor : w->getFloors()) {
			for (auto sectionRef : floor->get_Sections()) {
				this->getFloor(floor->get_Floor())->hand_Over_Section(sectionRef, sectionRef->get_ID());
			}
		}
	}
};

void insert_Things_From_ThingsToDraw(World* world);
void updateWhatToDrawOnAllCurrentFloors();
void updateWhatToDrawOnFloor(size_t floor, bool tiles, bool items);
void updateWhatToDrawOnFloors(size_t fromFloor, size_t toFloor, bool tiles, bool items);
void sortSection(World* world, int floor, int section);
bool checkAbove(int xCheck, int yCheck, int zCheck, int section);
bool checkAbove(int xCheck, int yCheck, int zCheck, int section, bool bigItem);
void newFloor();

/*
*	Used for getting the currently rendered sections of the map.
*
*	@return std::unordered_map<int, int>, a map containing the rendered sections.
*
*/
std::unordered_map<int, int> get_Currently_Rendered_Sections();

/*
*	Used for getting the currently loaded sections of the map.
*
*	@return std::unordered_map<int, int>, a map containing the loaded sections.
*
*/
std::unordered_map<int, int> get_Currently_Loaded_Sections();


/*
* 
* Used for determining and updating which sections are loaded.
*
*/
void check_If_Load_New_Sections();

#endif
