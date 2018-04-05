#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#include "Tile.h"

using std::vector;

class Floor {
public:
	Floor() { 
		//for (size_t i = 0; i < 200; ++i) {
			//sections[i] = vector<tile>();
		//}
	};
	vector<tile>& getSection(size_t section) { return sections[section]; };
	void setSection(vector<tile>& other, size_t section) { sections[section] = other; };
private:
	vector<tile> sections[250];
};
#endif