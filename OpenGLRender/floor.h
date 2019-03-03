#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#include "Tile.h"
#include <algorithm>

using std::vector;

class Floor {
public:
	Floor() { 
		//for (size_t i = 0; i < 200; ++i) {
			//sections[i] = vector<tile>();
		//}
	};
	~Floor() {
		int i = 0;
		for (auto& section : sections) {
			clearSection(i);
			i++;
		}
	}
	vector<tile*>& getSection(size_t section) { return sections[section]; };
	int& getItemsInSection(int& section) {
		int amount = 0; 
		for (auto& tile : sections[section]) {
			amount += tile->getAllItems().size();
		}
		return amount;
	};
	void setSection(vector<tile*>& other, size_t section) { 
		clearSection(section);
		sections[section].insert(other.begin(), other.begin(), other.end());
	};
	void setNewSection(vector<tile*>& other, size_t section) {
		if(sections[section].size() > 0) clearSection(section);
		for (auto& t : other) {
			sections[section].push_back(new tile(t));
		}
	};
	void clearSection(int section) {
		for (auto& t : sections[section]) {
			delete t;
		}
		sections[section].clear();
		sections[section].shrink_to_fit();
	}
	tile* getTile(int section, int x, int y) {
		auto it = std::find_if(sections[section].begin(), sections[section].end(), [x, y](tile*& t) { return (t->getX() == x && t->getY() == y); });
		if (it != sections[section].end())
			return *it;
		return nullptr;
	};
private:
	vector<tile*> sections[1600];
};
#endif