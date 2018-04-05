#ifndef WORLD_H
#define	WORLD_H

#include "floor.h"

class World {
public:
	World() {
		for (size_t i = 0; i < 14; ++i) {
			floors[i] = Floor();
		}
		//floors[7].getSection(0).push_back(tile(0, 0, 7, 1));
	};
	Floor& getFloor(size_t floor) { return floors[floor]; };
private:
	Floor floors[14];
};

#endif