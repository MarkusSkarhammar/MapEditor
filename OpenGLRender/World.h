#ifndef WORLD_H
#define	WORLD_H

#include "floor.h"
#include <thread>

class World {
public:
	World(std::string name): name(name) {
		//floors[7].getSection(0).push_back(tile(0, 0, 7, 1));
	};
	Floor& getFloor(size_t floor) { return floors[floor]; };
	std::string& getName() { return name; };
private:
	Floor floors[14];
	std::string name;
};

void insert_Things_Form_ThingsToDraw();
void updateWhatToDrawOnAllCurrentFloors();
void updateWhatToDrawOnFloor(size_t floor, bool tiles, bool items);
void updateWhatToDrawOnFloors(size_t fromFloor, size_t toFloor, bool tiles, bool items);
void sortSection(size_t current);
bool checkAbove(int xCheck, int yCheck, int zCheck);
bool checkAbove(int xCheck, int yCheck, int zCheck, bool bigItem);
void newFloor();

#endif