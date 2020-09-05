#ifndef WORLD_H
#define	WORLD_H

#include "floor.h"
#include <thread>

class World {
public:
	World(std::string name): name(name) {
		//floors[7].getSection(0).push_back(tile(0, 0, 7, 1));
	};
	World(World& w) {
		void insertWorld(World& w);
	};
	void insertWorld(World& w) {
		name = w.getName();
		for (int floor = 0; floor < 14; floor++) {
			for (int section = 0; section < 1600; section++) {
				getFloor(floor).setNewSection(w.getFloor(floor).getSection(section), section);
			}
		}
	}
	void copyWorld(World& w) {
		w.setName(w.getName());
		for (int floor = 0; floor < 14; floor++) {
			for (int section = 0; section < 1600; section++) {
				getFloor(floor).setNewSection(w.getFloor(floor).getSection(section), section);
			}
		}
	}
	void deleteWorld() {
		for (auto& floor : floors) {
			for (int section = 0; section < 1600; section++) {
				floor.clearSection(section);
			}
		}
	}
	Floor& getFloor(size_t floor) { return floors[floor]; };
	std::string& getName() { return name; };
	void setName(std::string v) { name = v; };
private:
	Floor floors[14];
	std::string name;
};

void insert_Things_From_ThingsToDraw(World& world);
void updateWhatToDrawOnAllCurrentFloors();
void updateWhatToDrawOnFloor(size_t floor, bool tiles, bool items);
void updateWhatToDrawOnFloors(size_t fromFloor, size_t toFloor, bool tiles, bool items);
void sortSection(size_t current);
bool checkAbove(int xCheck, int yCheck, int zCheck, int section);
bool checkAbove(int xCheck, int yCheck, int zCheck, int section, bool bigItem);
void newFloor();

#endif