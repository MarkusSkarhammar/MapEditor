#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#include "Tile.h"
#include <algorithm>

using std::vector;

class Section {
public:
	Section(int ID, int floor) : ID(ID), floor(floor) {};
	Section(int ID, int floor, vector<tile*> section) : ID(ID), floor(floor), section(section) {};
	Section(Section* other) {
		this->ID = other->get_ID();
		this->floor = other->get_Floor();
		for (auto t : other->get_Tiles()) {
			section.push_back(new tile(t));
		}
	};
	~Section() {
		for (auto t : section) {
			delete t;
		}
		section.clear();
		section.shrink_to_fit();
	};

	void set_ID(int ID) { this->ID = ID; };
	int get_ID() { return ID; };
	void set_Floor(int floor) { this->floor = floor; };
	int get_Floor() { return floor; };
	bool get_Changed() { return changed; };
	void set_Changed(bool b) { changed = b; };

	void add_Tile(tile* t) {
		section.push_back(t);
	}
	vector<tile*>& get_Tiles() { return section; };

private:
	int ID{ 0 }, floor{ 0 };
	bool changed{ 0 };
	vector<tile*> section;
};

class Floor {
public:

	Floor(int floor, int width, int height) : floor(floor) { 
		
	};

	Floor(Floor* other) {
		this->floor = other->get_Floor();
		for (auto section : other->get_Sections()) {
			sections.push_back(new Section(section));
		}
	};

	~Floor() {
		for (auto section : sections) {
			delete section;
		}
		sections.clear();
		sections.shrink_to_fit();
	}

	int get_Floor() { return floor; };

	vector<Section*>& get_Sections() { return sections; };

	Section* get_Section(int section) {
		auto it = std::find_if(sections.begin(), sections.end(), [section](Section* s) {
			return s->get_ID() == section;
			});
		if (it != sections.end())
			return *it;
		else
			return nullptr;
	};

	int get_Amount_Of_Items_In_Section(int& section) {
		auto sectionPtr = get_Section(section);
		if (sectionPtr) {
			int amount = 0;
			for (auto& tile : get_Section(section)->get_Tiles()) {
				amount += tile->getAllItems().size();
			}
			return amount;
		}
		else
			return 0;
	};

	Section* add_Section(Section* s, int ID) {
		auto sectionPtr = get_Section(ID);
		if (!sectionPtr) {
			sections.push_back(s);
		}
		return s;
	}

	void set_Section(Section* s, size_t section) {
		auto sectionPtr = clearSection(section);
		if (!sectionPtr)
			sectionPtr = add_Section(new Section(s), section);
		else {
			for (auto t : s->get_Tiles()) {
				sectionPtr->add_Tile(new tile(t));
			}
		}
		if (s->get_Changed())
			sectionPtr->set_Changed(true);
	};

	void hand_Over_Section(Section* s, int section) {
		auto sectionPtr = get_Section(section);
		if (!sectionPtr)
			sectionPtr = add_Section(new Section(section, s->get_Floor()), section);
		for (auto t : s->get_Tiles()) {
			sectionPtr->add_Tile(t);
		}
		s->get_Tiles().clear();
		if (s->get_Changed())
			sectionPtr->set_Changed(true);
	};

	void insert_Section(Section* s, size_t section) {
		auto sectionPtr = get_Section(section);
		if (!sectionPtr)
			sectionPtr = add_Section(new Section(s), section);
		else
			for (auto t : sectionPtr->get_Tiles()) {
				sectionPtr->add_Tile(new tile(t));
			}
	};

	Section* clearSection(int section) {
		auto sectionPtr = get_Section(section);
		if (sectionPtr) {
			auto& sectionRef = sectionPtr->get_Tiles();
			for (auto& t : sectionRef) {
				delete t;
			}
			sectionRef.clear();
			sectionRef.shrink_to_fit();
			return sectionPtr;
		}
		else
			nullptr;
	}

	void clear() {
		for (auto& section : sections) {
			delete section;
		}
		sections.clear();
		sections.shrink_to_fit();
	};

	tile* getTile(int section, int x, int y) {
		auto sectionPtr = get_Section(section);
		if (sectionPtr) {
			auto tilesRef = get_Section(section)->get_Tiles();
			auto it = std::find_if(tilesRef.begin(), tilesRef.end(), [x, y](tile* t) { return (t->getX() == x && t->getY() == y); });
			if (it != tilesRef.end())
				return *it;
			return nullptr;
		}
		return nullptr;
	};

private:
	int floor{ 0 };
	vector<Section*> sections;
};
#endif